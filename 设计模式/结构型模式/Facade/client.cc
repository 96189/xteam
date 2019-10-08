
#include "facade.h"
#include <string>

class ISceneState
{
public:
    ISceneState(std::string name)
    : sceneName_(name)
    {

    }
    virtual ~ISceneState() {}
public:
    virtual void startScene() = 0;
private:
    std::string sceneName_;
};

class BattleScene : public ISceneState
{
public:
    BattleScene()
    : ISceneState("BattleScene")
    {

    }
public:
    virtual void startScene()
    {
        std::cout << "Init BattleScene" << std::endl;
        Facade *pFacade = new Facade();
        pFacade->Init();
    }
};


int main(int argc, char* argv[])
{
    BattleScene *pScene = new BattleScene();
    pScene->startScene();
    return 0;
}