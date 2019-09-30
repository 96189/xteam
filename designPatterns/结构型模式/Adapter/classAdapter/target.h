
#ifndef _TARGET_H_
#define _TARGET_H_

#include <iostream>

class Target
{
public:
    virtual ~Target() {};

public:
    virtual void request()
    {
        std::cout << "Target request" << std::endl;
    }
};

#endif