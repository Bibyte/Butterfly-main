#ifndef VBMAINPARAMS_H
#define VBMAINPARAMS_H

#include <QString>

class VBMainParams
{
public:
    QString sKeywordArray;
    bool sNotFirstRun;
    int sMaxSearshCount;
    int sMaxParallelThreads;
    QString sOutputFileName;
    int sKeyType;
    QString sInputKeyFile;
    QString sFileWithSites;
    int sIsLoadSitesFromFile;
    QString sProgramPath;
    QString sCurrentSE;
    QString sCurrentFP;
    QString sCurrentLang;
    bool sIsWriteLogToFile;
    QString sLogFile;
    bool sSaveDomainOnly;
    QString sDomainPath;
    bool sSaveDuplicates;
    bool sSaveErrorSites;
    QString sLogin;
    QString sPass;
    bool sUseFilter;
    bool sUseProxy;
    int sProxyCheckingTime;
    bool sUseCAPTCHA;
    QString sCAPTCHAKey;
    int sWhatToDo;
public:
    VBMainParams();
    VBMainParams(QString pKeywordArray,
                 bool pNotFirstRun,
                 int pMaxSearshCount,
                 int pMaxParallelThreads,
                 QString pOutputFileName,
                 int pKeyType,
                 QString pInputKeyFile,
                 QString pFileWithSites,
                 int pIsLoadSitesFromFile,
                 QString pProgramPath,
                 QString pCurrentSE,
                 QString pCurrentFP,
                 QString pCurrentLang,
                 bool pIsWriteLogToFile,
                 QString pLogFile,
                 bool pSaveDomainOnly,
                 QString pDomainPath,
                 bool pSaveDuplicates,
                 bool pSaveErrorSites,
                 QString pLogin,
                 QString pPass,
                 bool pUseFilter,
                 bool pUseProxy,
                 int pProxyCheckingTime,
                 bool pUseCAPTCHA,
                 QString pCAPTCHAKey,
                 int pWhatToDo);
};

#endif // VBMAINPARAMS_H
