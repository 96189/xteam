
//

#ifndef _SUB_OBS_INTERFACE_HPP_
#define _SUB_OBS_INTERFACE_HPP_

class IObserver;
// 主题接口类
class ISubject 
{
public:
    virtual void RegisterObserver(IObserver* o) = 0;
    virtual void RemoveObserver(IObserver* o) = 0;
    virtual void NotifyObservers() = 0;
};

// 观察者接口类
class IObserver 
{
public:
    // virtual void Update(float temp, float humidity, float pressure) = 0; 推模型
    virtual void Update() = 0;
};

class IDisplayElement
{
public:
    virtual void Display() = 0;
};

#endif