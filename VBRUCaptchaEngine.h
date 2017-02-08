#ifndef VBRUCAPTCHAENGINE_H
#define VBRUCAPTCHAENGINE_H

#include <QObject>
#include <VBRUCapcha.h>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtWebKitWidgets/QWebView>
#include <QBuffer>
#include <VBSearshEngineParameters.h>
#include <QIODevice>
#include <QWebElement>
#include <QWebFrame>
#include <QBuffer>
#include <QImage>
#include "global.h"
//#include <QDebug>
#include <QFile>
#include <QHash>

class VBRUCaptchaEngine : public QObject
{
    Q_OBJECT
public:
    explicit VBRUCaptchaEngine(QObject *parent = 0);
    VBRUCaptchaEngine(VBRUCapcha pCAPTCHAParams, QObject *parent = 0);
    virtual ~VBRUCaptchaEngine();
    void setKey(QString pCAPTCHAKey);
    void checkCAPTCHA(QByteArray pData, VBSearshEngineParameters params);
    void watchDog();
    static QByteArray getCaptchaImage(QWebView *view, QString data, VBRUCapcha captchaParams);
    void releaseCaptchaForm(QString captchaValue);

private:
    VBRUCapcha sCAPTCHAParams;
    QString sCAPTCHAKey;
    QByteArray sCAPTCHAID;
    QNetworkAccessManager* manager;
    QNetworkAccessManager* managerGet;
    QTimer* dTimer;
    VBSearshEngineParameters seParams;

protected slots:
    void checkCAPTCHAFinished(QNetworkReply*reply);
    void checkCAPTCHAAnswerFinished(QNetworkReply*reply);
    //void checkCAPTCHARelease(QNetworkReply*reply);

signals:
    void captchaCheckingResult(QString captchaCheckingResult); //QHash<QByteArray,QByteArray> result);
    void captchaErrorResult(QByteArray result);

public slots:
    void checkCAPTCHAAnsver();


};

#endif // VBRUCAPTCHAENGINE_H
