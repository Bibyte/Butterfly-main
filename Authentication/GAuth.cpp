#include "GAuth.h"

GAuth::GAuth(QObject *parent) :
    QObject(parent)
{
    dAuthManager = new QNetworkAccessManager;
}

GAuth::~GAuth()
{
    dAuthManager->deleteLater();
}

void GAuth::updateConnection(QString pLogin, QString pPass)
{
    //QHash<QString,QByteArray> encodedStruct = GBProtocol::encodeMessage(pLogin,pPass,GMac::GetMacAdress());
    //sDecodeKey = encodedStruct["key"];
    //QString req;
    //req.append(encodedStruct["msg"].toHex());
    //connect(dAuthManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(connectionUpdated(QNetworkReply*)));
    //QNetworkRequest request;
    //request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html charset=utf-8");
    //request.setUrl(QUrl("http://bibyte.net/Butterfly/access/?req="+req));
    //connect(&sWatchDogTimer,SIGNAL(timeout()),this,SLOT(connectionNotResponse())); // WatchDog
    //sWatchDogTimer.start(60000);
    //dAuthManager->get(request);
}

void GAuth::connectionUpdated(QNetworkReply* reply)
{
    sWatchDogTimer.stop();
    disconnect(&sWatchDogTimer,SIGNAL(timeout()),this,SLOT(connectionNotResponse()));
    disconnect(dAuthManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(connectionUpdated(QNetworkReply*)));
    QByteArray data = reply->read(reply->bytesAvailable());
    reply->deleteLater();
    //QHash<int,bool> result = GBProtocol::decodeMessage(QByteArray::fromHex(data),sDecodeKey);
    bool wasError = false, wasAbility = false;
    QString htmlBody="";
    for(int i=-5;i<5;i++)
    {  emit EnableService(i); continue;
//        if(wasError) break; // Якщо була хоч одна помилка - то далі не перебираєм
//        if(i==1)
//        {
//            htmlBody += getHTMLMessage(10000); // Пишемо, що усе добре
//        }
//        if(result.contains(i))
//        {

//            if(result[i])
//            {
//                if(i<=0) wasError = true;
//                if(i>0)
//                {
//                    // Послуга заблокована
//                    wasAbility = true;
//                    htmlBody += getHTMLServiceMessage(i, 3);
//                    emit DisableService(i); // Виключаємо послугу
//                }
//                htmlBody += getHTMLMessage(i);
//            }
//            else
//            {
//                if(i>0)
//                {
//                        // Послуга доступна
//                        wasAbility = true;
//                        htmlBody += getHTMLServiceMessage(i, 2);
//                        emit EnableService(i); // Включає послугу
//                }
//            }
//        }else
//        {
//            if(i>0)
//            {
//                    // Послуга недоступна
//                    htmlBody += getHTMLServiceMessage(i, 1);
//                    emit DisableService(i); // Виключаємо послугу
//            }
//        }
    }
    if(wasError)
    {
        emit setAuthStatus(false);
        emit HaveNoServices(true);
        //ui->tabWidget->setTabIcon(2,QIcon(":/Main/key_error.png"));
    }
    else
    {
        emit setAuthStatus(true);
        //ui->tabWidget->setTabIcon(2,QIcon(":/Main/key.png"));
        if(!wasAbility)
        {
            htmlBody += getHTMLMessage(10001); // У Вас ще нема послуг
            emit HaveNoServices(true);
        }
        else
        {
            emit HaveNoServices(false);
        }
        htmlBody += getHTMLMessage(10002); // Инфо где как услуги
    }
    emit sendHTMLBody(htmlBody,false);
}

void GAuth::connectionNotResponse()
{
    disconnect(dAuthManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(connectionUpdated(QNetworkReply*)));
    disconnect(&sWatchDogTimer,SIGNAL(timeout()),this,SLOT(connectionNotResponse()));
    emit sendHTMLBody(tr("<H1>Error while connecting to the server.</H1> <p>The server is not responding. Please try again later.</p>"),true);
}

QString GAuth::getHTMLMessage(int code)
{
    switch(code)
    {
    // Ошибки

        // Ошибка на стороне сайта
        // Неизвестная ошибка произошла на сайте.
        // Попробуйте пожалуйста подключится позже.
        case -4: return tr("<H1>Unknown error on site</H1> "\
                           "<p>An unknown error has occurred on the site.</p>"\
                           "<p>Please try again later.</p>");

        // Ошибка аутентификации.
        //Возможно Вы впервые запустили программу с другого компьютера.
        //Чтобы использовать программу и иметь доступ к своим услугам, нужно подтвердить использование программы на новом компьютере.
        //Эта ошибка может возникать и в других случаях.
        //Вам на электронный адрес, который Вы указали при регистрации, отправлено письмо с ссылкой-подтверждением. Пожалуйста, перейдите по ссылке, указанной в письме.
        case -3: return tr("<H1>Authentication failed</H1> "\
                           "<p>Maybe you run the program from another computer first time.</p>"\
                           "<p>To use the program and have access to its services, you need to confirm the use of the software on the new computer.</p>"\
                           "<p>This error can also occur in other cases.</p>"\
                           "<p>At your email address, that you provided at registration, sent an email with confirmation link.</p>"\
                           "<p>Please <b>click on the confirmation link in the email</b>.</p>");

        //Доступ запрещен.
        //Ваша учетная запись была заблокирована.
        //Для решения этой проблемы, пожалуйста,
        //свяжитесь с бибйте (инфо@bibyte.net)
        case -2: return tr("<H1>Access denied<H1>"\
                           "<p>Your account has been locked.</p>"\
                           "<p>To solve this problem, please contact bibyte (info@bibyte.net)</p>");

        //Ошибка авторизации.
        //Пользователь и пароль не зарегистрирован на сайте.
        //Возможно Вы ошиблись при вводе логина и пароля. Проверьте раскладку клавиатуры, Caps Look и попробуйте еще раз.
        //Если Вы еще не зарегистрированы, пожалуйста, пройдите регистрацию здесь: http://www.bibyte.net/ru/user/register
        case -1: return tr("<H1>Authentication failed</H1>"\
                           "<p>User and password are not found on the site.</p>"\
                           "<p>Maybe you made a mistake when entering the username and/or password. Check the keyboard, caps lolk and try again.</p>"\
                           "<p>If you have not yet registered, please register here: <b>http://www.bibyte.net/ru/user/register</b></p>");

        // // Ошибка декодирования ответа сайта. Пытаются взломать
        //Ошибка соединения
        //Не удалось соединится с сайтом. Пожалуйста, обратитесь в службу поддержки: info@bibyte.net
        case 0: return tr("<H1>Connection Error</H1>"\
                          "<p>Unable to connect to the site. Please contact support: <b>info@bibyte.net</b></p>");
    // Служебные сообщения
        // Ваши услуги
        case 10000: return tr("<H1>Your Services</H1>");

        // У Вас еще нет дополнительных услуг
        case 10001: return tr("<p><b>You don't have additional services.</b></p>");

        // Для того чтобы заказать дополнительную услугу, отправьте письмо с просьбой заказа услуги на info@bibyte.net.
        //В тему письма напишите "ЗАКАЗ УСЛУГИ"
        //В теле письма укажите код и название услуги.
        //Только после того, как получите ответ о подтверждении, переведите стоимость услуги на кошелек Z246211331762, указав в описании операции код и название услуги.
        //Стоимость дополнительных услуг и детальную информацию об их получении смотрите на bibyte.net
        case 10002: return tr("<hr><p>To order the service send a letter on <b>info@bibyte.net</b>.</p>"\
                              "<p>In the subject line write ""<b>ORDER OF SERVICE</b>""</p>"\
                              "<p>In the body enter code and name of service.</p>"\
                              "<p>Only after email confirmation will get an answer, pay the cost of the service to the WM <b>Z246211331762</b>, specifying the description of the code and the name of the service.</p>"\
                              "<hr><p>Cost of services and detailed information about how to obtain additional services, see <b>bibyte.net</b>.</p>");

    case 10003: return tr("<p><span style = 'color:[COLOR];'>([STATUS])</span>");

        // Услуги



    default: return "";
    }
    return "";
}

QString GAuth::getHTMLServiceMessage(int code, int satus) // 1 - недоступно, 2 - доступно, 3 - блоковано
{
    QString msg = "<p><img src='[IMG]'> <span style = 'color:[COLOR];'><b>[MESSAGE] ([STATUS])</b></span></p>";

    switch(code)
    {
        case 1: // Отмена ограничения количества параллельных процессов в 32 потока
            msg = msg.replace("[MESSAGE]",tr("Cancel limiting the number of parallel processes in 32 threads"));
        break;
        case 2:
            msg = msg.replace("[MESSAGE]",tr("Using a proxy"));
        break;
        case 3:
            msg = msg.replace("[MESSAGE]",tr("Using Anti CAPTCHA"));
        break;
        default: return "";
    }

    switch(satus)
    {
        case 1:
            msg = msg.replace("[COLOR]","gray");
            msg = msg.replace("[STATUS]",tr("Not available"));
            msg = msg.replace("[IMG]",":/Main/Denied-ability-small.png");
        break;
        case 2:
            msg = msg.replace("[COLOR]","green");
            msg = msg.replace("[STATUS]",tr("Available"));
            msg = msg.replace("[IMG]",":/Main/Avalaible-ability-small.png");
        break;
        case 3:
            msg = msg.replace("[COLOR]","red");
            msg = msg.replace("[STATUS]",tr("Blocked"));
            msg = msg.replace("[IMG]",":/Main/Blocked-ability-small.png");
        break;
        default:
            msg = msg.replace("[COLOR]","gray");
            msg = msg.replace("[STATUS]",tr("Not available"));
            msg = msg.replace("[IMG]",":/Main/Denied-ability-small.png");
    }

    return msg;
}
