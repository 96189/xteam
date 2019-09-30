
//

#ifndef _CONCRETE_OBSERVER_HPP_
#define _CONCRETE_OBSERVER_HPP_

#include "observer.hpp"
#include "observable.hpp"
#include "concreteObservable.hpp"
#include <iostream>

class CurrentWeatherBoard : public Observer 
{
private:
    float temperature_;
    float humidity_;

public:
    // 构造 析构
    CurrentWeatherBoard(Observable *s)
        : Observer(s), temperature_(0.0), humidity_(0.0)
    {

    }
    ~CurrentWeatherBoard()
    {

    }
    // 
    virtual void Update()
    {
        Weather* pData = dynamic_cast<Weather*>(sub_);
        temperature_ = pData->GetTemperature();
        humidity_ = pData->GetHumidity();
        
        ShowWeather();
    }
    //
    void ShowWeather()
    {
        std::cout << "Current conditions : " << temperature_ << "F degress and " << humidity_ << "% humidity" << std::endl;
    }
};


class ForecastWeatherBorad : public Observer
{
private:
    float currentPressure_;
    float lastPressure_;

public:
    // 构造 析构
    ForecastWeatherBorad(Observable *s)
        : Observer(s), currentPressure_(29.92f), lastPressure_(0.0)
    {

    }
    ~ForecastWeatherBorad()
    {

    }
    // 
    virtual void Update()
    {
        Weather* pData = dynamic_cast<Weather*>(sub_);
        lastPressure_ = currentPressure_;
        currentPressure_ = pData->GetTemperature();
        ShowWeather();
    }
    void ShowWeather()
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
