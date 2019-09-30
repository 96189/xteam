
#ifndef _FLYWEIGHT_H_
#define _FLYWEIGHT_H_

class Pos;
class Color;

// 享元接口
class ITree
{
public:
    virtual ~ITree() {};

    virtual void setParam(Pos pos, double h, double t, Color b, Color l) = 0;   // 由客户端改变状态的接口
};

#endif