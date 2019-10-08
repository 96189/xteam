
// http://www.cnblogs.com/edisonchou/p/6828436.html
// http://www.hollischuang.com/archives/1477

#ifndef _BUILDER_HPP_
#define _BUILDER_HPP_

#include "product.hpp"

class ActorBuilder 
{
public:
    Actor *actor_;

    ActorBuilder()
    {
        actor_ = new Actor();
    }
    virtual ~ActorBuilder()
    {   
        delete actor_;
    }
    // 接口定义
    virtual void BuildSex() = 0;
    virtual void BuildRole() = 0;
    virtual void BuildFace() = 0;
    virtual void BuildCostume() = 0;
    virtual void BuildHairStyle() = 0;

    // 工厂方法
    Actor* CreateActor()
    {
        return actor_;
    }
};

#endif