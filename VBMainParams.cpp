#include "VBMainParams.h"

VBMainParams::VBMainParams()
{
}

VBMainParams::VBMainParams(QString pKeywordArray,
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
             int pWhatToDo)
{
    sKeywordArray = pKeywordArray;
    sNotFirstRun = pNotFirstRun;
    sMaxSearshCount = pMaxSearshCount;
    sMaxParallelThreads = pMaxParallelThreads;
    sOutputFileName = pOutputFileName;
    sKeyType = pKeyType;
    sInputKeyFile = pInputKeyFile;
    sProgramPath = pProgramPath;
    sCurrentFP = pCurrentFP;
    sCurrentSE = pCurrentSE;
    sFileWithSites = pFileWithSites;
    sIsLoadSitesFromFile = pIsLoadSitesFromFile;
    sCurrentLang = pCurrentLang;
    sIsWriteLogToFile = pIsWriteLogToFile;
    sLogFile = pLogFile;
    sSaveDomainOnly = pSaveDomainOnly;
    sDomainPath = pDomainPath;
    sSaveDuplicates = pSaveDuplicates;
    sSaveErrorSites = pSaveErrorSites;
    sLogin = pLogin;
    sPass = pPass;
    sUseFilter = pUseFilter;
    sUseProxy = pUseProxy;
    sProxyCheckingTime = pProxyCheckingTime;
    sUseCAPTCHA = pUseCAPTCHA;
    sCAPTCHAKey = pCAPTCHAKey;
    sWhatToDo = pWhatToDo;
}
