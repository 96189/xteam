
// 观察者接口类型

#ifndef _OBSERVER_HPP_
#define _OBSERVER_HPP_

class Observable;
class Observer 
{
protected:
    Observable* sub_;

public:
    // 构造 析构
    Observer(Observable *s);
    virtual ~Observer();

    // 接口定义 数据如何更新由派生类决定
    virtual void Update() = 0;
};

#endif