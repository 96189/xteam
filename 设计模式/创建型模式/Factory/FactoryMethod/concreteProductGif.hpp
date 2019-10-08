
//

#ifndef _CONCRETE_PRODUCT_GIT_HPP_
#define _CONCRETE_PRODUCT_GIT_HPP_

#include "product.hpp"
#include <iostream>

class GifLoader : public Loader 
{
public:
    GifLoader()
    {

    }
    ~GifLoader()
    {

    }
    // 
    virtual void LoaderImg()
    {
        std::cout << "loader gif" << std::endl;
    }
};

#endif