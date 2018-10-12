
// lazy pirate client
// use zmq_poll to do a safe request-reply
// to run, start piserver and then randomly kill/restart it

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

#define BUFF_SIZE 128
#define REQUEST_TIMEOUT 2500 // msecs (>1000)
#define REQUEST_RETRIES 5
#define SERVER_ADDR "tcp://localhost:5555"

static void *s_client_socket(void *context)
{
    void *client = zmq_socket(context, ZMQ_REQ);
    int ret = zmq_connect(client, SERVER_ADDR);
    assert(ret != -1);
    printf("I: connecting to server [0x%x]…\n", (char*)client);

    // configure socket to not wait at close time
    // 不进入TIMEWAIT状态
    int linger = 0;
    zmq_setsockopt(client, ZMQ_LINGER, &linger, sizeof(linger));
    return client;
}

int main(int argc, char const *argv[])
{
    void *context = zmq_init(1);
    void *client = s_client_socket(context);

    int sequence = 0;
    int retries_left = REQUEST_RETRIES;

    while (retries_left)
    {
        char request[BUFF_SIZE] = {'\0'};
        snprintf(request, BUFF_SIZE, "%d", ++sequence);
        zstr_send(client, request);
        sleep(1);

        bool expect_reply = true;
        while (expect_reply)
        {
            zmq_pollitem_t items[] = 
            {
                { client, 0, ZMQ_POLLIN, 0 }
            };

            int ret = zmq_poll(items, 1, REQUEST_TIMEOUT);;
            assert(ret != -1);

            // 服务端应答
            if (items[0].revents & ZMQ_POLLIN)
            {
                char *reply = zstr_recv(client);
                if (!reply) break;
                if (atoi(reply) == sequence)
                {
                    printf("I: server replied OK (%s)\n", reply);
                    retries_left = REQUEST_RETRIES;
                    expect_reply = false;
                }
                else 
                {
                    printf("E: malformed reply from server: %s\n", reply);
                }
                free(reply);
            }
            // 服务端超时未应答
            else 
            {
                // 多次重试 依然没有应答则放弃请求
                if (--retries_left == 0)
                {
                    printf("E: server seems to be offline, abandoning\n");
                    expect_reply = false;
                    break;
                }
                else
                {
                    // 客户端重试 重发请求
                    printf("W: no response from server, retrying…\n");
                    zmq_close(client);
                    client = s_client_socket(context);
                    zstr_send(client, request);
                }
            }
        }
    }

    zmq_close(client);
    zmq_term(context);
    return 0;
}
