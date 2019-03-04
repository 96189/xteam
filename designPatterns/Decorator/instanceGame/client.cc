
#include "componentConcrete.h"
#include "decoratorConcrete.h"
#include <iostream>

void ShowInfo(IAttribute *attr)
{
    std::cout << "desc: " << attr->getDescribe() << std::endl;
    std::cout << "defence: " << attr->getDefense() << std::endl;
    std::cout << "power: " << attr->getPower() << std::endl;
}

void decoratorTest()
{
    IAttribute *attrM = new Mage();
    attrM = new Shoe(attrM);
    attrM = new Wand(attrM);
    ShowInfo(attrM);

    IAttribute *attrS = new Sagittary();
    attrS = new Shoe(attrS);
    attrS = new Bow(attrS);
    ShowInfo(attrS);

    IAttribute *attrW = new Warrior();
    attrW = new Shoe(attrW);
    attrW = new Sword(attrW);
    ShowInfo(attrW);
}

int main(int argc, char *argv[])
{
    decoratorTest();
    return 0;
}