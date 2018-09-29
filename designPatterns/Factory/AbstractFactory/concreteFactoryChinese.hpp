
// 具体工厂 - 对应某一个产品族

#ifndef _CONCRETE_FACTORY_CHINESE_HPP_
#define _CONCRETE_FACTORY_CHINESE_HPP_

#include "abstractFactory.hpp"
#include "productText.hpp"
#include "productVocie.hpp"
#include "concreteProductTextChinese.hpp"
#include "concreteProductVoiceChinese.hpp"

// 汉语产品族
class ChineseFactory : public GlobalizationAF 
{
public:
    ChineseFactory()
    {

    }
    ~ChineseFactory()
    {

    }
    // 
    virtual Text* GetText()
    {
        return new ChineseText();
    }
    virtual Voice* GetVoice() 
    {
        return new ChineseVoice();
    }
};

#endif