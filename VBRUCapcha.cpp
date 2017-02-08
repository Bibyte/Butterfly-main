#include "VBRUCapcha.h"

VBRUCapcha::VBRUCapcha()
{
    sPhrase = 0;
    sRegsense = 0;
    sNumeric = 0;
    sCalc = 0;
    sMin_len = 0;
    sMax_len = 0;
    sLanguage = 0;

}

VBRUCapcha::VBRUCapcha(int pPhrase,
                       int pRegsense,
                       int pNumeric,
                       int pCalc,
                       int pMin_len,
                       int pMax_len,
                       int pLanguage,
                       QString pW3CaptchaSelector,
                       QString pW3FormCaptchaSelector,
                       QString pW3ResultCaptchaSelector,
                       QString pW3SubmitButtonSelector)
{
    sPhrase = pPhrase;
    sRegsense = pRegsense;
    sNumeric = pNumeric;
    sCalc = pCalc;
    sMin_len = pMin_len;
    sMax_len = pMax_len;
    sLanguage = pLanguage;
    sW3CaptchaSelector = pW3CaptchaSelector;
    sW3FormCaptchaSelector = pW3FormCaptchaSelector;
    sW3ResultCaptchaSelector = pW3ResultCaptchaSelector;
    sW3SubmitButtonSelector = pW3SubmitButtonSelector;
}
