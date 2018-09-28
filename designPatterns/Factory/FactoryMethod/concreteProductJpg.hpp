
//

#ifndef _CONCRETE_PRODUCT_JPG_HPP_
#define _CONCRETE_PRODUCT_JPG_HPP_

#include "product.hpp"
#include <iostream>

class JpgLoader : public Loader 
{
public:
    JpgLoader()
    {

    }
    ~JpgLoader()
    {

    }
    //
    virtual void LoaderImg()
    {
        std::cout << "loader jpg" << std::endl;
    }
};

#endif