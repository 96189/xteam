
//

#include "duckInstance.hpp"

// 绿头鸭
void TestMallardDuck()
{
    Duck *pDuck = new MallardDuck();
    pDuck->performFly();
    pDuck->performQuack();
    pDuck->display();
    delete pDuck;
    std:: cout << std::endl;
}

// 模型鸭
void TestModelDuck()
{
    Duck *pDuck = new ModelDuck();
    pDuck->performFly();
    pDuck->performQuack();
    pDuck->display();
    std:: cout << std::endl;
    
    // 给模型鸭加上火箭动力
    pDuck->SetFlyBehavior(new FlyRocketPowered());
    pDuck->performFly();
    pDuck->performQuack();
    pDuck->display();    
    delete pDuck;
    std:: cout << std::endl;
}

int main(int argc, char const *argv[])
{
    TestMallardDuck();
    TestModelDuck();
    return 0;
}
