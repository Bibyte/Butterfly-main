#include "VBSearshModule.h"

VBSearshModule::VBSearshModule(QObject *parent) :
    QObject(parent)
{

}

VBSearshModule::~VBSearshModule()
{

}

//void VBSearshModule::startSearsh(QString& parSearshText, long parCountResults, long parMaxParallelThreads, const VBSearshEngineParameters& engine, const VBFindProfile& profile, ProxyList* dProxyList, bool useFilter, bool checkOnlyMode)
void VBSearshModule::startSearsh(GSearchParams params)
{
    //thread()->setPriority(QThread::LowestPriority);
    searshMachine = new VBSearshMachine(params.sEngine, params.sProfile);
    connect(searshMachine,SIGNAL(addSiteSig(const QUrl&)),this,SLOT(addSite(const QUrl&)));
    connect(searshMachine,SIGNAL(fragmentsFoundSig(const QStringList&)),this,SLOT(fragmentsFound(const QStringList&)));
    connect(searshMachine,SIGNAL(sigLog(const QString&,bool)),this,SLOT(addLog(const QString&,bool)));
    connect(searshMachine,SIGNAL(sigStatus(const QString&)),this,SLOT(setStatus(const QString&)));
    connect(searshMachine,SIGNAL(siteErrorSig(const QUrl&, const QString&)),this,SLOT(siteError(const QUrl&, const QString&)));
    connect(searshMachine,SIGNAL(siteExecSig()),this,SLOT(siteExec()));
    connect(searshMachine,SIGNAL(endSearshSig()),this,SLOT(endSearsh()));
    connect(searshMachine,SIGNAL(siteWrongSig(const QUrl&)),this,SLOT(siteWrong(const QUrl&)));
    connect(searshMachine,SIGNAL(programContinueSearchSig()),this,SLOT(programContinueSearch()));
    connect(searshMachine,SIGNAL(sendHTMLList(const QString&)),this,SLOT(reciveHTMLList(const QString&)));
    connect(searshMachine,SIGNAL(currPageSig(long)),this,SLOT(currPageSlot(long)));
    //searshMachine->searshResult(parSearshText,parCountResults,parMaxParallelThreads,useFilter,checkOnlyMode);
    searshMachine->searshResult(params);
}

void VBSearshModule::pauseSearsh()
{
    searshMachine->pauseSearsh();
}

void VBSearshModule::continueSearsh()
{
    searshMachine->resumeSearsh();
}

void VBSearshModule::stopSearsh()
{
    searshMachine->stopSearsh();
   // delete searshMachine;
    //searshMachine->deleteLater();
}

// Слоти

void VBSearshModule::addLog(const QString& text, bool msg)
{
    emit sigLog(text,msg);
}

void VBSearshModule::setStatus(const QString& text)
{
    emit sigStatus(text);
}

void VBSearshModule::endSearsh()
{
    emit endSearshSig();
}

void VBSearshModule::siteError(const QUrl &url, const QString& msg)
{
    emit siteErrorSig(url, msg);
}

void VBSearshModule::siteWrong(const QUrl &url)
{
    emit siteWrongSig(url);
}

void VBSearshModule::siteExec()
{
    emit siteExecutedSig();
}

void VBSearshModule::addSite(const QUrl& url)
{
    emit addSiteSig(url);
}

void VBSearshModule::fragmentsFound(const QStringList& list)
{
    emit fragmentsFoundSig(list);
}

void VBSearshModule::reciveHTMLList(const QString& HTMLText)
{
    emit sendHTMLList(HTMLText);
}

void VBSearshModule::programContinueSearch()
{
    emit programContinueSearchSig();
}

void VBSearshModule::currPageSlot(long page)
{
    emit currPageSig(page);
}




