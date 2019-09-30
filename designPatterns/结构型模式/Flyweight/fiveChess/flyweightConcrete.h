
#ifndef _FLYWEIGHT_CONCRETE_H_
#define _FLYWEIGHT_CONCRETE_H_

#include "flyweight.h"

class BlackChessman : public AbstractChessman
{
public:
    BlackChessman()
    : AbstractChessman("Black")
    {

    }

    virtual void point(int x, int y)
    {
        x_ = x;
        y_ = y;
        show();
    }
};

class WhiteChessman : public AbstractChessman
{
public:
    WhiteChessman()
    : AbstractChessman("White")
    {

    }

    virtual void point(int x, int y)
    {
        x_ = x;
        y_ = y;
        show();
    }
};


#endif