
// 具体的策略类 实现策略模式接口

#ifndef _CONCRETE_STRATEGY_HPP_
#define _CONCRETE_STRATEGY_HPP_

#include "strategy.hpp"
#include <iostream>

class Knife : public Weapon 
{
    void Fight(std::string player)
    {
        std::cout << "Player : " << player << " fighting power 3 level " << std::endl;
    }
};

class Pistol : public Weapon 
{
    void Fight(std::string player)
    {
        std::cout << "Player : " << player << " fighting power 5 level " << std::endl;
    }
};

class AK47 : public Weapon 
{
    void Fight(std::string player)
    {
        std::cout << "Player : " << player << " fighting power 8 level " << std::endl;
    }
};

#endif