#include "mdbrokerworker.hpp"
#include "mdbroker.hpp"
#include "mdbrokerservice.hpp"

void Worker::Delete(int disconnect)
{
    if (disconnect) {
        this->Send(MDPW_DISCONNECT, NULL, NULL);
    }

    if (this->service_) {
        this->service_->PopFrontWorker();
    }

    this->broker_->RemoveWaitWorker(this);
    this->broker_->RemoveWorker(this->id_string_);
}
void Worker::Send(char *command, char *option, zmsg_t *msg)
{
    msg = msg ? zmsg_dup (msg) : zmsg_new();
    if (option) {
        zmsg_pushstr(msg, option);
    }
    zmsg_pushstr(msg, command);
    zmsg_pushstr(msg, MDPW_WORKER);
 
    zmsg_wrap(msg, zframe_dup (this->identity_));

    if (this->broker_->verbose_) {
        zclock_log("I: sending %s to worker 0x%X", mdps_commands[(int) *command], zframe_strhex(this->identity_));
        zmsg_dump (msg);
    }
    zmsg_send(&msg, this->broker_->socket_);
}
void Worker::Waiting()
{
    assert(this->broker_);
    this->broker_->AddWaitWorker(this);
    this->service_->PushBackWorker(this);
    this->expiry_ = zclock_time() + HEARTBEAT_EXPIRY;
    this->service_->ProcessMsg(NULL);
}