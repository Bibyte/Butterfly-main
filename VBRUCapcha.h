#ifndef VBRUCAPCHA_H
#define VBRUCAPCHA_H
#include <QString>
// API KEY: 3378f69c4fd07d3f9b99134cc099b4c1

class VBRUCapcha
{
public:
    VBRUCapcha();
    VBRUCapcha(int pPhrase,
        int pRegsense,
        int pNumeric,
        int pCalc,
        int pMin_len,
        int pMax_len,
        int pLanguage,
        QString pW3CaptchaSelector,
        QString pW3FormCaptchaSelector,
        QString pW3ResultCaptchaSelector,
        QString pW3SubmitButtonSelector);
    int sPhrase;
    int sRegsense;
    int sNumeric;
    int sCalc;
    int sMin_len;
    int sMax_len;
    int sLanguage;
    QString sW3CaptchaSelector;
    QString sW3FormCaptchaSelector;
    QString sW3ResultCaptchaSelector;
    QString sW3SubmitButtonSelector;
    //soft_id: bibyte
    //int sKey; // pr - в профілі


};

#endif // VBRUCAPCHA_H
