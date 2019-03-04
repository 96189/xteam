
#ifndef _COMPONENT_CONCRETE_H_
#define _COMPONENT_CONCRETE_H_

#include "component.h"

// 被装饰的对象

// 法师
class Mage : public IAttribute 
{
public:
    Mage()
    : defense_(5), power_(40), describe_("Maga")
    {

    }
    ~Mage()
    {

    }
public:
    virtual float getDefense()
    {
        return defense_;
    }
    virtual float getPower()
    {
        return power_;
    }
    virtual std::string getDescribe()
    {
        return describe_;
    }
private:
    float defense_;
    float power_;
    std::string describe_;
};

// 射手
class Sagittary : public IAttribute
{
public:
    Sagittary()
    : defense_(10), power_(30), describe_("Sagittary")
    {

    }
    ~Sagittary()
    {

    }
public:
    virtual float getDefense()
    {
        return defense_;
    }
    virtual float getPower()
    {
        return power_;
    }
    virtual std::string getDescribe()
    {
        return describe_;
    }
private:
    float defense_;
    float power_;
    std::string describe_;    
};

// 战士
class Warrior : public IAttribute
{
public:
    Warrior()
    : defense_(20), power_(20), describe_("Warrior")
    {

    }
    ~Warrior()
    {

    }
public:
    virtual float getDefense()
    {
        return defense_;
    }
    virtual float getPower()
    {
        return power_;
    }
    virtual std::string getDescribe()
    {
        return describe_;
    }
private:
    float defense_;
    float power_;
    std::string describe_;
};


#endif