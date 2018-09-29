
//

#ifndef _FACTORY_HPP_
#define _FACTORY_HPP_

#include "productText.hpp"
#include "productVocie.hpp"

class GlobalizationAF 
{
public:
    GlobalizationAF()
    {

    }
    virtual ~GlobalizationAF()
    {

    }
    // 
    virtual Text* GetText() = 0;
    virtual Voice* GetVoice() = 0;
};

#endif