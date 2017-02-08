#ifndef PROXYLIST_H
#define PROXYLIST_H

#include <QObject>
#include <QList>
#include <QtNetwork/QNetworkProxy>
//#include <QTableWidget>
#include <QDir>
#include <QtWidgets/QApplication>
#include <QTextStream>
#include <Proxy/gproxyoptions.h>
#include <QDateTime>

class ProxyList : public QObject
{
    Q_OBJECT

protected:
  QList<QNetworkProxy> sProxyArr;
  QList<GProxyOptions> sProxyOptionArr;

public:
    explicit ProxyList(QObject *parent = 0);
    //ProxyList(QObject *parent = 0, QList<GProxyOptions> ProxyOptionsList);
    void UpdateProxyList(QList<GProxyOptions> ProxyOptionsList);
    void UpdateProxyOptionArr(QVariant OptionValue, int OptionIndex, int Position);
    void DeleteItemProxyOptionArr(int Position);
    QNetworkProxy GetRandomProxy();
    void LoadProxyListFromFile(QString FileName);
    void LoadProxyListFromSystemFile();
    void SaveProxyListToFile(QString FileName = "");


protected:
    QString GetProxyString(GProxyOptions ProxyOption);
    void LoadProxyArr();
    void UpdateProxyArr(QVariant ProxyValue, int OptionIndex, int Position);

signals:
    void SendProxyList(QList<GProxyOptions>);
    void sigLog(const QString&, bool); //лог

public slots:

};

#endif // PROXYLIST_H
