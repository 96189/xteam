
//

#ifndef _PRODUCT_HPP_
#define _PRODUCT_HPP_

class Loader 
{
public:
    Loader()
    {

    }
    virtual ~Loader()
    {

    }

    // 接口定义
    virtual void LoaderImg() = 0;
};

#endif