
#include "context.h"
#include "stateConcrete.h"
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::vector<Input> inputs = {Input::PRESS_UP, Input::PRESS_DOWN, Input::RELEASE_DOWN, Input::RELEASE_UP};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(inputs.begin(), inputs.end(), std::default_random_engine(seed));

    Hero *pHero = new Hero();
    pHero->setState(new StandingState());
    for (auto it = inputs.begin(); it != inputs.end(); ++it)
    {
        pHero->handleIput(*it);
    }
    pHero->setState(new CrouchState());
    for (auto it = inputs.begin(); it != inputs.end(); ++it)
    {
        pHero->handleIput(*it);
    }
    pHero->setState(new JumpingState());
    for (auto it = inputs.begin(); it != inputs.end(); ++it)
    {
        pHero->handleIput(*it);
    }
    // pHero->setState(new DuckingState());
    // for (auto it = inputs.begin(); it != inputs.end(); ++it)
    // {
    //     pHero->handleIput(*it);
    // }    
    return 0;
}