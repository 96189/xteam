
//

#ifndef _WEATHER_HPP_
#define _WEATHER_HPP_

#include <vector>
#include <algorithm> 
#include "subobsInterface.hpp"

// 具体的主题 天气对象
class Weather : public ISubject
{
private:
    std::vector<IObserver*> observers_;

    // 一份天气数据
    float temperature_;
    float humidity_;
    float pressure_;

    bool changed_;

public:
// 构造 析构
    Weather()
        : temperature_(0.0), humidity_(0.0), pressure_(0.0), changed_(false)
    {
        observers_.clear();
    }   
    ~Weather()
    {
        observers_.clear();
    }
// 主题接口实现
    virtual 
    void RegisterObserver(IObserver* o)
    {
        observers_.push_back(o);
    }
    virtual 
    void RemoveObserver(IObserver* o)
    {
        std::vector<IObserver*>::iterator it = std::find(observers_.begin(), observers_.end(), o);
        if (it != observers_.end())
        {
            observers_.erase(it);
        }
    }
    virtual 
    void NotifyObservers()
    {
        if (changed_)
        {
            std::vector<IObserver*>::iterator it = observers_.begin();
            for ( ; it != observers_.end(); ++it)
            {
                // (*it)->Update(temperature_, humidity_, pressure_);
                (*it)->Update();
            }
        }
    }
// 
    void SetChanged(bool open)
    {
        changed_ = open;
    }
    void MeasurementsChanged()
    {
        SetChanged(true);
        NotifyObservers();
    }

    void SetMeasurements(float temperature, float humidity, float pressure)
    {
        temperature_ = temperature;
        humidity_ = humidity;
        pressure_ = pressure;
        MeasurementsChanged();
    }
// 为当前类的派生类提供拉数据接口
    float getTemperature()
    {
        return temperature_;
    }
    float getHumidity()
    {
        return humidity_;
    }
    float getPressure()
    {
        return pressure_;
    }
};

#endif