#ifndef GPROXYOPTIONS_H
#define GPROXYOPTIONS_H

#include <QString>

class GProxyOptions
{

public:
    //Options
    QString HostName;
    int Port;
    QString UserName;
    QString Password;

    GProxyOptions();
    GProxyOptions(QString InHostName, int InPort = 0, QString InUserName = "", QString InPassword = "");

};

#endif // GPROXYOPTIONS_H
