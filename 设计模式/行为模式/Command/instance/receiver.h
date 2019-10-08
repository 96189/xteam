
#ifndef _RECEIVER_H_
#define _RECEIVER_H_

class IReceiver
{
public:
    virtual void on() = 0;
    virtual void off() = 0;
};

#endif