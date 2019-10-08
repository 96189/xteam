
//

#ifndef _WEATHER_DISPLAY_HPP_
#define _WEATHER_DISPLAY_HPP_

#include <iostream>
#include "subobsInterface.hpp"
#include "weather.hpp"

// 具体的观察者 当前天气
class CurrentConditionsDisplay : public IObserver, IDisplayElement
{
private:
    ISubject* weather_;     // 持有主题对象 方便对该主题对象添加和删除当前观察者

    // 记录本观察者需要记录的数据
    float temperature_;
    float humidity_;

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

// 具体的观察者 天气统计
class StatisticsDisplay : public IObserver, IDisplayElement
{
private:
    ISubject *weather_;

    float maxtemperature_;
    float mintemperature_;
    float temperature_;
    int readings_;

public:
// 构造 析构
    StatisticsDisplay(Weather *weather)
        : maxtemperature_(0.0), mintemperature_(200.0), temperature_(0.0), readings_(0)
    {
        weather_ = weather;
        weather_->RegisterObserver(this);
    }
    ~StatisticsDisplay()
    {
        weather_->RemoveObserver(this);
    }
// 
    virtual
    void Update()
    {
        Weather* pData = dynamic_cast<Weather*>(weather_);  // upcast
        ++readings_;
        
        float temperature = pData->getTemperature();
        temperature_ += temperature;
        if (temperature > maxtemperature_)
        {
            maxtemperature_ = temperature;
        }
        if (temperature < mintemperature_)
        {
            mintemperature_ = temperature;
        }
        Display();
    }
    virtual 
    void Display()
    {
        std::cout << "Avg/Max/Min temperature = " << temperature_ / readings_ << "/" << maxtemperature_ << "/" << mintemperature_ << std::endl; 
    }
};

// 具体的观察者 天气预测
class ForecastDisplay : public IObserver, IDisplayElement 
{
private:
    ISubject *weather_;

    float currentPressure_;
    float lastPressure_;

public:
// 构造 析构
    ForecastDisplay(Weather *weather)
        : currentPressure_(29.92f), lastPressure_(0.0)
    {
        weather_ = weather;
        weather_->RegisterObserver(this);
    }
    ~ForecastDisplay()
    {
        weather_->RemoveObserver(this);
    }

// 
    virtual 
    void Update()
    {
        Weather* pData = dynamic_cast<Weather*>(weather_);  // upcast
        lastPressure_ = currentPressure_;
        currentPressure_ = pData->getTemperature();
        
        Display();
    }
    virtual 
    void Display()
    {
        std::cout << "Forecast : ";
        if (currentPressure_ > lastPressure_)
        {
            std::cout << "Improving weather on the way!" << std::endl;
        }
        else if (currentPressure_ == lastPressure_)
        {
            std::cout << "More of the same" << std::endl;
        }
        else  
        {
            std::cout << "Watch out for cooler, rainy weather" << std::endl;
        }
    }
};

#endif