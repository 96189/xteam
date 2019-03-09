
#ifndef _ADAPTER_H_
#define _ADAPTER_H_

#include "target.h"
#include "adaptee.h"

class Adapter : public Target, public Adaptee
{
public:

public:
    virtual void request()
    {
        specificRequest();
    }
};

#endif