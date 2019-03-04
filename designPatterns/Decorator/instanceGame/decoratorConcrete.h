
#ifndef _DECORATOR_CONCRETE_H_
#define _DECORATOR_CONCRETE_H_

#include "decorator.h"

// 装饰者实例扩展类

// 鞋子
class Shoe : public Equip 
{
public:
    Shoe(IAttribute* attr_)
    : Equip(attr_)
    {
        defense_ = 70;
        power_ = 10;
        describe_ = "Shoe";
    }
public:
    virtual float getDefense()
    {
        return attr_->getDefense() + defense_;
    }
    virtual float getPower()
    {
        return attr_->getPower() + power_;
    }
    virtual std::string getDescribe()
    {
        return attr_->getDescribe() + "," + describe_;
    }
private:
    float defense_;
    float power_;
    std::string describe_;
};

// 弓
class Bow : public Equip
{
public:
    Bow(IAttribute* attr_)
    : Equip(attr_)
    {
        defense_ = 5;
        power_ = 50;
        describe_ = "Bow";
    }
public:
    virtual float getDefense()
    {
        return attr_->getDefense() + defense_;
    }
    virtual float getPower()
    {
        return attr_->getPower() + power_;
    }
    virtual std::string getDescribe()
    {
        return attr_->getDescribe() + "," + describe_;
    }
private:
    float defense_;
    float power_;
    std::string describe_;
};

// 剑
class Sword : public Equip
{
public:
    Sword(IAttribute* attr_)
    : Equip(attr_)
    {
        defense_ = 20;
        power_ = 40;
        describe_ = "Sword";
    }
public:
    virtual float getDefense()
    {
        return attr_->getDefense() + defense_;
    }
    virtual float getPower()
    {
        return attr_->getPower() + power_;
    }
    virtual std::string getDescribe()
    {
        return attr_->getDescribe() + "," + describe_;
    }
private:
    float defense_;
    float power_;
    std::string describe_;
};

// 法杖
class Wand : public Equip
{
public:
    Wand(IAttribute* attr_)
    : Equip(attr_)
    {
        defense_ = 2;
        power_ = 90;
        describe_ = "Wand";
    }
public:
    virtual float getDefense()
    {
        return attr_->getDefense() + defense_;
    }
    virtual float getPower()
    {
        return attr_->getPower() + power_;
    }
    virtual std::string getDescribe()
    {
        return attr_->getDescribe() + "," + describe_;
    }
private:
    float defense_;
    float power_;
    std::string describe_;
};
#endif