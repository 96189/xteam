
// 抽象工厂

#ifndef _ABSTRACT_FACTORY_HPP_
#define _ABSTRACT_FACTORY_HPP_

#include "productText.hpp"
#include "productVocie.hpp"

// 国际化抽象工程
// 把拥有公共主题的工厂组合起来(Chinese工厂和English工厂)
// 抽象工厂确定工厂的业务范围(Text和Voice)
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