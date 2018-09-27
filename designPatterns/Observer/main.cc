
#include "concreteObserver.hpp"
#include "concreteObservable.hpp"

int main(int argc, char const *argv[])
{
    Weather *pWeather = new Weather();

    CurrentWeatherBoard *pcwb = new CurrentWeatherBoard(pWeather);
    ForecastWeatherBorad *pfwb = new ForecastWeatherBorad(pWeather);

    pWeather->PerceiveWeather(80, 65, 30.4f);
    pWeather->PerceiveWeather(82, 70, 29.2f);
    pWeather->PerceiveWeather(78, 90, 29.2f);
    return 0;
}
