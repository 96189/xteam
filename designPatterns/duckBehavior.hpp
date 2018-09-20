
// 策略模式
// 被委托的对象 封装算法簇 提供被组合的对象

#ifndef _DUCK_BEHAVIOR_HPP_
#define _DUCK_BEHAVIOR_HPP_

#include <iostream>

// 接口类型

// 飞行接口
class IFlyBehavior 
{
public:
    virtual void fly() = 0;
};

// 鸭子叫接口
class IQuackBehavior 
{
public:
    virtual void quack() = 0;
};

// 飞行接口实例化
class FlyWithWings : public IFlyBehavior 
{
public:
    virtual void fly()
    {
        std::cout << "i can fly, use wings" << std::endl;
    }
};

class FlyNoWay : public IFlyBehavior 
{
public:
    virtual void fly()
    {
        std::cout << "i can not fly" << std::endl;
    }
};

class FlyRocketPowered : public IFlyBehavior 
{
public:
    virtual void fly()
    {
        std::cout << "i can fly, use rocket" << std::endl;
    }
};

// 鸭子叫接口实例化
class Quack : public IQuackBehavior 
{
public:
    virtual void quack()
    {
        std::cout << "i can voice, gua gua" << std::endl;
    }
};

class Squeak : public IQuackBehavior 
{
public:
    virtual void quack()
    {
        std::cout << "i can voice, zhi zhi" << std::endl;
    }
};

class MuteQuack : public IQuackBehavior 
{
public:
    virtual void quack()
    {
        std::cout << "i can not voice" << std::endl;
    }
};

#endif