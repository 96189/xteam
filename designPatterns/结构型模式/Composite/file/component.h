
#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>

class AbstractFile 
{
public:
    virtual ~AbstractFile() {}
    void setName(std::string name)
    {
        name_ = name;
    }
    std::string getName()
    {
        return name_;
    }

public:
    virtual void showFileName() = 0;

protected:
    std::string name_;
};

#endif