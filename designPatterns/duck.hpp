
//

#ifndef _DUCK_HPP_
#define _DUCK_HPP_

#include "duckBehavior.hpp"

// 鸭子基类 委托人(delegate)
class Duck 
{
private:
    // 动态行为委托给接口类型 通过任意组合来表达不同的行为
    IQuackBehavior *pquackBehavior_;
    IFlyBehavior *pflyBehavior_;
    
public:
// 构造 析构
    Duck(IQuackBehavior* quackBehavior, IFlyBehavior *pflyBehavior)
        : pquackBehavior_(quackBehavior), pflyBehavior_(pflyBehavior)
    {

    }
    virtual ~Duck()
    {

    }

// 提供接口使子类动态表达行为
// 动态行为设定
    void SetQuackBehavior(IQuackBehavior *qb)
    {
        pquackBehavior_ = qb;
    }
    void SetFlyBehavior(IFlyBehavior *fb)
    {
        pflyBehavior_ = fb;
    }
// 动态行为调用 委托调用
    void performQuack()
    {
        pquackBehavior_->quack();
    }
    void performFly()
    {
        pflyBehavior_->fly();
    }

// 非易变化行为由子类各自实现 继承即可
    virtual void swim()
    {

    }
    virtual void display()
    {

    }
};

#endif