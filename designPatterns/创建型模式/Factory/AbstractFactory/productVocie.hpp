
// 抽象产品

#ifndef _PRODUCT_VOICE_HPP_
#define _PRODUCT_VOICE_HPP_

// 语音产品
class Voice 
{
public:
    Voice()
    {

    }
    virtual ~Voice()
    {

    }
    virtual void PlayVoice() = 0;
};

#endif