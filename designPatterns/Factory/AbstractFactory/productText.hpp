
// 抽象产品

#ifndef _CONCRETE_TEXT_HPP_
#define _CONCRETE_TEXT_HPP_

// 文字产品
class Text 
{
public:
    Text()
    {

    }
    virtual ~Text()
    {

    }
    virtual void PlayText() = 0;
};

#endif