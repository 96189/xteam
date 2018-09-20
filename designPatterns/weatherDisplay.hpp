
//

#ifndef _WEATHER_DISPLAY_HPP_
#define _WEATHER_DISPLAY_HPP_

#include <iostream>
#include "subobsInterface.hpp"
#include "weather.hpp"

// 具体的观察者 当前状况显示对象
class CurrentConditionsDisplay : public IObserver, IDisplayElement
{
private:
    ISubject* weather_;

    // 记录本观察者需要记录的数据
    float temperature_;
    float humidity_;
    float pressure_;

public:
// 构造 析构
    CurrentConditionsDisplay(Weather *weather)
    {
        weather_ = weather;
        weather_->RegisterObserver(this);
    }
    ~CurrentConditionsDisplay()
    {
        weather_->RemoveObserver(this);
    }
//
    // void Update(float temperature, float humidity, float pressure)   // 推模型
    virtual 
    void Update()   // 拉模型(按需获取)
    {   
        Weather* pData = dynamic_cast<Weather*>(weather_);  // upcast
        this->temperature_ = pData->getTemperature();
        this->humidity_ = pData->getHumidity();

        Display();
    }
    virtual 
    void Display()
    {
        std::cout << "Current conditions : " << temperature_ << "F degress and " << humidity_ << "% humidity" << std::endl;
    }
};

#endif