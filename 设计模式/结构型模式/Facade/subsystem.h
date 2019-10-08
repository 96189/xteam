
#ifndef _SUB_SYSTEM_H_
#define _SUB_SYSTEM_H_

#include <iostream>

// 游戏战斗系统
class BattleSystem
{
public:
    void Init()
    {
        std::cout << "Init BattleSystem" << std::endl;
    }
};

// 游戏背包系统
class PacketSystem
{
public:
    void Init()
    {
        std::cout << "Init PacketSystem" << std::endl;
    }
};

// 游戏金币系统
class GoldSystem
{
public:
    void Init()
    {
        std::cout << "Init GoldSystem" << std::endl;
    }
};
#endif