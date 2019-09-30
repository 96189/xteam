
#ifndef _ADAPTER_H_
#define _ADAPTER_H_

#include "target.h"
#include "adaptee.h"

class Adapter : public Target
{
public:
    Adapter(Adaptee *adaptee)
    : adaptee_(adaptee)
    {
        
    }
    ~Adapter()
    {
        if (adaptee_)
        {
            delete adaptee_;
            adaptee_ = NULL;
        }
    }
public:
    virtual void request()
    {
        if (adaptee_)
        {
            adaptee_->specificRequest();
        }
    }
private:
    Adaptee *adaptee_;
};

#endif