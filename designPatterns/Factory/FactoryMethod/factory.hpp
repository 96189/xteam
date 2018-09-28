
//

#ifndef _FACTORY_HPP_
#define _FACTORY_HPP_

#include "product.hpp"

class LoaderFactory 
{
public:
    LoaderFactory()
    {

    }
    virtual ~LoaderFactory()
    {

    }

    // 接口定义
    virtual Loader* GetLoader() = 0;
};

#endif