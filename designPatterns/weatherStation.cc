
//
#include "weatherDisplay.hpp"

int main(int argc, char const *argv[])
{
    // 主题
    Weather *pWeather = new Weather();
    
    // 观察者
    CurrentConditionsDisplay *pDisplay = new CurrentConditionsDisplay(pWeather);
    
    // 主题更新数据
    pWeather->SetMeasurements(80, 65, 30.4f);
    return 0;
}
