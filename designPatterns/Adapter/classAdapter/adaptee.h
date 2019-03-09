
#ifndef _ADAPTEE_H_
#define _ADAPTEE_H_

#include <iostream>

class Adaptee
{
public:
    virtual ~Adaptee() {};

public:
    virtual void specificRequest()
    {
        std::cout << "Adaptee specificRequest" << std::endl;
    }
};

#endif