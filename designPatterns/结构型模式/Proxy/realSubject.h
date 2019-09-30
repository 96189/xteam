
#ifndef _REAL_SUBJECT_H_
#define _REAL_SUBJECT_H_

#include "subject.h"
#include <iostream>

class RealSubject : public IHttpInvokeSubject 
{
public:
    RealSubject()
    {

    }
    ~RealSubject()
    {

    }
public:
    virtual std::string invoke(const std::string& request)
    {
        std::cout << "handle request" << std::endl;
        return "";
    }
};
#endif