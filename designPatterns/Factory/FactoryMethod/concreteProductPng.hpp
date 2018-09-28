
//

#ifndef _CONCRETE_PRODUCT_PNG_HPP_
#define _CONCRETE_PRODUCT_PNG_HPP_

#include "product.hpp"
#include <iostream>

class PngLoader : public Loader 
{
public:
    PngLoader()
    {

    }
    ~PngLoader()
    {

    }
    // 
    virtual void LoaderImg()
    {
        std::cout << "loader png" << std::endl;
    }
};

#endif