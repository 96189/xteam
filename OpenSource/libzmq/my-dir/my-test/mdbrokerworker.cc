#include "mdbrokerworker.hpp"
#include "mdbroker.hpp"
#include "mdbrokerservice.hpp"

void Worker::Delete (int disconnect)
{
    if (disconnect) {
        this->Send (MDPW_DISCONNECT, NULL, NULL);
    }

    if (this->service_) {
        zlist_remove (this->service_->waiting_, this);
        this->service_->workers_--;
    }
    zlist_remove (this->broker_->waiting_, this);
    zhash_delete (this->broker_->workers_, this->id_string_);
}
void Worker::Send (char *command, char *option, zmsg_t *msg)
{
    msg = msg ? zmsg_dup (msg) : zmsg_new ();
    if (option) {
        zmsg_pushstr (msg, option);
    }
    zmsg_pushstr (msg, command);
    zmsg_pushstr (msg, MDPW_WORKER);
 
    zmsg_wrap (msg, zframe_dup (this->identity_));

    if (this->broker_->verbose_) {
        zclock_log ("I: sending %s to worker 0x%X", mdps_commands[(int) *command], zframe_strhex(this->identity_));
        zmsg_dump (msg);
    }
    zmsg_send (&msg, this->broker_->socket_);
}
void Worker::Waiting ()
{
    assert (this->broker_);
    zlist_append (this->broker_->waiting_, this);
    zlist_append (this->service_->waiting_, this);
    this->expiry_ = zclock_time () + HEARTBEAT_EXPIRY;
    // broker_->DispatchMsg (this->service_, NULL);
    this->service_->ProcessMsg(NULL);
}