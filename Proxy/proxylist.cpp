#include "proxylist.h"

ProxyList::ProxyList(QObject *parent):
    QObject(parent)
{
    //qsrand(458);
    QTime randTime = QDateTime::currentDateTime().time();
    qsrand(randTime.hour()+randTime.minute()+randTime.second());
    sProxyArr = QList<QNetworkProxy>();
    sProxyOptionArr = QList<GProxyOptions>();
    sProxyArr.clear();
    sProxyOptionArr.clear();
}

void ProxyList::UpdateProxyList(QList<GProxyOptions> ProxyOptionsList)
{

    sProxyArr.clear();

    foreach (GProxyOptions ProxyOptions, ProxyOptionsList) {

        QNetworkProxy Proxy;

        Proxy.setType(QNetworkProxy::HttpProxy);

        if (ProxyOptions.HostName.isEmpty()) {
            continue;
        } else {
            Proxy.setHostName(ProxyOptions.HostName);
        }

        if (ProxyOptions.Port > 0) {
            Proxy.setPort(ProxyOptions.Port);
        }

        if (!(ProxyOptions.UserName.isEmpty())) {
            Proxy.setUser(ProxyOptions.UserName);
        }

        if (!(ProxyOptions.Password.isEmpty())) {
            Proxy.setPassword(ProxyOptions.Password);
        }

        sProxyArr.append(Proxy);

    }

}

QNetworkProxy ProxyList::GetRandomProxy()
{
    //QString test;
    //test.setNum(sProxyArr.count());
    //test.setNum(0);
    //emit sigLog(test, false); //лог

    //return sProxyArr.at(qrand() % sProxyArr.count());

    //return QNetworkProxy();
    if(!sProxyArr.count()) return QNetworkProxy();
    return sProxyArr.at(qrand() % sProxyArr.count());
}

void ProxyList::LoadProxyListFromFile(QString FileName)
{
    QFile file(FileName);
    if (!file.exists())
    {
        file.open(QIODevice::Append | QIODevice::Text);
        file.write("");
        file.close();
    }
    QString ProxyLine;
    if (file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        sProxyOptionArr.clear();
        while ( !file.atEnd() )
        {
            ProxyLine = file.readLine();
            if (ProxyLine.isEmpty())
            {
                continue;
            }
            ProxyLine.replace("\n","");
            GProxyOptions ProxyOption;
            QStringList lst = ProxyLine.split("@", QString::SkipEmptyParts);
            if (lst.count() == 1)
            {
                lst = ProxyLine.split(":", QString::SkipEmptyParts);
                if (lst.count() == 1)
                {
                    //ми знайшли тільки адресу хост проксі
                    ProxyOption.HostName = ProxyLine;
                } else
                {
                    //ми знайшли хост-адресу і порт
                    ProxyOption.HostName = lst[0];
                    bool isInt;
                    int Port = lst[1].toInt(&isInt);
                    if (isInt)
                    {
                        ProxyOption.Port     = Port;
                    }
                }
            }
                else
            {
                QStringList LoginPass = lst[0].split(":", QString::SkipEmptyParts);
                if (LoginPass.count() == 1)
                {
                    //only login
                    ProxyOption.UserName = lst[0];
                } else
                {
                    ProxyOption.UserName = LoginPass[0];
                    ProxyOption.Password = LoginPass[1];
                }
                QStringList HostPort  = lst[1].split(":", QString::SkipEmptyParts);
                if (HostPort.count() == 1) {
                    //only login
                    ProxyOption.HostName = lst[1];
                } else
                {
                    ProxyOption.HostName = HostPort[0];
                    bool isInt;
                    int Port = HostPort[1].toInt(&isInt);
                    if (isInt)
                    {
                        ProxyOption.Port     = Port;
                    }
                }
            }
            sProxyOptionArr.append(ProxyOption);
        }

        file.close();
        LoadProxyArr();
    }

    emit SendProxyList(sProxyOptionArr);

}

void ProxyList::LoadProxyListFromSystemFile()
{
    QString ProgramPath = qApp->applicationDirPath();

    QDir dir(ProgramPath);
    if(!dir.exists("data"))
    {
        dir.mkdir("data");
    }
    dir.cd("data");

    LoadProxyListFromFile(dir.absoluteFilePath("Proxy.txt"));
}

void ProxyList::SaveProxyListToFile(QString FileName)
{
    if (FileName.isEmpty()) {

        QString ProgramPath = qApp->applicationDirPath();

        QDir dir(ProgramPath);
        if(!dir.exists("data"))
        {
            dir.mkdir("data");
        }
        dir.cd("data");

        FileName = dir.absoluteFilePath("Proxy.txt");
    }

    QFile file(FileName);
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream writestream(&file);

    for (int i = 0; i < sProxyOptionArr.count(); ++i) {

        if (sProxyOptionArr[i].HostName.isEmpty()) {
            continue;
        };

        QString ProxyOption = GetProxyString(sProxyOptionArr[i]);
        writestream << ProxyOption;
        writestream << "\n";

    }

    file.close();

}

QString ProxyList::GetProxyString(GProxyOptions ProxyOption)
{
    QString ResultString ="";

    if (!ProxyOption.UserName.isEmpty()
            && !ProxyOption.Password.isEmpty()) {

        ResultString = ProxyOption.UserName + ":" + ProxyOption.Password;

    } else if(!ProxyOption.UserName.isEmpty()){

        ResultString = ProxyOption.UserName;

    }

    if (!ResultString.isEmpty()) {
        ResultString = ResultString + "@";
    }

    if (!ProxyOption.HostName.isEmpty()
            && ProxyOption.Port != 0) {

        ResultString = ResultString + ProxyOption.HostName + ":" + QString::number(ProxyOption.Port);

    } else if(!ProxyOption.HostName.isEmpty()){

        ResultString = ResultString + ProxyOption.HostName;

    }

    return ResultString;
}

void ProxyList::UpdateProxyOptionArr(QVariant OptionValue, int OptionIndex, int Position)
{

    if (Position + 1> sProxyOptionArr.count()) {
        GProxyOptions ProxyOption;
        sProxyOptionArr.append(ProxyOption);
    }

    if (OptionIndex == 0) {
        sProxyOptionArr[Position].HostName = OptionValue.toString();
    } else if (OptionIndex == 1) {
        sProxyOptionArr[Position].Port = OptionValue.toInt();
    } else if (OptionIndex == 2) {
        sProxyOptionArr[Position].UserName = OptionValue.toString();
    } else {
        sProxyOptionArr[Position].Password = OptionValue.toString();
    }

    UpdateProxyArr(OptionValue,OptionIndex,Position);

}

void ProxyList::DeleteItemProxyOptionArr(int Position)
{
    sProxyOptionArr.removeAt(Position);
    sProxyArr.removeAt(Position);
}

void ProxyList::LoadProxyArr()
{

    sProxyArr.clear();

    foreach (GProxyOptions ProxyOptions, sProxyOptionArr) {

        QNetworkProxy Proxy;

        Proxy.setType(QNetworkProxy::HttpProxy);

        if (ProxyOptions.HostName.isEmpty()) {
            continue;
        } else {
            Proxy.setHostName(ProxyOptions.HostName);
        }

        if (ProxyOptions.Port > 0) {
            Proxy.setPort(ProxyOptions.Port);
        }

        if (!(ProxyOptions.UserName.isEmpty())) {
            Proxy.setUser(ProxyOptions.UserName);
        }

        if (!(ProxyOptions.Password.isEmpty())) {
            Proxy.setPassword(ProxyOptions.Password);
        }

        sProxyArr.append(Proxy);

    }

}

void ProxyList::UpdateProxyArr(QVariant ProxyValue, int OptionIndex, int Position)
{

    if (Position + 1> sProxyArr.count()) {
        QNetworkProxy Proxy;
        sProxyArr.append(Proxy);
    }

    if (OptionIndex == 0) {

        if (!ProxyValue.toString().isEmpty()) {
            sProxyArr[Position].setHostName(ProxyValue.toString());
        }

    } else if (OptionIndex == 1) {

        if (ProxyValue.toInt() > 0) {
            sProxyArr[Position].setPort(ProxyValue.toInt());
        }

    } else if (OptionIndex == 2) {

        if (!ProxyValue.toString().isEmpty()) {
            sProxyArr[Position].setUser(ProxyValue.toString());
        }

    } else {

        if (!ProxyValue.toString().isEmpty()) {
            sProxyArr[Position].setPassword(ProxyValue.toString());
        }

    }

}
