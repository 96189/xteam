
#include "prototypeConcrete.h"

// class Spawner
// {
// public:
//     Spawner(IMonster* prototype)
//     : prototype_(prototype)
//     {

//     }

//     IMonster* spawnMonster()
//     {
//         return prototype_->clone();
//     }

// private:
//     IMonster *prototype_;
// };

class ISpawner
{
public:
    virtual ~ISpawner() {}
    virtual IMonster* spawnMonster() = 0;
};

template <typename T>
class SpawnerFor : public ISpawner 
{
public:
    virtual IMonster* spawnMonster()
    {
        return new T();
    }
};

int main(int argc, char* argv[])
{
    // IMonster *ghostPrototype = new Ghost(15, 3);
    // Spawner *pSpawner = new Spawner(ghostPrototype);
    // pSpawner->spawnMonster();

    ISpawner *pSpawner = new SpawnerFor<Ghost>();
    pSpawner->spawnMonster();
    return 0;
}

// http://gameprogrammingpatterns.com/prototype.html
// https://www.cnblogs.com/winter-cn/archive/2009/12/02/1614987.html
// protobuf反射机制中的原型模式
