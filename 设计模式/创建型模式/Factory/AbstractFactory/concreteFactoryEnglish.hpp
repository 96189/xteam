
// 具体工厂 - 对应某一个产品族

#ifndef _CONCRETE_FACTORY_ENGLISH_HPP_
#define _CONCRETE_FACTORY_ENGLISH_HPP_

#include "abstractFactory.hpp"
#include "productText.hpp"
#include "productVocie.hpp"
#include "concreteProductTextEnglish.hpp"
#include "concreteProductVoiceEnglish.hpp"

// 英语产品族
class EnglishFactory : public GlobalizationAF
{
public:
    EnglishFactory()
    {

    }
    ~EnglishFactory()
    {

    }
    //
    virtual Text* GetText()
    {
        return new EnglishText();
    }
    virtual Voice* GetVoice()
    {
        return new EnglishVoice();
    }
};

#endif