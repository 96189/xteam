
// 环境类 使用算法的角色(针对接口编程)

#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include "strategy.hpp"

class Player 
{
private:
    std::string player_;
    Weapon *pWeapon_;

public:
    // 构造 析构
    Player(std::string name)
        : player_(name), pWeapon_(NULL)
    {

    }
    ~Player()
    {

    }
    // 武器设置
    void SetWeapon(Weapon* weapon)
    {
        pWeapon_ = weapon;
    }

    void PlayerFight()
    {
        pWeapon_->Fight(player_);
    }
};

#endif