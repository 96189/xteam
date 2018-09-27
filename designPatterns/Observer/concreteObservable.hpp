
//

#ifndef _CONCRETE_OBSERVABLE_HPP_
#define _CONCRETE_OBSERVABLE_HPP_

#include "observable.hpp"

class Weather : public Observable 
{
private:
    float temperature_;
    float humidity_;
    float pressure_;

public: 
    // 构造 析构
    Weather()
        : temperature_(0.0), humidity_(0.0), pressure_(0.0)
    {

    }
    ~Weather()
    {

    }
    // 
    void PerceiveWeather(float temperature, float humidity, float pressure)
    {
        temperature_ = temperature;
        humidity_ = humidity;
        pressure_ = pressure;
        Notify();
    }
    float GetTemperature()
    {
        return temperature_;
    }
    float GetHumidity()
    {
        return humidity_;
    }
    float GetPressure()
    {
        return pressure_;
    }
};

#endif