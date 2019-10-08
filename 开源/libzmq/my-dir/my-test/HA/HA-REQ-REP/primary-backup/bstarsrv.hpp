
//

#ifndef _B_STAR_SRV_HPP_
#define _B_STAR_SRV_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

// config parse
#define PRIMARY_PUB_ADDR "tcp://*:5003"
#define PRIMARY_SUB_ADDR "tcp://localhost:5004"
#define PRIMARY_FRONTEND_ADDR "tcp://*:5001"

#define BACKUP_PUB_ADDR "tcp://*:5004"
#define BACKUP_SUB_ADDR "tcp://localhost:5003"
#define BACKUP_FRONTEND_ADDR "tcp://*:5002"

#define HEARTBEAT 1000          // ms

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

class BStarSrv 
{
private:
    void *context_;
    void *statepub_;            // 当前服务器的发布套接字
    void *statesub_;            // 当前服务器的订阅套接字
    void *frontend_;            // 提供给客户端连接的端点

    State_t curstate_;          // 当前服务器状态
    Event_t curevent_;          // 当前发生的事件 主备机状态改变 或者 接收到客户端请求
    int64_t peer_expiry_;       // 主备机互相持有对方心跳

public:
// 构造析构
    BStarSrv()
        : context_(NULL), statepub_(NULL), statesub_(NULL), frontend_(NULL), curstate_(STATE_NONE), curevent_(PEER_NONE), peer_expiry_(0)
    {

    }
    ~BStarSrv()
    {
        Destroy();
    }

    // 资源初始化
    void Init(const char *mode)
    {
        context_ = zmq_init(1);
        statepub_ = zmq_socket(context_, ZMQ_PUB);
        statesub_ = zmq_socket(context_, ZMQ_SUB);
        // 订阅套接字必须要订阅消息才能收到消息
        zmq_setsockopt(statesub_, ZMQ_SUBSCRIBE, "", 0);
        frontend_ = zmq_socket(context_, ZMQ_ROUTER);
        // 主机模式启动
        if (streq(mode, "-p"))
        {
            printf ("I: Primary active, waiting for backup (passive)\n");
            curstate_ = STATE_PRIMARY;
            int ret = zmq_bind(frontend_, PRIMARY_FRONTEND_ADDR);
            assert(ret != -1);

            ret = zmq_bind(statepub_, PRIMARY_PUB_ADDR);
            assert(ret != -1);
            ret = zmq_connect(statesub_, PRIMARY_SUB_ADDR);
            assert(ret != -1);
            printf("Primary server binding...\nfrontend [%s]\nstatepub [%s]\n", PRIMARY_FRONTEND_ADDR, PRIMARY_PUB_ADDR);
        }
        // 备机模式启动
        else if (streq(mode, "-b"))
        {
            printf ("I: Backup passive, waiting for primary (active)\n");
            curstate_ = STATE_BACKUP;
            int ret = zmq_bind(frontend_, BACKUP_FRONTEND_ADDR);
            assert(ret != -1);

            ret = zmq_bind(statepub_, BACKUP_PUB_ADDR);
            assert(ret != -1);
            ret = zmq_connect(statesub_, BACKUP_SUB_ADDR);
            assert(ret != -1);
            printf("Backup server binding...\nfrontend [%s]\nstatepub [%s]\n", BACKUP_FRONTEND_ADDR, BACKUP_PUB_ADDR);
        }
        else 
        {
            assert(false);
        }
    }
    // 资源清理
    void Destroy()
    {
        zmq_close(statepub_);
        zmq_close(statesub_);
        zmq_close(frontend_);
        zmq_term(context_);
    }
    // 状态转换(是否发生异常)
    bool StateMachine()
    {
        bool exception = false;

        // 当前机器是主机
        if (this->curstate_ == STATE_PRIMARY)
        {
            // 其他机器是备机
            if (this->curevent_ == PEER_BACKUP)
            {
                printf ("I: connected to backup (passive), ready active\n");
                // 当前机器转入主动(活跃)状态
                this->curstate_ = STATE_ACTIVE;
            }
            // 其他机器已经在主动(活跃)状态
            else if (this->curevent_ == PEER_ACTIVE)
            {
                printf ("I: connected to backup (active), ready passive\n");
                // 当前机器进入被动(非活跃)状态
                this->curstate_ = STATE_PASSIVE;
            }
        }
        // 当前机器是备机
        else if (this->curstate_ == STATE_BACKUP)
        {
            // 其他机器已经在主动(活跃)状态
            if (this->curevent_ == PEER_ACTIVE)
            {
                printf("I: connected to primary (active), ready passive\n");
                this->curstate_ = STATE_PASSIVE;
            }
            // 其他机器在被动(非活跃)状态
            else if (this->curevent_ == CLIENT_REQUEST)
            {
                // 被动状态的机器 且接受请求 异常错误
                exception = true;
            }
        } 
        // 当前机器处于主动(活跃)状态
        else if (this->curstate_ == STATE_ACTIVE) 
        {
            // 其他机器处于主动活跃状态
            if (this->curevent_ == PEER_ACTIVE) 
            {
                // 双主 属于异常错误
                printf("E: fatal error - dual actives, aborting\n");
                exception = true;
            }
        } 
        // 当前机器处于被动(非活跃)状态
        else if (this->curstate_ == STATE_PASSIVE) 
        {
            // 其他机器处于主机状态
            if (this->curevent_ == PEER_PRIMARY) 
            {
                printf("I: primary (passive) is restarting, ready active\n");
                // 提升当前机器为主动(活跃)状态
                this->curstate_ = STATE_ACTIVE;
            } 
            // 其他机器处于备机状态
            else if (this->curevent_ == PEER_BACKUP) 
            {
                printf("I: backup (passive) is restarting, ready active\n");
                // 提升当前机器为主动(活跃)状态
                this->curstate_ = STATE_ACTIVE;
            } 
            // 其他机器为被动状态
            else if (this->curevent_ == PEER_PASSIVE) 
            {
                printf("E: fatal error - dual passives, aborting\n");
                // 存在两个被动状态的机器 属于异常错误
                exception = true;
            } 
            // 其他节点处于请求状态
            else if (this->curevent_ == CLIENT_REQUEST) 
            {
                assert(this->peer_expiry_ > 0);
                // 其他节点心跳过期
                if (zclock_time() >= this->peer_expiry_) // 其他节点心跳超时
                {
                    printf("I: failover successful, ready active\n");
                    // 提升当前节点为主动(活跃)状态
                    this->curstate_ = STATE_ACTIVE;
                } 
                // 其他节点处于请求状态 且未心跳过期 属于异常错误
                else 
                {
                    exception = true;
                }
            }
        }
        return exception;
    }
//
    void Loop()
    {
        int64_t send_state_at = zclock_time() + HEARTBEAT;
        while (!zctx_interrupted)
        {
            zmq_pollitem_t items[] = 
            {
                { frontend_, 0, ZMQ_POLLIN, 0 },    /* 与客户端的连接 */
                { statesub_, 0, ZMQ_POLLIN, 0 }     /* 主备机连接 */
            };
            int time_left = (int)((send_state_at - zclock_time()));
            if (time_left < 0) time_left = 0;

            int ret = zmq_poll(items, sizeof(items)/sizeof(items[0]), time_left * ZMQ_POLL_MSEC);
            if (ret == -1) break;

            // 与客户端的连接
            if (items[0].revents & ZMQ_POLLIN)
            {
                zmsg_t *msg = zmsg_recv(this->frontend_);
                this->curevent_ = CLIENT_REQUEST;
                printf("current server curstate[%d] curevent[%d] msg:\n", curstate_, curevent_);
                zmsg_print(msg);
                if (this->StateMachine())
                {
                    zmsg_destroy(&msg);
                }
                else 
                {
                    zmsg_send(&msg, this->frontend_);
                }
            }

            // 主备机连接 互相更新心跳
            if (items[1].revents & ZMQ_POLLIN)
            {
                char *message = zstr_recv(this->statesub_); // 收到其他服务器节点的状态 以该状态作为事件类型来记录
                this->curevent_ = (Event_t)atoi(message);
                free(message);
                printf("current server recv other node state : %d\n", this->curevent_);
                if (this->StateMachine())
                {
                    break;
                }
                this->peer_expiry_ = zclock_time() + 2 * HEARTBEAT; // 心跳更新
            }

            if (zclock_time() >= send_state_at)
            {
                char message[sizeof(long long)];
                snprintf(message, sizeof(long long), "%d", this->curstate_);
                zstr_send(this->statepub_, message);    // 发送当前服务器状态给其他服务器节点 作为其他服务器节点的事件类型
                send_state_at = zclock_time() + HEARTBEAT;
            }
        }
        printf ("W: interrupted\n");
    }
};

#endif