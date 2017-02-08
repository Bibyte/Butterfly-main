#ifndef VBCHECKURLTHREAD_H
#define VBCHECKURLTHREAD_H

#include <QThread>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QList>
#include <QStringList>
#include <QTimer>
#include <QMutex>
#include "global.h"
#include <QTextCodec>
#include <QByteArray>
#include <QSslError>

class VBCheckURLThread : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
    QString checkForPHPData;
    QString sDomain;
    QTimer *timer;
    QUrl sURL;
    QUrl sSaveUrl;
    QMutex mtx;
    VBFindProfile sFPParam;
    QThread* sCurrentThread;
    bool sManagerWasCreated;
    bool replyWasDeleted;
    long sID;
    bool sCollectEmails;

protected:
    QByteArray getCodePage(QByteArray data);

protected slots:
    void checkSiteFinished(QNetworkReply* localReply);
    void checkAuthenticationRequired(QNetworkReply*,QAuthenticator*);
    void checkSslErrors(QNetworkReply* reply,QList<QSslError> errList);
    void checkWork();
    void replyDestroyed();
public slots:
    void runChecking();
public:
    explicit VBCheckURLThread(QObject *parent = 0);
    VBCheckURLThread(const long& parID, const QUrl& parURL, const QUrl& parSaveURL, const VBFindProfile& parFPParam, bool parCollectEmails, QThread* parCurrentThread, QObject *parent = 0);
    ~VBCheckURLThread();
    QThread* getCurrentThread();
    void setParameters(const long& parID, const QUrl& parURL, const QUrl &parSaveURL, const VBFindProfile &parFPParam);
signals:
    void siteErrorSig(const long id, const QUrl& url, const QString& msg); // Якщо час вийшов а сайт не повернуув результат
    void addSiteSig(const long id, const QUrl& url); // коли усі обмеження пройшли
    void siteIsWrongSig(const long id, const QUrl& url);
    void quitNode(VBCheckURLThread* thisObject, bool deleting);
    void startTimer(int msec);
    void stopTimer();
    void fragmentsFoundSig(const long id, QStringList lst);

public slots:
    void offNode(bool deleting = false);

};

#endif // VBCHECKURLTHREAD_H
