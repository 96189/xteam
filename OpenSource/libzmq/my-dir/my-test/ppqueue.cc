
// paranoid pirate queue

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

#define HEARTBEAT_LIVENESS 3    // 心跳健康度
#define HEARTBEAT_INTERVAL 1000 // 心跳间隔时间 1ms

#define PPP_READY "\001"        // signals worker is ready
#define PPP_HEARTBEAT "\002"    // signals worker heartbeat

#define FRONTEND_ADDR "tcp://*:5555"
#define BACKEND_ADDR "tcp://*:5556"

class Worker 
{
public:
    zframe_t *identity_;
    char *id_string_;
    int64_t expiry_;

public:
// 构造 析构
    Worker(zframe_t *identity)
    {
        identity_ = identity;
        id_string_ = zframe_strhex(identity);
        expiry_ = zclock_time() + HEARTBEAT_LIVENESS * HEARTBEAT_INTERVAL;
    }
    ~Worker()
    {
        zframe_destroy(&identity_);
        free(id_string_);
    }
};

// the ready method puts a worker to the end of the ready list
static void worker_ready(Worker *self, zlist_t *lruqueue)
{
    Worker *worker = (Worker *) zlist_first (lruqueue);
    while (worker) {
        if (streq (self->id_string_, worker->id_string_)) {
            zlist_remove (lruqueue, worker);
            delete worker;
            break;
        }
        worker = (Worker *) zlist_next (lruqueue);
    }
    zlist_append (lruqueue, self);
}
// the next method returns the next available worker identity
static zframe_t *workers_next (zlist_t *lruqueue)
{
    Worker *worker = (Worker *)zlist_pop(lruqueue);
    assert (worker);
    zframe_t *frame = worker->identity_;
    assert (frame);
    worker->identity_ = NULL;
    delete worker;
    return frame;
}
// the purge method looks for kills expired workers.
// we hold workers from oldest to most recent, so we stop at the first alive worker
static void workers_purge (zlist_t *lruqueue)
{
    Worker *worker = (Worker *) zlist_first (lruqueue);
    while (worker) {
        // 未过期
        if (zclock_time () < worker->expiry_)
            break;

        // 已过期
        zlist_remove (lruqueue, worker);
        delete worker;
        worker = (Worker *) zlist_first (lruqueue);
    }
}


int main(int argc, char const *argv[])
{
    void *context = zmq_init(1);

    void *frontend = zmq_socket(context, ZMQ_ROUTER);
    void *backend = zmq_socket(context, ZMQ_ROUTER);
    int ret = zmq_bind(frontend, FRONTEND_ADDR);
    assert(ret != -1);
    ret = zmq_bind(backend, BACKEND_ADDR);
    assert(ret != -1);

    zlist_t *lruqueue = zlist_new();    // 当前lru队列为 后端尚且存活的worker对象队列 worker对象包含identity和expiry

    int64_t heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;

    while (1)
    {
        zmq_pollitem_t items[] = 
        {
            { backend, 0, ZMQ_POLLIN, 0 },
            { frontend, 0, ZMQ_POLLIN, 0 }
        };

        ret = zmq_poll(items, zlist_size(lruqueue) ? 2 : 1, HEARTBEAT_INTERVAL);
        if (ret == -1) break;

        // 后端worker消息
        if (items[0].revents & ZMQ_POLLIN)
        {
            zmsg_t *msg = zmsg_recv(backend);
            if (!msg) break;
            zframe_t *identity = zmsg_unwrap(msg);
            Worker *worker = new Worker(identity);  // 新建一个worker对象(新的identity和expiry)
            worker_ready(worker, lruqueue);         // 删除队列中存储的重复的identity用新的worker对象替换旧的对象

            // validate control message
            if (zmsg_size(msg) == 1)
            {
                zframe_t *frame = zmsg_first(msg);
                // 非worker READY和心跳消息
                if (memcmp(zframe_data(frame), PPP_READY, 1) &&
                    memcmp(zframe_data(frame), PPP_HEARTBEAT, 1))
                {
                    printf("E: invalid message from worker\n");
                    zmsg_dump(msg);
                }
                zmsg_destroy(&msg);
            }
            // reply to client
            else 
            {
                zmsg_send(&msg, frontend);
            }
        }

        // 前端client消息
        if (items[1].revents & ZMQ_POLLIN)
        {
            zmsg_t *msg = zmsg_recv(frontend);
            if (!msg) break;
            zframe_t *identity = workers_next(lruqueue);    // 从队列中删除已经指派任务的worker对象(identity和expiry随对象被清除)
            assert(identity);
            zmsg_prepend(msg, &identity);
            zmsg_send(&msg, backend);
        }

        // we handle heratbeating after any socket activity.
        // first, we send heartbeats to any idle workers if it is time
        // then, we purge any dead workers
        if (zclock_time() >= heartbeat_at)
        {
            Worker *worker = (Worker*)zlist_first(lruqueue);
            while (worker)
            {
                zframe_send(&worker->identity_, backend, ZFRAME_REUSE + ZFRAME_MORE);

                zframe_t *frame = zframe_new(PPP_HEARTBEAT, 1);
                assert(frame);
                zframe_send(&frame, backend, 0);
                worker = (Worker*)zlist_next(lruqueue);
            }
            heartbeat_at = zclock_time() + HEARTBEAT_INTERVAL;
        }
        // 清除没有心跳的worker 内部检查lruqueue中的worker对象是否过期
        workers_purge(lruqueue);
    }

    while (zlist_size(lruqueue))
    {
        Worker* worker = (Worker*)zlist_pop(lruqueue);
        assert(worker);
        delete worker;
    }

    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;
}

