
// 如何在请求-响应模式中使用套接字标识
// ROUTER根据套接字标识来标记消息来源

#include <zmq.h>
#include <assert.h>
#include <string>

static void s_dump (void *socket)
{
    int rc;

    zmq_msg_t message;
    rc = zmq_msg_init (&message);
    assert (rc == 0);

    puts ("----------------------------------------");
    //  Process all parts of the message
    do {
        int size = zmq_msg_recv (&message, socket, 0);
        assert (size >= 0);

        //  Dump the message as text or binary
        char *data = (char *) zmq_msg_data (&message);
        assert (data != 0);
        int is_text = 1;
        int char_nbr;
        for (char_nbr = 0; char_nbr < size; char_nbr++) {
            if ((unsigned char) data[char_nbr] < 32
                || (unsigned char) data[char_nbr] > 126) {
                is_text = 0;
            }
        }

        printf ("[%03d] ", size);
        for (char_nbr = 0; char_nbr < size; char_nbr++) {
            if (is_text) {
                printf ("%c", data[char_nbr]);
            } else {
                printf ("%02X", (unsigned char) data[char_nbr]);
            }
        }
        printf ("\n");
    } while (zmq_msg_more (&message));

    rc = zmq_msg_close (&message);
    assert (rc == 0);
}

int main(int argc, char *argv[])
{
    void *context = zmq_init(1);

    // ROUTER 监听套接字
    void *sink = zmq_socket(context, ZMQ_ROUTER);
    int ret = zmq_bind(sink, "inproc://example");
    assert(ret != -1);

    // REQ 请求套接字 由ZMQ自动设置标识
    void *anonymous = zmq_socket(context, ZMQ_REQ);
    ret = zmq_connect(anonymous, "inproc://example");
    assert(ret != -1);
    std::string msg_anony = "ROUTER uses a generated UUID";
    int n = zmq_send(anonymous, msg_anony.c_str(), msg_anony.size(), 0);
    assert(n > 0);
    s_dump(sink);   // 打印监听套接字取得的信息

    // REQ 请求套接字 由用户自己设置标识
    void *identified = zmq_socket(context, ZMQ_REQ);
    zmq_setsockopt(identified, ZMQ_IDENTITY, "hello", 5);
    ret = zmq_connect(identified, "inproc://example");
    assert(ret != -1);
    std::string msg_identi = "ROUTER socket uses REQ's socket identity";
    n = zmq_send(identified, msg_identi.c_str(), msg_identi.size(), 0);
    assert(n > 0);
    s_dump(sink);   // 打印监听套接字取得的信息

    zmq_close(sink);
    zmq_close(anonymous);
    zmq_close(identified);
    zmq_term(context);
    return 0;
}