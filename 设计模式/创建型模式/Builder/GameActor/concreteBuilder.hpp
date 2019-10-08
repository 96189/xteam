
//

#ifndef _CONCRETE_BUILDER_HPP_
#define _CONCRETE_BUILDER_HPP_

#include "builder.hpp"

class HeroBuilder : public ActorBuilder 
{
public:
    virtual void BuildSex() 
    {
        actor_->SetSex("MALE");
    }
    virtual void BuildRole() 
    {
        actor_->SetRole("HERO");
    }
    virtual void BuildFace()
    {
        actor_->SetFace("ROUND");
    }
    virtual void BuildCostume()
    {
        actor_->SetCostume("ARMOR");
    }
    virtual void BuildHairStyle() 
    {
        actor_->SetHairStyle("SHORT");
    }
};

#endif