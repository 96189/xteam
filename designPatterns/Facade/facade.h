
#ifndef _FACADE_H_
#define _FACADE_H_

#include "subsystem.h"

class Facade
{
public:
    void Init()
    {
        BattleSystem *pBattle = new BattleSystem();
        pBattle->Init();
        PacketSystem *pPacket = new PacketSystem();
        pPacket->Init();
        GoldSystem *pGold = new GoldSystem();
        pGold->Init();
    }
};

#endif