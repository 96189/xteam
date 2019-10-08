
#include "product.hpp"
#include "builder.hpp"
#include "director.hpp"
#include "concreteBuilder.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    ActorBuilder *builder = new HeroBuilder();
    ActorController *director = new ActorController();
    Actor *actor = director->Construct(builder);
    std::cout << actor->GetRole() << " | " << actor->GetSex() << " | " << actor->GetFace() << " | " << actor->GetCostume() << " | " << actor->GetHairStyle() << std::endl;
    return 0;
}
