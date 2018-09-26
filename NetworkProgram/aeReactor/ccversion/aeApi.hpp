
//
#ifndef _AE_API_HPP_
#define _AE_API_HPP_

class aeEventLoop;
class AEApi 
{
public:
    virtual int aeApiCreate(aeEventLoop *eventLoop) = 0;
    virtual void aeApiFree(aeEventLoop *eventLoop) = 0;
    virtual int aeApiResize(aeEventLoop *eventLoop, int setsize) = 0;

    virtual int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask) = 0;
    virtual void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int delmask) = 0;
    
    virtual int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp) = 0;
    
    virtual char *aeApiName(void) = 0;

};

#endif