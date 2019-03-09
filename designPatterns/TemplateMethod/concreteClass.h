
#ifndef _CONCRETE_CLASS_H_
#define _CONCRETE_CLASS_H_

#include "abstractClass.h"

class CurrentAccount : public Account
{
public:
    virtual void calculateInterest()
    {
        std::cout << "x*3.7" << std::endl;
    }
};


class SavingAccount : public Account
{
public:
    virtual void calculateInterest()
    {
        std::cout << "x*4.1" << std::endl;
    }    
};
#endif