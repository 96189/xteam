
#ifndef _PROTOTYPE_H_
#define _PROTOTYPE_H_

class IMonster 
{
public:
    virtual ~IMonster() {};
    virtual IMonster* clone() = 0;
};

#endif