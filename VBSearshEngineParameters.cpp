#include "VBSearshEngineParameters.h"

VBSearshEngineParameters::VBSearshEngineParameters()
{
}

VBSearshEngineParameters::VBSearshEngineParameters(QString pName,
        QString pDescription,
        QDate pDate,
        QString pSearshHost,
        QString pSearshMachineBrand,
        QString pLastPageCheck,
        QString pSpamCheck,
        QString pNoResultsCheck,
        int pStartPage,
        int pPageStep,
        bool pLastPageCheckBox,
        bool pSpamCheckBox,
        bool pNoResultsCheckBox,
        int pMinWait,
        int pMaxWait,
        QString pSessionParams,
        VBRUCapcha pCaptchaParams)
{
    sName = pName;
    sDescription = pDescription;
    sDate = pDate;
    sSearshHost = pSearshHost;
    sSearshMachineBrand = pSearshMachineBrand;
    sLastPageCheck = pLastPageCheck;
    sSpamCheck = pSpamCheck;
    sNoResultsCheck = pNoResultsCheck;
    sStartPage = pStartPage;
    sPageStep = pPageStep;
    sLastPageCheckBox = pLastPageCheckBox;
    sSpamCheckBox = pSpamCheckBox;
    sNoResultsCheckBox = pNoResultsCheckBox;
    sMinWait = pMinWait;
    sMaxWait = pMaxWait;
    sSessionParams = pSessionParams;
    sCaptchaParams = pCaptchaParams;
}
