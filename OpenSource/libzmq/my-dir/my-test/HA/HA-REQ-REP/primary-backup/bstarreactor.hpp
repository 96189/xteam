
#ifndef _B_STAR_REACTOR_HPP_
#define _B_STAR_REACTOR_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

// 主机状态
typedef enum 
{
    STATE_NONE = 0,     // 无效状态
    STATE_PRIMARY = 1,  // 主机
    STATE_BACKUP = 2,   // 备机
    STATE_ACTIVE = 3,   // 主动(活跃)状态
    STATE_PASSIVE = 4   // 备机(非活跃)状态
}State_t;

// 事件类型
typedef enum 
{
    PEER_NONE = 0,      // 无效事件类型
    PEER_PRIMARY = 1,   // 其他机器是主机
    PEER_BACKUP = 2,    // 其他机器是备机
    PEER_ACTIVE = 3,    // 其他机器处于活跃状态
    PEER_PASSIVE = 4,   // 其他机器处于非活跃状态
    CLIENT_REQUEST = 5  // 接收到客户端请求
}Event_t;

#define BSTAR_HEARTBEAT     1000    // ms

class BStarReactor 
{
private:
    void *context_;
    zloop_t *loop_;         // reactor loop
    void *statepub_;        // 状态发布套接字
    void *statesub_;        // 状态订阅套接字
    
    State_t state_;         // 当前服务器状态
    Event_t event_;         // peer服务器状态 或 客户端请求
    int64_t peer_expiry_;   // peer过期时间
    
    zloop_fn *voter_fn_;    // 具体的业务执行逻辑
    void *voter_arg_;
    
    zloop_fn *active_fn_;   // 活跃状态回调函数
    void *active_arg_;
    
    zloop_fn *passive_fn_;  // 非活跃状态回调函数
    void *passive_arg_;

public:
// 构造 析构
    BStarReactor()
        : statepub_(NULL), statesub_(NULL), state_(STATE_NONE), event_(PEER_NONE), peer_expiry_(0),
          voter_fn_(NULL), voter_arg_(NULL), active_fn_(NULL), active_arg_(NULL), passive_fn_(NULL), passive_arg_(NULL)
    {
        context_ = zmq_init(1);
        loop_ = zloop_new();
        state_ = STATE_NONE;
    }
    ~BStarReactor()
    {
        Destroy();
        zloop_destroy(&this->loop_);
        zmq_term(context_);
    }
// 资源初始化 资源释放
    void Init(const int primary, const char *local, const char *remote)
    {
        state_ = primary ? STATE_PRIMARY : STATE_BACKUP;
        
        statepub_ = zmq_socket(context_, ZMQ_PUB);
        int ret = zmq_bind(statepub_, local);
        assert(ret != -1);

        statesub_ = zmq_socket(context_, ZMQ_SUB);
        zmq_setsockopt(statesub_, ZMQ_SUBSCRIBE, "", 0);
        ret = zmq_connect(statesub_, remote);
        assert(ret != -1);

        zloop_timer(this->loop_, BSTAR_HEARTBEAT, 0, SendState, this);  // 注册状态更新回调-状态发送
        zmq_pollitem_t poller = { this->statesub_, 0, ZMQ_POLLIN };
        zloop_poller(this->loop_, &poller, RecvState, this);    // 注册状态更新回调-状态接收
    }
    void Destroy()
    {
        zmq_close(statesub_);
        zmq_close(statepub_);
    }
// 回调函数
    // 状态更新回调-状态发送
    static int SendState(zloop_t *loop, int timerId, void *arg)
    {
        BStarReactor *self = (BStarReactor *)arg;
        zstr_sendf(self->statepub_, "%d", self->state_);
        return 0;
    }
    // 状态更新回调-状态接收
    static int RecvState(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
    {
        BStarReactor *self = (BStarReactor *)arg;
        char *state = zstr_recv(poller->socket);
        if (state)
        {
            self->event_ = (Event_t)atoi(state);
            self->UpdatePeerExpiry();
            free(state);
        }
        return self->ExecuteFsm();
    }
    // 业务逻辑是否可执行回调
    static int VoterReady(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
    {
        BStarReactor *self = (BStarReactor *)arg;
        self->event_ = CLIENT_REQUEST;
        if (!self->ExecuteFsm())
        {
            self->voter_fn_(self->loop_, poller, self->voter_arg_);
        }
        else 
        {
            zmsg_t *msg = zmsg_recv(poller->socket);
            zmsg_destroy(&msg);
        }
        return 0;
    }
// 
    void UpdatePeerExpiry()
    {
        this->peer_expiry_ = zclock_time() + 2 * BSTAR_HEARTBEAT;
    }
    // 有限状态机 execute finite state machine
    int ExecuteFsm()
    {
        int rc = 0;
        if (this->state_ == STATE_PRIMARY)
        {
            if (this->event_ == PEER_BACKUP)
            {
                zclock_log("I: connected to backup (passive), ready as active");
                this->state_ = STATE_ACTIVE;
                if (this->active_fn_)
                    this->active_fn_(this->loop_, NULL, this->active_arg_);
            }
            else if (this->event_ == PEER_ACTIVE)
            {
                zclock_log("I: connected to backup (active), ready as passive");
                this->state_ = STATE_PASSIVE;
                if (this->passive_fn_)
                    this->passive_fn_(this->loop_, NULL, this->passive_arg_);
            }
            else if (this->event_ == CLIENT_REQUEST)
            {
                assert(this->peer_expiry_ > 0);
                if (zclock_time() >= this->peer_expiry_)
                {
                    zclock_log("I: request from client, ready as active");
                    this->state_ = STATE_ACTIVE;
                    if (this->active_fn_)
                        this->active_fn_(this->loop_, NULL, this->active_arg_);
                }
                else 
                {   
                    rc = -1;
                }
            }
        }
        else if (this->state_ == STATE_BACKUP)
        {
            if (this->event_ == PEER_ACTIVE)
            {
                zclock_log("I: connected to primary (active), ready as passive");
                this->state_ = STATE_PASSIVE;
                if (this->passive_fn_)
                    this->passive_fn_(this->loop_, NULL, this->passive_arg_);
            }
            else if (this->event_ == CLIENT_REQUEST)
            {
                rc = -1;
            }
        }
        else if (this->state_ == STATE_ACTIVE)
        {
            if (this->event_ == PEER_ACTIVE)
            {
                zclock_log("E: fatal error - dual actives, aborting");
                rc = -1;
            }
        }
        else if (this->state_ == STATE_PASSIVE)
        {
            if (this->event_ == PEER_PRIMARY)
            {
                zclock_log("I: primary (passive) is restarting, ready as active");
                this->state_ = STATE_ACTIVE;
            }
            else if (this->event_ == PEER_BACKUP)
            {
                zclock_log("I: backup (passive) is restarting, ready as active");
                this->state_ = STATE_ACTIVE;
            }
            else if (this->event_ == PEER_PASSIVE)
            {
                zclock_log("E: fatal error - dual passives, aborting");
                rc = -1;
            }
            else if (this->event_ == CLIENT_REQUEST)
            {
                assert(this->peer_expiry_ > 0);
                if (zclock_time() >= this->peer_expiry_)
                {
                    zclock_log("I: failover successful, ready as active");
                    this->state_ = STATE_ACTIVE;
                }
                else 
                {
                    rc = -1;
                }
            }

            if (this->state_ == STATE_ACTIVE && this->active_fn_)
            {
                this->active_fn_(this->loop_, NULL, this->active_arg_);
            }
        }
        return rc;
    }
    zloop_t *Zloop()
    {
        return this->loop_;
    }
    // 对外提供接口 注册具体的业务处理函数
    int Voter(char *endpoint, int type, zloop_fn handler, void *arg)
    {
        void *socket = zmq_socket(this->context_, type);
        int ret = zmq_bind(socket, endpoint);
        assert(ret != -1);
        
        assert(!this->voter_fn_);
        this->voter_fn_ = handler;
        this->voter_arg_ = arg;
        zmq_pollitem_t poller = { socket, 0, ZMQ_POLLIN };
        return zloop_poller(this->loop_, &poller, VoterReady, this);    // 注册业务逻辑是否可执行回调
    }
    // 对外提供接口 注册具体的非活跃状态处理函数
    void NewActive(zloop_fn handler, void *arg)
    {
        assert(!this->active_fn_);
        this->passive_fn_ = handler;
        this->passive_arg_ = arg;
    }
    // 对外提供接口 注册具体的活跃状态处理函数
    void NewPassive(zloop_fn handler, void *arg)
    {
        assert(!this->passive_fn_);
        this->passive_fn_ = handler;
        this->passive_arg_ = arg;
    }
    // 执行主时间循环reactor loop
    int Start()
    {
        assert(this->voter_fn_);
        this->UpdatePeerExpiry();
        return zloop_start(this->loop_);
    }
    // 设置日志输出级别
    void SetVerbose(bool verbose)
    {
        zloop_set_verbose(this->loop_, verbose);
    }
};

#endif