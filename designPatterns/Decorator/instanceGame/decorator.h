
#ifndef _DECORATOR_H_
#define _DECORATOR_H_

#include "component.h"

// 装饰者基类

class Equip : public IAttribute 
{
public:
    Equip(IAttribute* attr)
    : attr_(attr)
    {

    }
    ~Equip()
    {
        if (attr_)
        {
            delete attr_;
        }
    }
public:
    virtual float getDefense()
    {
        return 0;
    }
    virtual float getPower()
    {
        return 0;
    }
    virtual std::string getDescribe()
    {
        return "";
    }
protected:
    IAttribute* attr_;
};

#endif