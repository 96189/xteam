
//
#ifndef _AE_API_HPP_
#define _AE_API_HPP_

class AEApi 
{
public:
    virtual int aeApiCreate() = 0;
    virtual void aeApiFree() = 0;
    virtual int aeApiResize(int setsize) = 0;

    virtual int aeApiAddEvent(int fd, int mask) = 0;
    virtual void aeApiDelEvent(int fd, int delmask) = 0;
    
    virtual int aeApiPoll(struct timeval *tvp) = 0;
    
    virtual char *aeApiName(void) = 0;

};

#endif