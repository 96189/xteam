
//

#ifndef _DUCK_INSTANCE_HPP_
#define _DUCK_INSTANCE_HPP_

#include "duck.hpp"

// 鸭子实例类
// 绿头鸭
class MallardDuck : public Duck 
{
public:
// 构造 析构 行为组合
    MallardDuck()
        : Duck(new Quack(), new FlyWithWings())
    {

    }
    ~MallardDuck()
    {

    }

    virtual
    void display()
    {
        std::cout << "I am a real Mallard duck" << std::endl;
    }
};

// 模型鸭
class ModelDuck : public Duck 
{
public:
// 构造 析构 行为组合
    ModelDuck()
        : Duck(new Quack(), new FlyNoWay())
    {

    }
    ~ModelDuck()
    {

    }

    virtual 
    void display()
    {
        std::cout << "I am a model duck" << std::endl;
    }
};

#endif