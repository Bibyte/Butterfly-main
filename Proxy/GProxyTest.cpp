#include "GProxyTest.h"

GProxyTest::GProxyTest(QObject *parent):
    QObject(parent)
{

}

bool GProxyTest::checkProxy(QString pHost, int pPort, int sProxyCheckingTime = 1)
{
    QTcpSocket* dTCPSocket = new QTcpSocket;
    dTCPSocket->connectToHost(pHost,pPort,QTcpSocket::ReadOnly);
    bool result = dTCPSocket->waitForConnected(sProxyCheckingTime * 1000);
    if(result) dTCPSocket->disconnect();
    dTCPSocket->deleteLater();
    return result;
}

