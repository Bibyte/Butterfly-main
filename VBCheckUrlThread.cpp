#include "VBCheckUrlThread.h"
//#include <QDebug>

VBCheckURLThread::VBCheckURLThread(QObject *parent) :
    QObject(parent)
{

}
// Тут ще локальний адрес страници треба для проверки її на пхп
VBCheckURLThread::VBCheckURLThread(const long& parID, const QUrl& parURL, const QUrl& parSaveURL, const VBFindProfile& parFPParam, bool parCollectEmails, QThread* parCurrentThread, QObject *parent):
    QObject(parent)
{
    // qDebug() << "Constructor start";
    sID = parID;
    sDomain = "";
    sManagerWasCreated = false;
    sCurrentThread = parCurrentThread;
    sURL = parURL;
    sSaveUrl = parSaveURL;
    sFPParam = parFPParam;
    checkForPHPData = cEmptyData;
    sCollectEmails = parCollectEmails;
    //// qDebug()<<"create";
    //manager = new QNetworkAccessManager(); // тут утечки памяти
    //manager->moveToThread(sCurrentThread);
    timer = new QTimer();
    if(timer == NULL)  timer = new QTimer();
    connect(this, SIGNAL(startTimer(int)),timer,SLOT(start(int)));
    connect(this, SIGNAL(stopTimer()),timer,SLOT(stop()));
    connect(timer, SIGNAL(timeout()), this, SLOT(checkWork()));
    timer->setInterval(30000); // 30 sec
    sDomain = sURL.toString();
    if(sDomain.left(7)!="http://" && sDomain.left(8)!="https://") sDomain = "http://" + sDomain;
    // qDebug() << "Constructor end";
}

VBCheckURLThread::~VBCheckURLThread()
{
    // qDebug() << "Destructor start";
    //timer->moveToThread(this);
    delete timer;
    manager->deleteLater();
    //delete manager;
    //// qDebug()<<"delete";
    // qDebug() << "Destructor end";
}

QThread* VBCheckURLThread::getCurrentThread()
{
    // qDebug() << "GetCurrentThread";
    return sCurrentThread;
}

void VBCheckURLThread::setParameters(const long& parID, const QUrl& parURL, const QUrl& parSaveURL, const VBFindProfile& parFPParam)
{
    // qDebug() << "setParameters start";
    sID = parID;
    sURL = parURL;
    sSaveUrl = parSaveURL;
    sDomain = sURL.toString();
    if(sDomain.left(7)!="http://" && sDomain.left(8)!="https://") sDomain = "http://" + sDomain;
    sFPParam = parFPParam;
    checkForPHPData = cEmptyData;
    // qDebug() << "setParameters end";
}

void VBCheckURLThread::replyDestroyed()
{
    // qDebug() << "replyDestroyed start";
    replyWasDeleted = true;
    // qDebug() << "replyDestroyed end";
}

void VBCheckURLThread::runChecking()
{
    // qDebug() << "runChecking start";
    //mtx.unlock();
    if(!sManagerWasCreated)
    {
        manager = new QNetworkAccessManager();
        sManagerWasCreated = true;
    }
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(checkSiteFinished(QNetworkReply*)));   // тут утечки памяти
    connect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this, SLOT(checkAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(checkSslErrors(QNetworkReply*,QList<QSslError>)));
    replyWasDeleted = false;

    QNetworkRequest request = QNetworkRequest();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html charset=utf-8");
    try
    {
        request.setUrl(QUrl(sDomain,QUrl::TolerantMode));
    }
    catch(int e)
    {
        checkWork();
        return;
    }
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute,QVariant(false));
    request.setRawHeader("User-Agent", cUserAgent);
    request.setRawHeader("charset", "utf-8");
    request.setRawHeader("Connection", "keep-alive");

    ////////////////////// Debugging
//    replyWasDeleted = true;
//    checkWork();
//    return;
    ///////////////////////////////

    try
    {
        reply = manager->get(request);                    //// qDebug()<<"4";   // тут утечки памяти
    }
    catch(int e)
    {
        checkWork();
        return;
    }


    connect(reply,SIGNAL(destroyed()),this,SLOT(replyDestroyed()));
    emit startTimer(50000);
    // qDebug() << "runChecking end";
}

void VBCheckURLThread::checkWork()
{
    // qDebug() << "checkWork start";
    if(!mtx.tryLock()) return;
    disconnect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(checkSiteFinished(QNetworkReply*)));   // тут утечки памяти
    disconnect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this, SLOT(checkAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    disconnect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(checkSslErrors(QNetworkReply*,QList<QSslError>)));
    emit stopTimer();
    if(!replyWasDeleted)
    {
        reply->abort();
        reply->deleteLater();
    }
    //manager->deleteLater();
    emit siteErrorSig(sID, sSaveUrl, tr("Site is not responsible: "));
    mtx.unlock();
    offNode();
    // qDebug() << "checkWork end";
}

void VBCheckURLThread::checkAuthenticationRequired(QNetworkReply*,QAuthenticator*)
{
    // qDebug() << "checkWork start";
    if(!mtx.tryLock()) return;
    disconnect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(checkSiteFinished(QNetworkReply*)));   // тут утечки памяти
    disconnect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this, SLOT(checkAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    disconnect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(checkSslErrors(QNetworkReply*,QList<QSslError>)));
    emit stopTimer();
    if(!replyWasDeleted)
    {
        reply->abort();
        reply->deleteLater();
    }
    //manager->deleteLater();
    emit siteErrorSig(sID, sSaveUrl, tr("Site requires authorization: "));
    mtx.unlock();
    offNode();
    // qDebug() << "checkWork end";
}

void VBCheckURLThread::checkSslErrors(QNetworkReply* reply,QList<QSslError> errList)
{
    // qDebug() << "checkWork start";
    if(!mtx.tryLock()) return;
    disconnect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(checkSiteFinished(QNetworkReply*)));   // тут утечки памяти
    disconnect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this, SLOT(checkAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    disconnect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(checkSslErrors(QNetworkReply*,QList<QSslError>)));
    emit stopTimer();
    if(!replyWasDeleted)
    {
        reply->abort();
        reply->deleteLater();
    }
    //manager->deleteLater();
    emit siteErrorSig(sID, sSaveUrl, tr("There are SSL errors while opening a website: "));
    mtx.unlock();
    offNode();
    // qDebug() << "checkWork end";
}


void VBCheckURLThread::checkSiteFinished(QNetworkReply* localReply)
{
    // qDebug() << "checkSiteFinished start";
    // Мютекс бо може з попереднім конфліктувати
  //  if(!mtx.tryLock()) return;
  //  emit stopTimer();
    //// qDebug()<<"finished begin";
    //    if(isFinished())
    //    {
    //        return;
    //    }


    QByteArray data;
    try
    {
        data = reply->read(reply->bytesAvailable());
    }
    catch(int e)
    {
        if(!mtx.tryLock()) return; // Зупинка
        emit stopTimer();

        //// qDebug()<<"error read";
        emit siteErrorSig(sID, sSaveUrl, tr("Reading data error: "));
        if(!replyWasDeleted)
        {
            reply->abort();
            reply->deleteLater();
        }
        //manager->deleteLater();
        mtx.unlock();
        offNode();
        return;
    }

    if(data.isEmpty()) // Якщо повернуло пусту то ждем наступну або буде тайм ліміт
    {
        return;
    }

    if(!mtx.tryLock()) return; // Зупинка
    emit stopTimer();

    if(!replyWasDeleted)
    {
        reply->abort();//// qDebug()<<"6";
        reply->deleteLater();
    }
    //manager->deleteLater();
//    if(data.isEmpty()) // Якщо повернуло пусту - то помилка
//    {
//        emit siteErrorSig(sSaveUrl, tr("The reply is empty: "));
//        mtx.unlock();
//        offNode();        //// qDebug()<<"finished exit data empty";
//        return;
//    }
    // Визначення кодування сторінки

    //QTextCodec *pageCodec = QTextCodec::codecForName(getCodePage(data));
    //
    if(checkForPHPData==cEmptyData) // Це перший запуск, значить перевіряємо на умови
    {
        bool result = true;                           // Тут починаємо перевіряти
        foreach(QStringList allAnd, sFPParam.sWhiteList) // Білий список
        {
            result = true;
            foreach(QString text, allAnd)
            {
                //QString temp = text;
                //QByteArray temp = pageCodec->fromUnicode(text);                  //// qDebug()<<"7";
                //// qDebug()<<"8";
                //qDebug() << data;
                if(data.indexOf(text) == -1)
                {
                    result = false;
                    break;
                }
            }
            if(result) break;
        }

        if(!result) // Якщо всі містять не виконалося то повертаємо і все
        {
            emit siteIsWrongSig(sID, sSaveUrl);
            mtx.unlock();
            offNode();    //// qDebug()<<"finished white list";
            return;
        }
        result = false;
        foreach(QStringList allAnd, sFPParam.sBlackList) // Чорний список
        {
            result = true;
            foreach(QString text, allAnd)
            {
                //QByteArray temp = pageCodec->fromUnicode(text);                  //// qDebug()<<"9";
                  //// qDebug()<<"10";
                ////// qDebug()<<temp;
                if(data.indexOf(text) == -1)
                {
                    result = false;
                    break;
                }
            }
            if(result) break;
        }
        //delete pageCodec;

        if(result) // Якщо усы умови чорного листа виконалися то повертаємо і все
        {
            emit siteIsWrongSig(sID, sSaveUrl);
            mtx.unlock();
            offNode();                      //// qDebug()<<"finished black list";
            return;
        }
    }
    if(sCollectEmails)
    {
       //data
//\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b
        QRegExp rx("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b");
        rx.indexIn(data);
        //QString s = "";
       // s.append(data);
        //rx.indexIn(s);
        //rx.indexIn("sdgdfgdg valery@bibyte.net dfgsdfg>vbi@ukr.net<fdg");
        QStringList list = rx.capturedTexts();

       //emit emailsFound(sID, list);
        foreach (QString email, list)
        {
            if(!email.isEmpty())
            {
                if(!email.contains(".png")) emit fragmentsFoundSig(sID, list);
            }

        }

    }
    else emit addSiteSig(sID, sSaveUrl);
    mtx.unlock();
    offNode();
    // qDebug() << "checkSiteFinished end";
}

void VBCheckURLThread::offNode(bool deleting)
{
    // qDebug() << "offNode start";
    emit stopTimer();
    disconnect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(checkSiteFinished(QNetworkReply*)));
    emit quitNode(this,deleting);
    // qDebug() << "offNode end";
}

QByteArray VBCheckURLThread::getCodePage(QByteArray data)
{
    QByteArray result = "UTF-8";
    long start = data.indexOf("<meta");
    if(start == -1) start = data.indexOf("< meta");
    if(start == -1) return result;
    QByteArray partData = data.mid(start);
    long end = partData.indexOf(">");
    if(end == -1) return result;
    partData = partData.mid(0,end);
    start = partData.indexOf("charset");
    if(start == -1) return result;
    partData = partData.mid(start+7);
    while(partData[0]==' ' || partData[0]=='=') partData = partData.mid(1);
    end = partData.indexOf('"');
    if(end == -1) end = partData.indexOf(";");
    if(end == -1) end = partData.indexOf("/");
    if(end != -1) partData = partData.mid(0,end);
    partData.replace("/","");
    partData.replace('"',"");
    partData.replace("=","");
    partData.replace(" ","");
    if(partData.isEmpty()) return result;
    partData = partData.toUpper();
    partData.replace("WINDOWS-","CP");
    partData.replace("WINDOWS","CP");
    return partData;
    // <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
}



