#ifndef GSEARCHPARAMS_H
#define GSEARCHPARAMS_H

#include <QString>
#include "VBSearshEngineParameters.h"
#include "VBFindProfile.h"
#include "Proxy/proxylist.h"

class GSearchParams
{
public:
    QString sParSearshText;
    long sParCountResults;
    long sParMaxParallelThreads;
    long sCurrPage;
    VBSearshEngineParameters sEngine;
    VBFindProfile sProfile;
   // ProxyList* sDProxyList;
    bool sUseFilter;
    bool sUseProxy;
    bool sCheckOnlyMode;
    bool sUseCaptcha;
    QString sCaptchaKey;
    int sProxyCheckingTime;
    bool sCollectEmails;
    GSearchParams(QString pParSearshText,
                  long pParCountResults,
                  long pParMaxParallelThreads,
                  long pCurrPage,
                  VBSearshEngineParameters pEngine,
                  VBFindProfile pProfile,
                  //ProxyList* pDProxyList,
                  bool pUseFilter,
                  bool pUseProxy,
                  bool pCheckOnlyMode,
                  int pProxyCheckingTime,
                  bool pUseCaptcha,
                  QString pCaptchaKey,
                  bool pCollectEmails);
};

#endif // GSEARCHPARAMS_H
