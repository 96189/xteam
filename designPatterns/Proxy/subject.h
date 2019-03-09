
#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include <string>

class IHttpInvokeSubject 
{
public:
    virtual ~IHttpInvokeSubject() {};
    virtual std::string invoke(const std::string& request) = 0;
};

#endif