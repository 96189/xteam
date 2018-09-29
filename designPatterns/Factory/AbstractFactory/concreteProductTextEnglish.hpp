
// 具体产品

#ifndef _CONCRETE_PRODUCT_TEXT_ENGLISH_HPP_
#define _CONCRETE_PRODUCT_TEXT_ENGLISH_HPP_

#include "productText.hpp"
#include <iostream>

class EnglishText : public Text 
{
public:
    EnglishText()
    {

    }
    ~EnglishText()
    {

    }
    //
    virtual void PlayText()
    {
        std::cout << "english text" << std::endl;
    }
};

#endif