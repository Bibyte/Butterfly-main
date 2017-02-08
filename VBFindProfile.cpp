#include "VBFindProfile.h"

VBFindProfile::VBFindProfile()
{
}

VBFindProfile::VBFindProfile(QString pName, QString pDescription, QDate pDate, bool pCheckSpecificURL, QString pSpecificURL, QList<QStringList> pWhiteList, QList<QStringList> pBlackList)
{
    sName = pName;
    sDescription = pDescription;
    sDate = pDate;
    sSpecificURL = pSpecificURL;
    sCheckSpecificURL = pCheckSpecificURL;
    sWhiteList = pWhiteList;
    sBlackList = pBlackList;
}
