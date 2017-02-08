#ifndef GPROXYENGINE_H
#define GPROXYENGINE_H

#include <QObject>
#include <Proxy/proxylist.h>

class GProxyEngine : public QObject
{
    Q_OBJECT
public:
    explicit GProxyEngine(QObject *parent = 0);

protected:
    void LoadProxyListFromFile();
    
signals:
    
public slots:
    
};

#endif // GPROXYENGINE_H
