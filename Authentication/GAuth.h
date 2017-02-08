#ifndef GAUTH_H
#define GAUTH_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <Authentication/GBProtocol.h>
#include <Authentication/gmac.h>

class GAuth : public QObject
{
    Q_OBJECT
private:
    QByteArray sDecodeKey;
    QNetworkAccessManager* dAuthManager;
    QTimer sWatchDogTimer;

protected:
    QString getHTMLMessage(int code);
    QString getHTMLServiceMessage(int code, int satus); // 1 - недоступно, 2 - доступно, 3 - блоковано

public:
    explicit GAuth(QObject *parent = 0);
    ~GAuth();
    
signals:
    void EnableService(int code);
    void DisableService(int code);
    void sendHTMLBody(QString, bool);
    void setAuthStatus(bool);
    void HaveNoServices(bool);

protected slots:
    void connectionUpdated(QNetworkReply* reply);
    void connectionNotResponse();
    
public slots:
    void updateConnection(QString pLogin, QString pPass);

};

#endif // GAUTH_H
