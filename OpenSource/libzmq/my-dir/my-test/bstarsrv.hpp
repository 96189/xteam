
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

typedef enum 
{
    STATE_NONE = 0,
    STATE_PRIMARY = 1,
    STATE_BACKUP = 2,
    STATE_ACTIVE = 3,
    STATE_PASSIVE = 4
}State_t;

typedef enum 
{
    PEER_NONE = 0,
    PEER_PRIMARY = 1,
    PEER_BACKUP = 2,
    PEER_ACTIVE = 3,
    PEER_PASSIVE = 4,
    CLIENT_REQUEST = 5
}Event_t;

class BStarSrv 
{
private:
    void *context_;
    void *statepub_;
    void *statesub_;
    void *frontend_;

    State_t curstate_;
    Event_t curevent_;
    int64_t peer_expiry_;

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

// 初始化
    void Init(const char *mode)
    {
        context_ = zmq_init(1);
        statepub_ = zmq_socket(context_, ZMQ_PUB);
        statesub_ = zmq_socket(context_, ZMQ_SUB);
        frontend_ = zmq_socket(context_, ZMQ_ROUTER);
        if (streq(mode, "-p"))
        {
            printf ("I: Primary active, waiting for backup (passive)\n");
            curstate_ = STATE_PRIMARY;
            zmq_bind(context_, PRIMARY_FRONTEND_ADDR);

            zmq_bind(context_, PRIMARY_PUB_ADDR);
            zmq_connect(context_, PRIMARY_SUB_ADDR);
        }
        else if (streq(mode, "-b"))
        {
            printf ("I: Backup passive, waiting for primary (active)\n");
            curstate_ = STATE_BACKUP;
            zmq_bind(context_, BACKUP_FRONTEND_ADDR);

            zmq_bind(context_, BACKUP_PUB_ADDR);
            zmq_connect(context_, BACKUP_SUB_ADDR);
        }
        else 
        {
            assert(false);
        }
    }
    void Destroy()
    {
        zmq_close(statepub_);
        zmq_close(statesub_);
        zmq_close(frontend_);
        zmq_term(context_);
    }
// 
    bool StateMachine()
    {
        bool exception = false;

        if (this->curstate_ == STATE_PRIMARY)
        {
            if (this->curevent_ == PEER_BACKUP)
            {
                printf ("I: connected to backup (passive), ready active\n");
                this->curstate_ = STATE_ACTIVE;
            }
            else if (this->curevent_ == PEER_ACTIVE)
            {
                printf ("I: connected to backup (active), ready passive\n");
                this->curstate_ = STATE_PASSIVE;
            }
        }
        else if (this->curstate_ == STATE_BACKUP)
        {
            if (this->curevent_ == PEER_ACTIVE)
            {
                this->curstate_ = STATE_PASSIVE;
            }
            else if (this->curevent_ == CLIENT_REQUEST)
            {
                exception = true;
            }
        }
        else 
        {
            if (this->curstate_ == STATE_ACTIVE)
            {
                if (this->curevent_ == PEER_ACTIVE)
                {
                    printf ("E: fatal error - dual actives, aborting\n");
                    exception = true;
                }
            }
            else 
            {
                if (this->curstate_ == STATE_PASSIVE)
                {
                    if (this->curevent_ == PEER_PRIMARY)
                    {
                        printf ("I: primary (passive) is restarting, ready active\n");
                        this->curstate_ = STATE_ACTIVE;
                    }
                    else if (this->curevent_ == PEER_BACKUP)
                    {
                        printf ("I: backup (passive) is restarting, ready active\n");
                        this->curstate_ = STATE_ACTIVE;
                    }
                    else if (this->curevent_ == PEER_PASSIVE)
                    {
                        printf ("E: fatal error - dual passives, aborting\n");
                        exception = true;
                    }
                    else if (this->curevent_ == CLIENT_REQUEST)
                    {
                        assert(this->peer_expiry_ > 0);
                        if (zclock_time () >= this->peer_expiry_)
                        {
                            printf ("I: failover successful, ready active\n");
                            this->curstate_ = STATE_ACTIVE;
                        }
                        else 
                        {
                            exception = true;
                        }
                    }
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
                { frontend_, 0, ZMQ_POLLIN, 0 },
                { statesub_, 0, ZMQ_POLLIN, 0 }
            };
            int time_left = (int)((send_state_at - zclock_time()));
            if (time_left < 0) time_left = 0;

            int ret = zmq_poll(items, sizeof(items)/sizeof(items[0]), time_left * ZMQ_POLL_MSEC);
            if (ret == -1) break;

            if (items[0].revents & ZMQ_POLLIN)
            {
                zmsg_t *msg = zmsg_recv(this->frontend_);
                this->curevent_ = CLIENT_REQUEST;
                if (this->StateMachine())
                {
                    zmsg_destroy(&msg);
                }
                else 
                {
                    zmsg_send(&msg, this->frontend_);
                }
            }

            if (items[1].revents & ZMQ_POLLIN)
            {
                char *message = zstr_recv(this->statesub_);
                this->curevent_ = (Event_t)atoi(message);
                free(message);
                if (this->StateMachine())
                {
                    break;
                }
                this->peer_expiry_ = zclock_time() + 2 * HEARTBEAT;
            }

            if (zclock_time() >= send_state_at)
            {
                char message[1];
                snprintf(message, 1, "%d", this->curstate_);
                zstr_send(this->statepub_, message);
                send_state_at = zclock_time() + HEARTBEAT;
            }
        }
        printf ("W: interrupted\n");
    }
};

#endif