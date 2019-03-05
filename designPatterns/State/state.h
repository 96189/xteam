
#ifndef _STATE_H_
#define _STATE_H_

enum Input
{
    NONE = 0x0,
    PRESS_UP,
    PRESS_DOWN,
    RELEASE_UP,
    RELEASE_DOWN,
};

class Hero;
class HeroState 
{
public:
    virtual ~HeroState() {};
    virtual void handleInput(Hero& hero, Input input) = 0;
};

#endif