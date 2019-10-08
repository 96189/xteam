
#ifndef _FLYWEIGHT_H_
#define _FLYWEIGHT_H_

#include <string>
#include <iostream>

class AbstractChessman
{
public:
    AbstractChessman(std::string chess)
    : chess_(chess)
    {
        x_ = y_ = -1;
    }
    virtual ~AbstractChessman()
    {

    }
    void show()
    {
        std::cout << chess_ << "(" << x_ << "," << y_ << ")" << std::endl;
    }

public:
    virtual void point(int x, int y) = 0;   // 由客户端改变状态的接口

protected:
    int x_;                 // (x_,y_)通过接口由客户端改变的外部状态
    int y_;
    std::string chess_;     // 黑白(可以共享的内部状态)
};

#endif