#include "VBParameters.h"


VBParameters::VBParameters(QObject* parent):QObject(parent)
{
    sParamPath = "";
    sError = "";
//    codec = QTextCodec::codecForName("CP1251");
//    QTextCodec::setCodecForTr(codec);
}

VBParameters::VBParameters(QString pParamPath, QObject* parent):QObject(parent)
{
    sParamPath = pParamPath;
    sError = "";
    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, sParamPath);
//    codec = QTextCodec::codecForName("CP1251");
//    QTextCodec::setCodecForTr(codec);
}

// Static params

void VBParameters::setMainParams(VBMainParams pParams)
{
    QSettings settings("BiByte","Butterfly");
    settings.setValue("keywordArray",pParams.sKeywordArray);
    settings.setValue("notFirstRun",pParams.sNotFirstRun);
    settings.setValue("maxSearshCount",pParams.sMaxSearshCount);
    settings.setValue("maxParallelThreads",pParams.sMaxParallelThreads);
    settings.setValue("outputFile",pParams.sOutputFileName);
    settings.setValue("keyType",pParams.sKeyType);
    settings.setValue("inputKeyFile",pParams.sInputKeyFile);
    settings.setValue("currentFP",pParams.sCurrentFP);
    settings.setValue("currentSE",pParams.sCurrentSE);
    settings.setValue("loadSitesFromFile",pParams.sIsLoadSitesFromFile);
    settings.setValue("fileWithSites",pParams.sFileWithSites);
    settings.setValue("currentLang",pParams.sCurrentLang);
    settings.setValue("isWriteLogToFile",pParams.sIsWriteLogToFile);
    settings.setValue("logFile",pParams.sLogFile);
    settings.setValue("saveDomainOnly",pParams.sSaveDomainOnly);
    settings.setValue("domainPath",pParams.sDomainPath);
    settings.setValue("SaveDuplicates",pParams.sSaveDuplicates);
    settings.setValue("SaveErrorSites",pParams.sSaveErrorSites);
    //settings.setValue("Login",pParams.sLogin);
    settings.setValue("ProxyCheckingTime",pParams.sProxyCheckingTime);
    settings.setValue("WhatToDo",pParams.sWhatToDo);
    //GCrypt crypt;
    //crypt.init();
    QByteArray b;
    b.append(pParams.sPass);
    //settings.setValue("Pass",crypt.encode(b,"Gi6fr57uR5FugUIfg"));
    settings.setValue("UseFilter",pParams.sUseFilter);
    settings.setValue("UseProxy", pParams.sUseProxy);
    settings.setValue("UseCAPTCHA", pParams.sUseCAPTCHA);
    settings.setValue("CAPTCHAKey", pParams.sCAPTCHAKey);

}

VBMainParams VBParameters::getMainParams()
{
    QSettings settings("BiByte","Butterfly");
    VBMainParams sParams;
    if(settings.value("notFirstRun").toString().isEmpty())
    {
        QSettings settingsOld("VBI","Butterfly");
        sParams.sKeywordArray = settingsOld.value("keywordArray").toString();
        sParams.sNotFirstRun = settingsOld.value("notFirstRun").toBool();
        sParams.sMaxSearshCount = settingsOld.value("maxSearshCount").toInt();
        sParams.sMaxParallelThreads = settingsOld.value("maxParallelThreads").toInt();
        sParams.sOutputFileName = settingsOld.value("outputFile").toString();
        sParams.sKeyType = settingsOld.value("keyType").toBool();
        sParams.sInputKeyFile = settingsOld.value("inputKeyFile").toString();
        sParams.sCurrentFP = settingsOld.value("currentFP").toString();
        sParams.sCurrentSE = settingsOld.value("currentSE").toString();
        sParams.sIsLoadSitesFromFile = settingsOld.value("loadSitesFromFile").toBool();
        sParams.sFileWithSites = settingsOld.value("fileWithSites").toString();
        sParams.sCurrentLang = settingsOld.value("currentLang").toString();
        sParams.sSaveDomainOnly = true;
        sParams.sSaveDuplicates = false;
        sParams.sUseProxy = false;
        sParams.sProxyCheckingTime = 1;
        sParams.sWhatToDo = 0;
    }
    else
    {
        sParams.sKeywordArray = settings.value("keywordArray").toString();
        sParams.sNotFirstRun = settings.value("notFirstRun").toBool();
        sParams.sMaxSearshCount = settings.value("maxSearshCount").toInt();
        sParams.sMaxParallelThreads = settings.value("maxParallelThreads").toInt();
        sParams.sOutputFileName = settings.value("outputFile").toString();
        sParams.sKeyType = settings.value("keyType").toBool();
        sParams.sInputKeyFile = settings.value("inputKeyFile").toString();
        sParams.sCurrentFP = settings.value("currentFP").toString();
        sParams.sCurrentSE = settings.value("currentSE").toString();
        sParams.sIsLoadSitesFromFile = settings.value("loadSitesFromFile").toBool();
        sParams.sFileWithSites = settings.value("fileWithSites").toString();
        sParams.sCurrentLang = settings.value("currentLang").toString();
        sParams.sIsWriteLogToFile = settings.value("isWriteLogToFile").toBool();
        sParams.sLogFile = settings.value("logFile").toString();
        if(settings.contains("WhatToDo"))
        {
            sParams.sWhatToDo = settings.value("WhatToDo").toInt();
        }
        else
        {
            sParams.sWhatToDo = 0;
        }
        if(!settings.contains("saveDomainOnly"))
        {
            sParams.sSaveDomainOnly = true;
        }
        else
        {
            sParams.sSaveDomainOnly = settings.value("saveDomainOnly").toBool();
        }
        sParams.sDomainPath = settings.value("domainPath").toString();
        sParams.sSaveDuplicates = settings.value("SaveDuplicates").toBool();
        sParams.sSaveErrorSites = settings.value("SaveErrorSites").toBool();
        //sParams.sLogin = settings.value("Login").toString();
        sParams.sProxyCheckingTime = settings.value("ProxyCheckingTime").toInt();
        if(sParams.sProxyCheckingTime == 0) sParams.sProxyCheckingTime = 1;
        //GCrypt crypt;
       // crypt.init();
        //sParams.sPass = crypt.decode(settings.value("Pass").toByteArray(),"Gi6fr57uR5FugUIfg");
        sParams.sUseFilter = settings.value("UseFilter").toBool();
        sParams.sUseProxy = settings.value("UseProxy").toBool();
        sParams.sUseCAPTCHA = settings.value("UseCAPTCHA").toBool();
        sParams.sCAPTCHAKey = settings.value("CAPTCHAKey").toString();
    }

    if(settings.value("UseFilter").isNull())
    {
        sParams.sUseFilter = true;
    }
    return sParams;
}

void VBParameters::updateOnlineParams(VBOnlineParams pParams)
{
    QSettings settings("BiByte","Butterfly");
    settings.setValue("opCurrPage",pParams.sCurrPage);
    settings.setValue("opFailed",pParams.sFailed);
    settings.setValue("opRepeating",pParams.sRepeating);
    settings.setValue("opValid",pParams.sValid);
    settings.setValue("opWrong",pParams.sWrong);
}

void VBParameters::resetOnlineParams()
{
    updateOnlineParams(VBOnlineParams(0,0,0,0,0));
}

VBOnlineParams VBParameters::getOnlineParams()
{
    QSettings settings("BiByte","Butterfly");
    bool ok = true;
    return VBOnlineParams(settings.value("opCurrPage").toLongLong(&ok),
                          settings.value("opValid").toLongLong(&ok),
                          settings.value("opRepeating").toLongLong(&ok),
                          settings.value("opWrong").toLongLong(&ok),
                          settings.value("opFailed").toLongLong(&ok));
}

QHash<QString,QString> VBParameters::getLangList()
{
    tr("English");
    QDir dir(sParamPath);
    if(!dir.exists("lang"))
    {
        dir.mkdir("lang");
    }
    dir.cd("lang");
    QHash<QString,QString> result;
    result["English"]="";
    QStringList langs = dir.entryList(QDir::Files,QDir::Name);
    foreach(QString lang, langs)
    {
        QFileInfo fileInfo(dir.absoluteFilePath(lang));
        if(fileInfo.suffix() != "lng") continue;
        QTranslator translator;
        if(translator.load(lang, dir.path()))
        {
            result[translator.translate("VBParameters", "English")]=lang;
        }
    }
    return result;
}

QHash<QString,QString> VBParameters::getParamList(QString profileType)
{
    QStringList unicNameList;
    QDir dir(sParamPath);
    if(!dir.exists(profileType))
    {
        dir.mkdir(profileType);
    }
    dir.cd(profileType);
    QHash<QString,QString> result;
    QStringList sParams = dir.entryList(QDir::Files,QDir::Name);
    foreach(QString sParam, sParams)
    {
        QFileInfo fileInfo(dir.absoluteFilePath(sParam));
        if(fileInfo.suffix() != "ini" && profileType == cFindProfileFolder) continue;
        QSettings settings(QSettings::IniFormat, QSettings::SystemScope, profileType, fileInfo.completeBaseName());
        QString desc = settings.value(profileType+"Name").toString();
        if(desc.isEmpty()) continue;
        if(unicNameList.contains(desc))
        {
            addError(tr("In the parameters file ")+sParam+tr(", in folder ")+profileType+tr(", specified ")+desc+tr(", specified in another parameter file is already loaded into the program!"));
            continue;
        }else
        {
            unicNameList.append(desc);
        }
        result[settings.value(profileType+"Name").toString()]=fileInfo.completeBaseName();
    }
    return result;
}

// Списки профилей отримуються (назви файлів тіпа)
QHash<QString,QString> VBParameters::getSearshEnginesList()
{
    return getParamList(cSearshEngineFolder);
}

QHash<QString,QString> VBParameters::getFindProfileList()
{
    return getParamList(cFindProfileFolder);
}

// Профілі отримуються і записуються
VBSearshEngineParameters VBParameters::getSearshEngine(QString pID)
{
    QSettings settings(QSettings::IniFormat, QSettings::SystemScope, cSearshEngineFolder, pID);
    VBSearshEngineParameters sSearshEngineParams;
    QString profileType = cSearshEngineFolder;
    sSearshEngineParams.sName = settings.value(profileType + "Name").toString();
    sSearshEngineParams.sDescription = settings.value("Description").toString();
    sSearshEngineParams.sDate = settings.value("Date").toDate();
    sSearshEngineParams.sLastPageCheck = settings.value("LastPageCheck").toString();
    sSearshEngineParams.sSearshHost = settings.value("SearshHost").toString();
    sSearshEngineParams.sNoResultsCheck = settings.value("NoResultsCheck").toString();
    sSearshEngineParams.sSearshMachineBrand = settings.value("SearshMachineBrand").toString();
    sSearshEngineParams.sSpamCheck = settings.value("SpamCheck").toString();
    sSearshEngineParams.sStartPage = settings.value("StartPage").toInt();
    sSearshEngineParams.sPageStep = settings.value("PageStep").toInt();
    sSearshEngineParams.sLastPageCheckBox = settings.value("LastPageCheckBox").toBool();
    sSearshEngineParams.sSpamCheckBox = settings.value("SpamCheckBox").toBool();
    sSearshEngineParams.sNoResultsCheckBox = settings.value("NoResultsCheckBox").toBool();
    sSearshEngineParams.sSessionParams = settings.value("SessionParams").toString();
    if(settings.contains("MinWait"))
    {
        sSearshEngineParams.sMinWait = settings.value("MinWait").toInt();
    }
    else
    {
        sSearshEngineParams.sMinWait = 3;
    }
    if(settings.contains("MaxWait"))
    {
        sSearshEngineParams.sMaxWait = settings.value("MaxWait").toInt();
    }
    else
    {
        sSearshEngineParams.sMaxWait = 8;
    }

    VBRUCapcha ruCaptcha(settings.value("RuCaptchaPhrase").toInt(),
                         settings.value("RuCaptchaRegsense").toInt(),
                         settings.value("RuCaptchaNumeric").toInt(),
                         settings.value("RuCaptchaCalc").toInt(),
                         settings.value("RuCaptchaMinLen").toInt(),
                         settings.value("RuCaptchaMaxLen").toInt(),
                         settings.value("RuCaptchaLanguage").toInt(),
                         settings.value("RuCaptchaW3Selector").toString(),
                         settings.value("RuCaptchaFormW3Selector").toString(),
                         settings.value("RuCaptchaResultW3Selector").toString(),
                         settings.value("RuCaptchaSubmitW3Selector").toString());
    sSearshEngineParams.sCaptchaParams = ruCaptcha;

    return sSearshEngineParams;
}

VBFindProfile VBParameters::getFindProfile(QString pID)
{
    bool ok = true;
    QSettings settings(QSettings::IniFormat, QSettings::SystemScope, cFindProfileFolder, pID);
    VBFindProfile sFindProfile;
    sFindProfile.sSpecificURL = settings.value("SpecificURL").toString();
    QString profileType = cFindProfileFolder;
    sFindProfile.sName = settings.value(profileType+"Name").toString();
    sFindProfile.sDescription = settings.value("Description").toString();
    sFindProfile.sDate = settings.value("Date").toDate();
    sFindProfile.sCheckSpecificURL = settings.value("CheckSpecificURL").toBool();

    int count = settings.value("WhiteListCount").toInt(&ok);
    if(!ok) addError(tr("Wrong value of parameter WhiteListCount in file of search engine profile: ")+pID+"!");
    for(int i=1;i<=count;i++)
    {
        sFindProfile.sWhiteList.append(settings.value("WhiteListRecord"+QString::number(i)).toStringList());
    }

    count = settings.value("BlackListCount").toInt(&ok);
    if(!ok) addError(tr("Wrong value of parameter BlackListCount in file of search engine profile: ")+pID+"!");
    for(int i=1;i<=count;i++)
    {
        sFindProfile.sBlackList.append(settings.value("BlackListRecord"+QString::number(i)).toStringList());
    }

    return sFindProfile;
}

void VBParameters::setSearshEngine(VBSearshEngineParameters pParams, QString pID) // -1 Якщо новий профіль
{
    QSettings settings(QSettings::IniFormat, QSettings::SystemScope, cSearshEngineFolder, pID);
    QString profileType = cSearshEngineFolder;
    settings.setValue(profileType+"Name", pParams.sName);
    settings.setValue("Description", pParams.sDescription);
    settings.setValue("Date", pParams.sDate);
    settings.setValue("LastPageCheck", pParams.sLastPageCheck);
    settings.setValue("SearshHost", pParams.sSearshHost);
    settings.setValue("NoResultsCheck", pParams.sNoResultsCheck);
    settings.setValue("SearshMachineBrand", pParams.sSearshMachineBrand);
    settings.setValue("SpamCheck", pParams.sSpamCheck);
    settings.setValue("StartPage", pParams.sStartPage);
    settings.setValue("PageStep", pParams.sPageStep);
    settings.setValue("LastPageCheckBox", pParams.sLastPageCheckBox);
    settings.setValue("SpamCheckBox", pParams.sSpamCheckBox);
    settings.setValue("NoResultsCheckBox", pParams.sNoResultsCheckBox);
    settings.setValue("MinWait", pParams.sMinWait);
    settings.setValue("MaxWait", pParams.sMaxWait);
    settings.setValue("SessionParams", pParams.sSessionParams);

    settings.setValue("RuCaptchaPhrase", pParams.sCaptchaParams.sPhrase);
    settings.setValue("RuCaptchaRegsense", pParams.sCaptchaParams.sRegsense);
    settings.setValue("RuCaptchaNumeric", pParams.sCaptchaParams.sNumeric);
    settings.setValue("RuCaptchaCalc", pParams.sCaptchaParams.sCalc);
    settings.setValue("RuCaptchaMinLen", pParams.sCaptchaParams.sMin_len);
    settings.setValue("RuCaptchaMaxLen", pParams.sCaptchaParams.sMax_len);
    settings.setValue("RuCaptchaLanguage", pParams.sCaptchaParams.sLanguage);
    settings.setValue("RuCaptchaW3Selector", pParams.sCaptchaParams.sW3CaptchaSelector);
    settings.setValue("RuCaptchaFormW3Selector", pParams.sCaptchaParams.sW3FormCaptchaSelector);
    settings.setValue("RuCaptchaResultW3Selector", pParams.sCaptchaParams.sW3ResultCaptchaSelector);
    settings.setValue("RuCaptchaSubmitW3Selector", pParams.sCaptchaParams.sW3SubmitButtonSelector);
}

void VBParameters::setFindProfile(VBFindProfile pParams, QString pID)
{
    QSettings settings(QSettings::IniFormat, QSettings::SystemScope, cFindProfileFolder, pID);
    settings.setValue("SpecificURL", pParams.sSpecificURL);
    QString profileType = cFindProfileFolder;
    settings.setValue(profileType+"Name", pParams.sName);
    settings.setValue("Description", pParams.sDescription);
    settings.setValue("Date", pParams.sDate);
    settings.setValue("CheckSpecificURL", pParams.sCheckSpecificURL);
    settings.setValue("WhiteListCount", pParams.sWhiteList.count());
    int counter = 1;
    foreach(QStringList s, pParams.sWhiteList)
    {
        settings.setValue("WhiteListRecord"+QString::number(counter++), s);
    }
    settings.setValue("BlackListCount", pParams.sBlackList.count());
    counter = 1;
    foreach(QStringList s, pParams.sBlackList)
    {
        settings.setValue("BlackListRecord"+QString::number(counter++), s);
    }
}

bool VBParameters::delSearshEngine(QString pID)
{
    QDir dir(sParamPath);
    if(!dir.exists(cSearshEngineFolder))
    {
        dir.mkdir(cSearshEngineFolder);
    }
    dir.cd(cSearshEngineFolder);
    return dir.remove(pID+".ini");
}

bool VBParameters::delFindProfile(QString pID)
{
    QDir dir(sParamPath);
    if(!dir.exists(cFindProfileFolder))
    {
        dir.mkdir(cFindProfileFolder);
    }
    dir.cd(cFindProfileFolder);
    return dir.remove(pID+".ini");
}

void VBParameters::addError(QString text)
{
    QString resText = text.simplified();
    if(!resText.isEmpty())
    {
        sError.prepend(" ");
        sError.prepend(resText);
    }
}

void VBParameters::setPath(QString path)
{
    sParamPath = path;
}

QString VBParameters::getPath()
{
    return sParamPath;
}

QString VBParameters::getFPPath()
{
    QDir dir(sParamPath);
    if(!dir.exists(cFindProfileFolder))
    {
        dir.mkdir(cFindProfileFolder);
    }
    dir.cd(cFindProfileFolder);
    return dir.path();
}

QString VBParameters::getSEPath()
{
    QDir dir(sParamPath);
    if(!dir.exists(cSearshEngineFolder))
    {
        dir.mkdir(cSearshEngineFolder);
    }
    dir.cd(cSearshEngineFolder);
    return dir.path();
}

QString VBParameters::getLastError()
{
    QString error = sError;
    sError = "";
    return error;
}


