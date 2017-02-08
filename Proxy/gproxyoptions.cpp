#include "gproxyoptions.h"

GProxyOptions::GProxyOptions()
{
    HostName = "";
    Port     = 0;
    UserName = "";
    Password = "";
}

GProxyOptions::GProxyOptions(QString InHostName, int InPort, QString InUserName, QString InPassword)
{

    HostName = InHostName;
    Port     = InPort;
    UserName = InUserName;
    Password = InPassword;

}
