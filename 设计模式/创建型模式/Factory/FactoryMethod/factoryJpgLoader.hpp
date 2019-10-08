
//

#ifndef _FACTORY_JPG_LOADER_HPP_
#define _FACTORY_JPG_LOADER_HPP_

#include "factory.hpp"
#include "concreteProductJpg.hpp"

class JpgLoaderFactory : public LoaderFactory 
{
public:
    JpgLoaderFactory()
    {

    }
    ~JpgLoaderFactory()
    {

    }
    //
    virtual Loader* GetLoader()
    {
        return new JpgLoader();
    }
};

#endif