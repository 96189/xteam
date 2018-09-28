
//

#ifndef _FACTORY_GIF_LOADER_HPP_
#define _FACTORY_GIF_LOADER_HPP_

#include "factory.hpp"
#include "concreteProductGif.hpp"

class GifLoaderFactory : public LoaderFactory 
{
public:
    GifLoaderFactory()
    {

    }
    ~GifLoaderFactory()
    {

    }
    //
    virtual Loader* GetLoader()
    {
        return new GifLoader();
    }
};

#endif