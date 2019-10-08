
#include "product.hpp"
#include "factory.hpp"
#include "factoryJpgLoader.hpp"
#include "factoryPngLoader.hpp"
#include "factoryGifLoader.hpp"


int main(int argc, char const *argv[])
{
    // LoaderFactory *pFactory = new JpgLoaderFactory();
    // LoaderFactory *pFactory = new PngLoaderFactory();
    LoaderFactory *pFactory = new GifLoaderFactory();
    Loader* pLoader = pFactory->GetLoader();
    pLoader->LoaderImg();
    return 0;
}
