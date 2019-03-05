
#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "state.h"
#include <cstddef>
#include <iostream>

enum ACT
{
    IMAGE_STAND = 0x1,
    IMAGE_DUCK,
    IMAGE_JUMP,
    IMAGE_DIVE
};

class Hero
{
public:
    Hero()
    : state_(NULL)
    {

    }
public:
    void setState(HeroState *state)
    {
        if (state_)
        {
            delete state_;
            state_ = NULL;
        }
        state_ = state;
    }
    void handleIput(Input input)
    {
        if (state_) 
            state_->handleInput(*this, input);
    }
    void setGraphics(ACT act)
    {
        if (ACT::IMAGE_STAND == act)
        {
            std::cout << "IMAGE_STAND" << std::endl;
        }
        else if (ACT::IMAGE_DUCK == act)
        {
            std::cout << "IMAGE_DUCK" << std::endl;
        }
        else if (ACT::IMAGE_JUMP == act)
        {
            std::cout << "IMAGE_JUMP" << std::endl;
        }
        else if (ACT::IMAGE_DIVE == act)
        {
            std::cout << "IMAGE_DIVE" << std::endl;
        }
        else 
        {
            std::cout << "NO ACT" << std::endl;
        }
    }
    // void Update()
    // {
    //     state_->update(*this);
    // }
    // void superBomb()
    // {

    // }
private:
    HeroState *state_;
};

#endif