#ifndef VBSEARSHMODULE_H
#define VBSEARSHMODULE_H

#include <QObject>
#include "VBSearshMachine.h"
#include "VBCheckUrlThread.h"
#include "global.h"
#include "Proxy/proxylist.h"
#include "GSearchParams.h"

class VBSearshModule : public QObject
{
    Q_OBJECT 
private:
    VBSearshMachine* searshMachine;
public:
    explicit VBSearshModule(QObject *parent = 0);
    ~VBSearshModule();
    //void startSearsh(QString& parSearshText, long parCountResults, long parMaxParallelThreads, const VBSearshEngineParameters& engine, const VBFindProfile& profile, ProxyList* dProxyList, bool useFilter, bool checkOnlyMode = false);
    void startSearsh(GSearchParams params);

    void pauseSearsh();
    void continueSearsh();
    void stopSearsh();
signals:
    void sigLog(const QString&, bool); //лог
    void sigStatus(const QString&); // Відправляється сигнал зміни статусу і текст статусу
    void endSearshSig(); // машина зупинена
    void siteErrorSig(const QUrl &url, const QString &msg); // Якщо сайт не відповів
    void siteWrongSig(const QUrl &url); // Якщо сайт не пройшов відбір
    void addSiteSig(const QUrl& url); // Треба додати сайт
    void fragmentsFoundSig(const QStringList& list);
    void siteExecutedSig(); // означаэ що сайт оброблений
    void sendHTMLList(const QString&);
    void programContinueSearchSig();
    void currPageSig(long);

protected slots:
    void addLog(const QString&, bool); //лог
    void setStatus(const QString&); // Відправляється сигнал зміни статусу і текст статусу
    void endSearsh(); // машина зупинена
    void siteError(const QUrl &url, const QString &msg); // Якщо сайт не відповів
    void siteWrong(const QUrl &url);
    void addSite(const QUrl &url); // Треба додати сайт
    void fragmentsFound(const QStringList& list);
    void siteExec();
    void reciveHTMLList(const QString&);
    void programContinueSearch();
    void currPageSlot(long page);

};

#endif // VBSEARSHMODULE_H
