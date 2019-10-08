
//

#ifndef _DIRECTOR_HPP_
#define _DIRECTOR_HPP_

#include "product.hpp"
#include "builder.hpp"

class ActorController 
{
public:
    ActorController()
    {

    }
    ~ActorController()
    {
        
    }
public:
    Actor* Construct(ActorBuilder *builder)
    {
        builder->BuildRole();
        builder->BuildSex();
        builder->BuildFace();
        builder->BuildCostume();
        builder->BuildHairStyle();

        return builder->CreateActor();
    }
};

#endif