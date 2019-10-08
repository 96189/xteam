
#ifndef _STATE_CONCRETE_H_
#define _STATE_CONCRETE_H_

#include "state.h"
#include "context.h"


// 站立状态
class StandingState : public HeroState
{
public:
    virtual void handleInput(Hero& hero, Input input)
    {
        if (input == Input::PRESS_UP)
        {
            hero.setGraphics(ACT::IMAGE_JUMP);
        }
        if (input == Input::PRESS_DOWN)
        {
            hero.setGraphics(ACT::IMAGE_DUCK);
        }
        // 本状态下 其他操作不处理
    }
};

// 下蹲状态
class CrouchState : public HeroState 
{
public:
    virtual void handleInput(Hero& hero, Input input)
    {
        if (input == Input::RELEASE_DOWN)
        {
            hero.setGraphics(ACT::IMAGE_STAND);
        }
        // 本状态下 其他操作不处理
    }
};

// 跳跃状态
class JumpingState : public HeroState 
{
public:
    virtual void handleInput(Hero& hero, Input input)
    {
        if (input == Input::RELEASE_DOWN)
        {
            hero.setGraphics(ACT::IMAGE_DIVE);
        }
        // 本状态下 其他操作不处理
    }
};

// 闪避状态
class DuckingState : public HeroState 
{
public:
    DuckingState()
    {

    }
public:
    virtual void handleInput(Hero& hero, Input input) 
    {
        // 本状态下 其他操作不处理
    }
};

// 俯冲状态
class DivingState :public HeroState 
{
public:

};
#endif