
// 具体产品

#ifndef _CONCRETE_PRODUCT_TEXT_CHINESE_HPP_
#define _CONCRETE_PRODUCT_TEXT_CHINESE_HPP_

#include "productText.hpp"
#include <iostream>

class ChineseText : public Text 
{
public:
    ChineseText()
    {

    }
    ~ChineseText()
    {

    }
    virtual void PlayText()
    {
        std::cout << "chinese text" << std::endl;
    }
};

#endif