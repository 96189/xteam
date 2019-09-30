
#include "abstractFactory.hpp"
#include "concreteFactoryChinese.hpp"
#include "concreteFactoryEnglish.hpp"
#include "productText.hpp"
#include "productVocie.hpp"


int main(int argc, char const *argv[])
{
    GlobalizationAF *pGAF = new ChineseFactory();
    Text* pText = pGAF->GetText();
    Voice* pVoice = pGAF->GetVoice();
    pText->PlayText();
    pVoice->PlayVoice();
    return 0;
}
