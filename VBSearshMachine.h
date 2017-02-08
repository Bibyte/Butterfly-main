#ifndef VBSEARSHMACHINE_H
#define VBSEARSHMACHINE_H

#include <QObject>
#include <QUrl>
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QTimer>
#include <QMutex>
#include <QDateTime>
#include "VBCheckUrlThread.h"
#include <QThread>
#include <QStack>
#include "global.h"
#include <QFile>
#include <QTextStream>
#include "Proxy/proxylist.h"
#include <QHash>
#include <GResultContainer.h>
#include <GSearchParams.h>
#include <Proxy/GProxyTest.h>
#include <QSslSocket>
#include <VBPOSTSessionEngine.h>
#include <VBRUCaptchaEngine.h>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

class VBSearshMachine : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager* manager;
    QTimer *timer;
    QString sSearshMachineBrand, sLastPageCheck, sSpamCheck;
    long currSite;
    long currSiteGlobal;
    long currSiteGlobalReturn;
    long currPage;
    QString sSearshText;
    long sCountResults;
    QString sCurrentSearshData;
    bool isActive, isEnd;
    long countThreads;
    long sMaxParallelThreads;
    QString sCurrFindKeyword;
    QMutex mtx;
    QWebView* dBrowser;
    QString saveSearshData, previousSearchedData;
    bool sCheckOnlyMode, saveIsLastPage;
    QList<QUrl> sLastUrls;
    QStringList sPimaryFilter;
    QStack<VBCheckURLThread*> sAvalaibleThreads;
    ProxyList* dProxylist;
    QNetworkProxy sProxy;
    QHash<long, GResultContainer> sURLBuffer;
    bool sUseFilter, sUseProxy, sSSLErrors, sUseCaptcha;
    int sProxyCheckingTime;
    VBPOSTSessionEngine* sPOSTSessionEngine;
    VBRUCaptchaEngine* dCaptchaEngine;
    bool sCollectEmails;
private:
    VBSearshEngineParameters sSEParams;
    VBFindProfile sFPParams;
protected:
    int getWaitTime();
public:
    explicit VBSearshMachine(QObject *parent = 0);
    VBSearshMachine(const VBSearshEngineParameters& pSEParams, const VBFindProfile& pFPParams, QObject *parent = 0);
    ~VBSearshMachine();
protected:
    QString getNextKeyword(QString* SearshText);
    QString takeNextURL(QString*);
    void programPauseSearch(const QString& msg);
    void initBrowser(bool pDeleteOld = false);
    void execUrlBuffer();
signals:
    //void resultSearshed(QString, bool isLast = false); // тут відправляється список резултів для парсинга і перевірки.
    void sigLog(const QString&, bool); //лог
    void sigStatus(const QString&); // Відправляється сигнал зміни статусу і текст статусу
    void endSearshSig(); // машина зупинена
    void stopSearshSigThreads(bool = false); // Сигнал зупинити треди
    void parseNextUrlSig(QString&, bool = false);
    void siteErrorSig(const QUrl& url, const QString& msg); // Якщо сайт не відповів
    void siteWrongSig(const QUrl& url);
    void addSiteSig(const QUrl& url); // Треба додати сайт
    void fragmentsFoundSig(const QStringList& list);
    void siteExecSig(); // означает что сайт обработан
    void sendHTMLList(const QString&); // Отправляет текущий список HTML
    void programContinueSearchSig();
    void checkSite();
    void currPageSig(long);
protected slots:
    void searshFinished(bool normal = true);
    void parseNextUrl(QString &searshDataPart, bool isLastPage = false);
    void continueParseNextUrl();
    void siteError(const long& id, const QUrl& url, const QString& msg);
    void siteIsWrong(const long& id, const QUrl& url); // сайт не підходить під відбір
    void addSite(const long& id, const QUrl& url);
    void fragmentsFound(const long& id, const QStringList& list);
    void writeLog(const QString& message, bool showMessage = true);
    void checkWork();
    void movingToNextPage(bool nextPage = true);
    void hideBrovserAndContinueWork();
    void offClosedThread(VBCheckURLThread* objectToDelete, bool deleting);
    void captchaCheckingResult(QString);
    void captchaErrorResult(QByteArray result);
    void OnUrlChanged(const QUrl&);

public slots:
    void searshResult(GSearchParams params); // Статичні
    void stopSearsh();
    void pauseSearsh();
    void resumeSearsh();
};

#endif // VBSEARSHMACHINE_H
