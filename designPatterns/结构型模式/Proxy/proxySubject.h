
#ifndef _PROXY_SUBJECT_H_
#define _PROXY_SUBJECT_H_

#include "subject.h"
#include <iostream>

class ProxySubject : public IHttpInvokeSubject
{
public:
    ProxySubject(IHttpInvokeSubject* subject)
    : subject_(subject)
    {

    }
    ~ProxySubject()
    {
        if (subject_)
        {
            delete subject_;
            subject_ = NULL;
        }
    }
public:
    virtual std::string invoke(const std::string& request)
    {
        std::string response;
        if (subject_)
        {
            decodeRequest(request);
            response = subject_->invoke(request);
            encodeResponse(response);
        }
        return response;
    }
    void decodeRequest(const std::string& request)
    {
        std::cout << "decode request" << std::endl;
    }
    void encodeResponse(const std::string& response)
    {
        std::cout << "encode response" << std::endl;
    }
private:
    IHttpInvokeSubject *subject_;
};


#endif