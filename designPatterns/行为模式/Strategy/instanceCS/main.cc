
// 策略模式-CS武器系统测试

#include "strategy.hpp"
#include "context.hpp"
#include "concreteStrategy.hpp"

int main(int argc, char const *argv[])
{
    Weapon *pAK47 = new AK47();
    Weapon *pKnife = new Knife();
    Weapon *pPistol = new Pistol();

    Player *pAda = new Player("ada");
    pAda->SetWeapon(pAK47);
    pAda->PlayerFight();

    Player *pTiti = new Player("titi");
    pTiti->SetWeapon(pKnife);
    pTiti->PlayerFight();

    Player *pJeff = new Player("jeff");
    pJeff->SetWeapon(pPistol);
    pJeff->PlayerFight();
    return 0;
}
