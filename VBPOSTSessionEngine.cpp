#include "VBPOSTSessionEngine.h"

VBPOSTSessionEngine::VBPOSTSessionEngine(QObject *parent) :
    QObject(parent)
{
    sSessionString = "";
}

void VBPOSTSessionEngine::addParam(QString pParamName)
{
    sSessionVars.append(pParamName);
}

void VBPOSTSessionEngine::writeParams(QString pParamString)
{
    sSessionString = "";
    foreach (QString sessionVar, sSessionVars)
    {
        sSessionString += "&"+sessionVar+"="+getSessionValue(sessionVar,pParamString);
    }
}

QString VBPOSTSessionEngine::getParamString()
{
    return sSessionString;
}

QString VBPOSTSessionEngine::getSessionValue(QString pParamName, QString pParamString)
{
    int first = pParamString.indexOf(pParamName);
    if(first==-1) return "";
    QString tempStr = pParamString.mid(first);
    first = tempStr.indexOf("=");
    if(first==-1) return "";
    tempStr = tempStr.mid(first+1);
    first = tempStr.indexOf("&");
    if(first==-1) return tempStr;
    tempStr = tempStr.left(first);
    return tempStr;
}
