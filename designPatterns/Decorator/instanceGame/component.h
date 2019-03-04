
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>

class IAttribute
{
public:
    virtual ~IAttribute() {};
public:
    virtual float getDefense() = 0;
    virtual float getPower() = 0;
    virtual std::string getDescribe() = 0;
};

#endif