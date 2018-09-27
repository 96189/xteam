
// 可观察者接口类型(subject)

#ifndef _OBSERV_ABLE_HPP_
#define _OBSERV_ABLE_HPP_

#include <vector>

class Observer;
class Observable 
{
private:
    std::vector<Observer*> observers_;

public:
    // 构造 析构
    Observable();
    virtual ~Observable();
    // 
    void Register(Observer *o);
    void UnRegister(Observer *o);
    void Notify();
};

#endif