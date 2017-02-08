#include "VBParamEngine.h"

VBParamEngine::VBParamEngine(QObject *parent)
{

}

VBParamEngine::VBParamEngine(QString pParamDir, QObject *parent) :
    VBParameters(pParamDir, parent)
{
    dSearshEngineEdditor = new SearshEngineEditor(pParamDir);
    dFindProfileEditor = new FindProfileEditor(pParamDir);
    connect(dSearshEngineEdditor,SIGNAL(profileChanged(QString)),this,SLOT(resendSEListChnaged(QString)));
    connect(dFindProfileEditor,SIGNAL(profileChanged(QString)),this,SLOT(resendFPListChanged(QString)));

    // qApp->applicationDirPath()
}

VBParamEngine::~VBParamEngine()
{
    delete dSearshEngineEdditor;
    delete dFindProfileEditor;
}

void VBParamEngine::openSearshEngineDialog(QString pCurrentSE)
{
    dSearshEngineEdditor->reloadProfileList();
    if(!pCurrentSE.isEmpty())
    {
        dSearshEngineEdditor->setCurrentSE(pCurrentSE);
    }
    dSearshEngineEdditor->show();
}

void VBParamEngine::openFindProfileDialog(QString pCurrentFP)
{
    dFindProfileEditor->reloadProfileList();
    if(!pCurrentFP.isEmpty())
    {
        dFindProfileEditor->setCurrentFP(pCurrentFP);
    }
    dFindProfileEditor->show();
}

void VBParamEngine::setMainParams(VBMainParams mainParams)
{
    sParams.setMainParams(mainParams);
}

VBMainParams VBParamEngine::getMainParams()
{
    return sParams.getMainParams();
}

QHash<QString,QString> VBParamEngine::getFindProfileParameterList()
{
    return sParams.getFindProfileList();
}

QHash<QString,QString> VBParamEngine::getSearshEngineParameterList()
{
    return sParams.getSearshEnginesList();
}

VBFindProfile VBParamEngine::getFindProfileParameter(QString pID)
{
    return sParams.getFindProfile(pID);
}

VBSearshEngineParameters VBParamEngine::getSearshEngineParameter(QString pID)
{
    return sParams.getSearshEngine(pID);
}

void VBParamEngine::resendFPListChanged(QString pChangedProfile)
{
    emit FPListChanged(pChangedProfile);
}

void VBParamEngine::resendSEListChnaged(QString pChangedProfile)
{
    emit SEListChnaged(pChangedProfile);
}

void VBParamEngine::setLocale(QLocale local)
{
    dSearshEngineEdditor->setLocale(local);
    dFindProfileEditor->setLocale(local);
}




