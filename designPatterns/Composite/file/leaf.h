
#ifndef _LEAF_H_
#define _LEAF_H_

#include "component.h"
#include <iostream>

class AsciiFile : public AbstractFile
{
public:
    AsciiFile(std::string name)
    {
        setName(name);
    }

public:
    virtual void showFileName()
    {
        std::cout << "ASCII " << getName() << std::endl;
    }
};


class BinaryFile : public AbstractFile
{
public:
    BinaryFile(std::string name)
    {
        setName(name);
    }

public:
    virtual void showFileName()
    {
        std::cout << "BINARY " << getName() << std::endl;
    }
};
#endif