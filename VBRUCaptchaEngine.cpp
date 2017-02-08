#include "VBRUCaptchaEngine.h"
//#include <QDebug>

VBRUCaptchaEngine::VBRUCaptchaEngine(QObject *parent) :
    QObject(parent)
{
}

VBRUCaptchaEngine::VBRUCaptchaEngine(VBRUCapcha pCAPTCHAParams, QObject *parent)
{
    sCAPTCHAParams = pCAPTCHAParams;
    manager = new QNetworkAccessManager();
    managerGet = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(checkCAPTCHAFinished(QNetworkReply*)));   // тут утечки памяти
    connect(managerGet, SIGNAL(finished(QNetworkReply*)),this, SLOT(checkCAPTCHAAnswerFinished(QNetworkReply*)));
    dTimer = new QTimer(this);
    connect(dTimer, SIGNAL(timeout()),this, SLOT(checkCAPTCHAAnsver()));   // тут утечки памяти
}

VBRUCaptchaEngine::~VBRUCaptchaEngine()
{
    dTimer->stop();
    delete dTimer;
    manager->deleteLater();
    managerGet->deleteLater();
}

void VBRUCaptchaEngine::setKey(QString pCAPTCHAKey)
{
    sCAPTCHAKey = pCAPTCHAKey;
}

void VBRUCaptchaEngine::checkCAPTCHA(QByteArray pData, VBSearshEngineParameters params)
{
    seParams = params;
    QUrl url;
    QByteArray postData;

    url.setUrl("http://rucaptcha.com/in.php");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    postData.append("method").append("=").append("base64").append("&");
    postData.append("key").append("=").append(sCAPTCHAKey.toLatin1()).append("&");
    postData.append("soft_id").append("=").append("284").append("&");
    postData.append("phrase").append("=").append(QByteArray::number(sCAPTCHAParams.sPhrase)).append("&");
    postData.append("regsense").append("=").append(QByteArray::number(sCAPTCHAParams.sRegsense)).append("&");
    postData.append("numeric").append("=").append(QByteArray::number(sCAPTCHAParams.sNumeric)).append("&");
    postData.append("calc").append("=").append(QByteArray::number(sCAPTCHAParams.sCalc)).append("&");
    postData.append("min_len").append("=").append(QByteArray::number(sCAPTCHAParams.sMin_len)).append("&");
    postData.append("max_len").append("=").append(QByteArray::number(sCAPTCHAParams.sMax_len)).append("&");
    postData.append("language").append("=").append(QByteArray::number(sCAPTCHAParams.sLanguage)).append("&");
    postData.append("body").append("=").append(QUrl::toPercentEncoding("data:image/png;base64,"+pData.toBase64()));

    manager->post(request,postData);
}

void VBRUCaptchaEngine::checkCAPTCHAFinished(QNetworkReply* reply)
{
    QByteArray RCReply = reply->readAll();
    if(RCReply.contains("OK"))
    {
        sCAPTCHAID = RCReply.mid(3);
        dTimer->start(6000);
    }
    else if(RCReply.contains("IP_BANNED"))
    {
        emit captchaErrorResult(RCReply);
    }
    else if(RCReply.contains("ERROR"))
    {
        emit captchaErrorResult(RCReply);
    }
    else // Сайт не доступен
    {
        emit captchaErrorResult(RCReply);
    }
}

void VBRUCaptchaEngine::checkCAPTCHAAnsver()
{
    QNetworkRequest request = QNetworkRequest();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html charset=utf-8");
    request.setUrl(QUrl("http://rucaptcha.com/res.php?key="+sCAPTCHAKey+"&action=get&id="+sCAPTCHAID,QUrl::TolerantMode));
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute,QVariant(false));
    request.setRawHeader("User-Agent", "Butterfly 4.1");
    request.setRawHeader("charset", "utf-8");
    request.setRawHeader("Connection", "keep-alive");
    managerGet->get(request);
}

void VBRUCaptchaEngine::checkCAPTCHAAnswerFinished(QNetworkReply*reply)
{
    QByteArray RCReply = reply->readAll();
    if(RCReply.contains("OK"))
    {
        dTimer->stop();
        releaseCaptchaForm(RCReply.mid(3));
    }
    else if(RCReply.contains("ERROR"))
    {
        dTimer->stop();
        emit captchaErrorResult(RCReply);
    }
    else if(!RCReply.contains("CAPCHA_NOT_READY")) // Сайт не доступен
    {
        dTimer->stop();
        emit captchaErrorResult(RCReply);
    }
}

void VBRUCaptchaEngine::watchDog()
{
    emit captchaErrorResult("Anticaptcha server not responce.");
}

////////////////////////////////////////////////////////////////////////////////////////////// Utils


QByteArray VBRUCaptchaEngine::getCaptchaImage(QWebView* view,QString data, VBRUCapcha captchaParams)
{
    view->page()->setViewportSize(view->page()->mainFrame()->contentsSize());
    QWebElementCollection collection = view->page()->mainFrame()->findAllElements(captchaParams.sW3CaptchaSelector); //"IMG[class=b-captcha__image]"
    QWebElement el = QWebElement();
    foreach (QWebElement paraElement, collection) {
         if(paraElement.geometry().width() > 0 && paraElement.geometry().height() > 0)
         {
             el = paraElement;
             break;
         }
     }

    QImage image(el.geometry().width(), el.geometry().height(), QImage::Format_ARGB32);
    QPainter painter(&image);
    el.render(&painter);
    painter.end();
    QByteArray ba;
    QBuffer buffer( &ba );
    //QFile buffer("/home/valery/Робочий стіл/in.png");
    buffer.open(QIODevice::WriteOnly);
    image.save( &buffer, "PNG" );

//    image.save("file.png");
//    QFile imgFile("out.png");
//    imgFile.open(QFile::WriteOnly);
//    imgFile.write(ba);
//    imgFile.close();

    return ba;


}

void VBRUCaptchaEngine::releaseCaptchaForm(QString captchaValue)
{
    emit captchaCheckingResult(captchaValue);
}


