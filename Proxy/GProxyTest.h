#ifndef GPROXYTEST_H
#define GPROXYTEST_H

#include <QtNetwork/QTcpSocket>
#include <QObject>


class GProxyTest : public QObject
{
    Q_OBJECT
public:
    explicit GProxyTest(QObject *parent = 0);
    static bool checkProxy(QString pHost, int pPort, int sProxyCheckingTime);

};

#endif // GPROXYTEST_H
