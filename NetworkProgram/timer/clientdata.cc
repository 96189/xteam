#include "TimerManager.h"

client_data::client_data()
{
    timer_ = NULL;
}
client_data::~client_data()
{
    if (timer_)
    {
        delete timer_;
        timer_ = NULL;
    }
}
client_data::client_data(int fd, sockaddr_in address)
    : sockfd_(fd), addr(address)
{
}
void client_data::SetTimer(TimerObj *pobj)
{
    timer_ = pobj;
}
