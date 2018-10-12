
//

#include <czmq.h>
#include <zmq.h>
#include <assert.h>
#include <unistd.h>

#include <vector>
#include <string>

#define REQUEST_TIMEOUT 1000
#define SETTLE_DELAY 2000

class BStarCli 
{
private:
    void *context_;
    void *client_;
    std::vector<std::string> servers_;  // 服务端主机列表(主机/备机)
    int svrIdx_;                        // 当前在用的服务器列表索引

public:
// 构造 析构 
    BStarCli(std::string svraddr1, std::string svraddr2)
        : context_(NULL), client_(NULL), svrIdx_(0)
    {
        servers_.clear();
        servers_.push_back(svraddr1);
        servers_.push_back(svraddr2);

        context_ = zmq_init(1);
    }
    ~BStarCli()
    {
        if (client_) zmq_close(client_);
        if (context_) zmq_term(context_);
    }
// 
    void Connect()
    {
        printf("I: connecting to server at %s…\n", servers_[svrIdx_].c_str());
        if (client_) 
        {
            zmq_close(client_);
            client_ = NULL;
        }
        client_ = zmq_socket(context_, ZMQ_REQ);
        assert(client_);
        int ret = zmq_connect(client_, servers_[svrIdx_].c_str());
        assert(ret != -1);
    }
    void Loop()
    {
        int sequence = 0;
        while (!zctx_interrupted)
        {
            char request[sizeof(long long)];
            memset(request, 0, sizeof(long long));
            snprintf(request, sizeof(long long), "%d", ++sequence);
            zstr_send(client_, request);

            int expect_reply = 1;
            while (expect_reply)
            {
                zmq_pollitem_t items[] = 
                {
                    { client_, 0, ZMQ_POLLIN, 0 }
                };
                int ret = zmq_poll(items, sizeof(items)/sizeof(items[0]), REQUEST_TIMEOUT);
                if (ret == -1) break;

                if (items[0].revents & ZMQ_POLLIN)
                {
                    char *reply = zstr_recv(client_);
                    if (atoi(reply) == sequence)
                    {
                        printf("I: server replied OK (%s)\n", reply);
                        expect_reply = 0;
                        sleep(1);
                    }
                    else 
                    {
                        printf("E: bad reply from server: %s\n", reply);
                    }
                    free(reply);
                }
                else 
                {
                    printf("W: no response from server, failing over\n");
                    
                    // 切换备机
                    svrIdx_ = (svrIdx_ + 1) % servers_.size();
                    zclock_sleep(SETTLE_DELAY);

                    // 连接备机
                    this->Connect();

                    zstr_send(client_, request);
                }
            }
        }
    }
};