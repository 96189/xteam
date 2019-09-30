
#ifndef _PROTOTYPE_CONCRETE_H_
#define _PROTOTYPE_CONCRETE_H_

#include "prototype.h"

class Ghost : public IMonster
{
public:
    Ghost(int health, int speed)
    : health_(health), speed_(speed)
    {

    }

public:
    virtual IMonster* clone()
    {
        return new Ghost(health_, speed_);
    }

private:
    int health_;
    int speed_;
};

#endif