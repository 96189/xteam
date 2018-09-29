
// 具体产品

#ifndef _CONCRETE_PRODUCT_VOICE_ENGLISH_HPP_
#define _CONCRETE_PRODUCT_VOICE_ENGLISH_HPP_

#include "productVocie.hpp"
#include <iostream>

class EnglishVoice : public Voice 
{
public:
    EnglishVoice()
    {

    }
    ~EnglishVoice()
    {

    }
    //
    virtual void PlayVoice()
    {
        std::cout << "english voice" << std::endl;
    }
};

#endif