
// 观察者模式
// 1->n 1份主题数据 n个观察者 

#include "weatherDisplay.hpp"

int main(int argc, char const *argv[])
{
    // 主题 1
    Weather *pWeather = new Weather();
    
    // 观察者 n
    CurrentConditionsDisplay *pccd = new CurrentConditionsDisplay(pWeather);
    StatisticsDisplay *psd = new StatisticsDisplay(pWeather);
    ForecastDisplay *pfd = new ForecastDisplay(pWeather);

    // 主题更新数据
    pWeather->SetMeasurements(80, 65, 30.4f);
    pWeather->SetMeasurements(82, 70, 29.2f);
    pWeather->SetMeasurements(78, 90, 29.2f);
    return 0;
}
