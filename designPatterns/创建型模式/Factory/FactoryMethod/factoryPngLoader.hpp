
//

#ifndef _FACTORY_PNG_LOADER_HPP_
#define _FACTORY_PNG_LOADER_HPP_

#include "factory.hpp"
#include "concreteProductPng.hpp"

class PngLoaderFactory : public LoaderFactory 
{
public:
    PngLoaderFactory()
    {

    }
    ~PngLoaderFactory()
    {

    }
    // 
    virtual Loader* GetLoader()
    {
        return new JpgLoader();
    }
};

#endif