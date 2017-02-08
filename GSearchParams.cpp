#include "GSearchParams.h"

GSearchParams::GSearchParams(QString pParSearshText,
              long pParCountResults,
              long pParMaxParallelThreads,
              long pCurrPage,
              VBSearshEngineParameters pEngine,
              VBFindProfile pProfile,
             // ProxyList *pDProxyList,
              bool pUseFilter,
              bool pUseProxy, bool pCheckOnlyMode, int pProxyCheckingTime, bool pUseCaptcha, QString pCaptchaKey,
                             bool pCollectEmails)
{
    sParSearshText = pParSearshText;
    sParCountResults = pParCountResults;
    sParMaxParallelThreads = pParMaxParallelThreads;
    sCurrPage = pCurrPage;
    sEngine = pEngine;
    sProfile = pProfile;
    //sDProxyList = pDProxyList;
    sUseFilter = pUseFilter;
    sUseProxy = pUseProxy;
    sCheckOnlyMode = pCheckOnlyMode;
    sProxyCheckingTime = pProxyCheckingTime;
    sUseCaptcha = pUseCaptcha;
    sCaptchaKey = pCaptchaKey;
    sCollectEmails = pCollectEmails;
}
