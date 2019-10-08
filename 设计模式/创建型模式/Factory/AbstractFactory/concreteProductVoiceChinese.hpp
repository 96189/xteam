
// 具体产品

#ifndef _CONCRETE_PRODUCT_VOICE_CHINESE_HPP_
#define _CONCRETE_PRODUCT_VOICE_CHINESE_HPP_

#include "productVocie.hpp"
#include <iostream>

class ChineseVoice : public Voice 
{
public:
    ChineseVoice()
    {

    }
    ~ChineseVoice()
    {

    }
    //
    virtual void PlayVoice()
    {
        std::cout << "chinese voice" << std::endl;
    }
};

#endif