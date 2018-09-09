
// paranoid pirate worker

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

#define HEARTBEAT_LIVENESS 3    // 心跳健康度
#define HEARTBEAT_INTERVAL 1000 // 单位:ms
#define INTERVAL_INIT 1000      // initial reconnect
#define INITVAL_MAX 3200        // after exponential backoff

#define BACKEND_ADDR "tcp://localhost:5556"

#define PPP_READY "\001"    // signals worker is ready
#define PPP_HEARTBEAT "\002"    // signals worker heartbeat

static void *s_worker_socket(void *context)
{
    void *worker = zmq_socket(context, ZMQ_DEALER);
    int ret = zmq_connect(worker, BACKEND_ADDR);
    assert(ret != -1);

    printf("I: worker ready\n");
    zframe_t *frame = zframe_new(PPP_READY, 1);
    zframe_send(&frame, worker, 0);
    return worker;
}

int main(int argc, char const *argv[])
{
    void *context = zmq_init(1);
    void *worker = s_worker_socket(context);

    size_t liveness = HEARTBEAT_LIVENESS;
    size_t interval = INTERVAL_INIT;

    int64_t heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;

    srandom((unsigned)time(NULL));
    int cycles = 0;
    while (1)
    {
        zmq_pollitem_t items[] = 
        {
            { worker, 0, ZMQ_POLLIN, 0}
        };
        int ret = zmq_poll(items, 1, HEARTBEAT_INTERVAL);
        if (ret == -1) break;

        if (items[0].revents & ZMQ_POLLIN)
        {
            // DEALER套接字需要自己处理msg
            // 3-part envelope delimiter content -> request
            // 1-part HEARTBEAT -> heartbeat
            zmsg_t *msg = zmsg_recv(worker);
            if (!msg) break;

            if (zmsg_size(msg) == 3)
            {
                ++cycles;
                if (cycles > 3 && randof(5) == 0)
                {
                    printf ("I: simulating a crash\n");
                    zmsg_destroy(&msg);
                    break;
                }
                if (cycles > 3 && randof(5) == 0)
                {
                    printf ("I: simulating CPU overload\n");
                    sleep(3);
                }

                printf ("I: normal reply\n");
                zmsg_send(&msg, worker);
                liveness = HEARTBEAT_LIVENESS;  // 重置健康程度
                sleep(1);
            }
            else 
            {
                // queue 心跳消息
                if (zmsg_size(msg) == 1)
                {
                    zframe_t *frame = zmsg_first(msg);
                    if (!memcmp(zframe_data(frame), PPP_HEARTBEAT, 1))
                    {
                        liveness = HEARTBEAT_LIVENESS;  // 重置健康程度
                    }
                    else 
                    {
                        printf ("E: invalid message\n");
                        zmsg_dump(msg);
                    }
                    zmsg_destroy(&msg);
                }
                // 未知消息
                else 
                {
                    printf ("E: invalid message\n");
                    zmsg_dump(msg);
                }
                interval = INTERVAL_INIT;   // 重置等待重连时间
            }
        }
        // 超时
        else 
        {
            // 超时不是立即关闭套接字 建立新套接字重连 而是要依据健康程度判断
            if (--liveness == 0)
            {
                printf("W: heartbeat failure, can't reach queue\n");
                printf("W: reconnecting in %zd msec…\n", interval);
                zclock_sleep(interval);

                if (interval < INITVAL_MAX)
                    interval *= 2;  // 等待重连时间 幂次增长
                zmq_close(worker);
                worker = s_worker_socket(context);
                liveness = HEARTBEAT_LIVENESS;  // 重置健康程度
            }
        }

        //  Send heartbeat to queue if it's time
        if (zclock_time () > heartbeat_at) 
        {
            heartbeat_at = zclock_time () + HEARTBEAT_INTERVAL;
            printf ("I: worker heartbeat\n");
            zframe_t *frame = zframe_new(PPP_HEARTBEAT, 1);
            assert(frame);
            zframe_send(&frame, worker, 0);
        }
    }

    zmq_close(worker);
    zmq_term(context);
    return 0;
}
