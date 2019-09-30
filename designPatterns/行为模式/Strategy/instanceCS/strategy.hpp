
// 策略模式 接口类

#ifndef _STRATEGY_HPP_
#define _STRATEGY_HPP_

#include <string>

class Weapon 
{
public:
    virtual void Fight(std::string player) = 0;
};

#endif