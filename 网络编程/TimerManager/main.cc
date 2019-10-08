
//

#include "timerManager.hpp"
#include <iostream>
#include <unistd.h>
#include <assert.h>

void TimeOut(int timerId)
{
    switch(timerId)
    {
        case 1:
            std::cout << timerId << " time out " << std::endl;
            break;
        case 2:
            std::cout << timerId << " time out " << std::endl;
            break;
        case 3:
            std::cout << timerId << " time out " << std::endl;
            break;
        case 4:
            std::cout << timerId << " time out " << std::endl;
            break;
        case 5:
            std::cout << timerId << " time out " << std::endl;
            break;
        default:
            break;
    }
}

int main(int argc, char const *argv[])
{
    TimerManager *pManager = new TimerManager();
    pManager->RegisterTimerOutCallback(TimeOut);

    // int id = 1;
    // int sec = 2;
    for (int i = 1; i <= 5; ++i)
    {
        pManager->StartTimer(i, i+1);
    }
    sleep(5);
    pManager->CheckExpired();
    return 0;
}
