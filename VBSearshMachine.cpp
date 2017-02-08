#include "VBSearshMachine.h"
#include <QDebug>

VBSearshMachine::VBSearshMachine(QObject *parent) :
    QObject(parent)
{

}

VBSearshMachine::VBSearshMachine(const VBSearshEngineParameters& pSEParams, const VBFindProfile& pFPParams, QObject *parent) :
    QObject(parent)
{
    //thread()->setPriority(QThread::LowestPriority);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(checkWork()));
    //connect(this,SIGNAL(resultSearshed(QString,bool)),this,SLOT(extSite(QString,bool)));
    connect(this,SIGNAL(parseNextUrlSig(QString&,bool)),this,SLOT(parseNextUrl(QString&,bool)));
    sSEParams = pSEParams;
    sFPParams = pFPParams;
    currSite=0;
    currSiteGlobal = 0;
    currSiteGlobalReturn = 1;
    currPage=0;
    isActive = false;
    isEnd = false;
    countThreads = 0;
    QTime randTime = QDateTime::currentDateTime().time();
    qsrand(randTime.hour()+randTime.minute()+randTime.second());

    saveSearshData = "";

    // Proxy
   // if(sUseProxy)
   // {
        dProxylist = new ProxyList();
        //dProxylist->LoadProxyListFromSystemFile();
        connect(dProxylist,SIGNAL(sigLog(QString,bool)),this,SLOT(writeLog(QString,bool)));
    //}
    // /Proxy

    //initBrowser();

    // Подготовим sPimaryFilter
    QString tempSearshMachineBrand = sSEParams.sSearshMachineBrand;
    while(!tempSearshMachineBrand.isEmpty())
    {
        sPimaryFilter.append(getNextKeyword(&tempSearshMachineBrand));
    }

    sPOSTSessionEngine = new VBPOSTSessionEngine(this);

    // Подготовим sPOSTSessionEngine
    tempSearshMachineBrand = sSEParams.sSessionParams;
    while(!tempSearshMachineBrand.isEmpty())
    {
        sPOSTSessionEngine->addParam(getNextKeyword(&tempSearshMachineBrand));
    }
    //if(sUseCaptcha)
    //{
        dCaptchaEngine = new VBRUCaptchaEngine(sSEParams.sCaptchaParams,this);
        connect(dCaptchaEngine,SIGNAL(captchaCheckingResult(QString)),this,SLOT(captchaCheckingResult(QString)));
        connect(dCaptchaEngine,SIGNAL(captchaErrorResult(QByteArray)),this,SLOT(captchaErrorResult(QByteArray)));
        //void captchaCheckingResult(QByteArray result);
        //void captchaErrorResult(QByteArray result);
   // }

    initBrowser();
}

VBSearshMachine::~VBSearshMachine()
{
    // disconnect all?
    delete sPOSTSessionEngine;
    delete timer;
    delete dBrowser;
    delete dProxylist;
    //if(sUseCaptcha)
    //{
        delete dCaptchaEngine;
    //}
}

void VBSearshMachine::initBrowser(bool pDeleteOld)
{
    if(pDeleteOld) delete dBrowser;
    dBrowser = new QWebView();
    connect(dBrowser,SIGNAL(loadStarted()),this,SLOT(hideBrovserAndContinueWork()));
    connect(dBrowser,SIGNAL(urlChanged(const QUrl&)), this, SLOT(OnUrlChanged(const QUrl&)));
    dBrowser->settings()->setAttribute(QWebSettings::AutoLoadImages,false);
    dBrowser->settings()->setAttribute(QWebSettings::JavascriptEnabled,true);
    //dBrowser->show();
    //++Proxy include
    QNetworkAccessManager* NewAccessManager = dBrowser->page()->networkAccessManager();

    NewAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    dBrowser->page()->setNetworkAccessManager(NewAccessManager);

}

///////////////////********************************** ПРОВЕРКА ***********************************************

void VBSearshMachine::checkWork()
{
    disconnect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));
    dBrowser->stop();
    if(isActive) // Чогось після паузи срабатує знову
    {
        if(dBrowser->page()->mainFrame()->toHtml().size() > 100)
        {
            previousSearchedData = cEmptyData;
            searshFinished(true);
        }
        else
        {
            timer->stop();
            programPauseSearch(tr("Search engine does not respond: Can not start / continue the search!"));
        }
    }
    else
    {
        timer->stop();
    }
}

///////////////////********************************** ПРОВЕРКА ***********************************************


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// ОСНОВНОЙ ЦИКЛ РАБОТЫ ПРОГРАММЫ //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void VBSearshMachine::searshResult(QString& parSearshText, long parCountResults, long parMaxParallelThreads, bool parUseFilter, bool parCheckOnlyMode) /////////////////// Старт пошуку
void VBSearshMachine::searshResult(GSearchParams params) /////////////////// Старт пошуку
{
    isActive = true;
    sCheckOnlyMode = params.sCheckOnlyMode; // Якщо тру, то parSearshText має містити список урлів - перевірити
    sCountResults = params.sParCountResults;
    sMaxParallelThreads = params.sParMaxParallelThreads;
    sUseFilter = params.sUseFilter;
    sUseProxy = params.sUseProxy;
    sUseCaptcha = params.sUseCaptcha;
    if(sUseCaptcha) dCaptchaEngine->setKey(params.sCaptchaKey);
    sProxyCheckingTime = params.sProxyCheckingTime;
    sCollectEmails = params.sCollectEmails;
    if(sCheckOnlyMode)
    {
        sSearshText = "";
        emit sigStatus(tr("Checking links..."));
        writeLog(tr("Checking links was started!"),false);
        parseNextUrl(params.sParSearshText);
    }
    else
    {
        dProxylist->LoadProxyListFromSystemFile();
        sSearshText = params.sParSearshText;
        currPage = sSEParams.sStartPage - sSEParams.sPageStep; // Навіть перед першим пошуком збільшує крок
        if(params.sCurrPage > currPage) currPage = params.sCurrPage;
        writeLog(tr("New search was started!"),false);
        sCurrFindKeyword = getNextKeyword(&sSearshText);
        //qDebug() << sCurrFindKeyword;
        //mtx.lock();
        emit sigStatus(tr("Search: ")+sCurrFindKeyword);
        //mtx.unlock();
        writeLog(tr("Search: ")+sCurrFindKeyword,false);
        emit sigStatus(tr("Parsing..."));
        movingToNextPage(false);
    }
}

void VBSearshMachine::movingToNextPage(bool nextPage)////////////////// MOVING ///////////////// Загружаэмо чергову сторінку з результатами пошуку
{
    if(sCurrFindKeyword.isEmpty())
    {
        isEnd = true;
        if(!countThreads)
        {
            //mtx.lock();
            emit stopSearshSigThreads(); // Удалить треды
            emit endSearshSig();
            //mtx.unlock();
            writeLog(tr("The search was stopped!"),true);
            return;
        }
        QTimer::singleShot(getWaitTime(),this,SLOT(movingToNextPage()));
        return;
    }

    if(nextPage)
    {
        currPage+=sSEParams.sPageStep;
        emit currPageSig(currPage);
    }

    QString searshData = sSEParams.sSearshHost;
    searshData.replace("[q]",sCurrFindKeyword);
    searshData.replace("[p]",QString::number(currPage));
    searshData += sPOSTSessionEngine->getParamString();
    //qDebug() << searshData;
    //writeLog(searshData,false);

    try
    {
        if(previousSearchedData != cEmptyData)
        {
            previousSearchedData = dBrowser->page()->mainFrame()->toHtml();
        }
        else
        {
            previousSearchedData = cEmptyData;
        }
    }
    catch(int e)
    {
        previousSearchedData = cEmptyData;
    }


    //initBrowser(true);
    if(sUseProxy)
    {

        //sProxy = dProxylist->GetRandomProxy();
        sProxy = dProxylist->GetRandomProxy();
        //writeLog(sProxy);
        while(!GProxyTest::checkProxy(sProxy.hostName(),sProxy.port(),sProxyCheckingTime))
        {
            writeLog(tr("Failed checking proxy: ") + sProxy.hostName()+":"+sProxy.port(),false);
            sProxy = dProxylist->GetRandomProxy();
        }
        QNetworkAccessManager* NewAccessManager = dBrowser->page()->networkAccessManager();
        NewAccessManager->setProxy(sProxy);
        NewAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
        dBrowser->page()->setNetworkAccessManager(NewAccessManager);
    }

    connect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));

    QNetworkRequest request = QNetworkRequest();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html charset=utf-8");
    request.setUrl(QUrl(searshData,QUrl::TolerantMode));
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute,QVariant(false));
    request.setRawHeader("User-Agent", cUserAgent);
    request.setRawHeader("charset", "utf-8");
    request.setRawHeader("Connection", "keep-alive");

    //initBrowser(true);
    dBrowser->stop();
    OnUrlChanged(QUrl(searshData,QUrl::TolerantMode));
    dBrowser->load(request);
    timer->start(120000); // 2 мин
}

void VBSearshMachine::searshFinished(bool normal) /////////////// SEARCH FINISHED ///////////////////////////////// Пошук завершено. Перевіряємо чи нармальні резулти
{
    if(!normal)
    {
        //writeLog("ne normal");
        return;
    }
    timer->stop();
    disconnect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));
    dBrowser->stop();
    sPOSTSessionEngine->writeParams(dBrowser->url().toString());
    QString webData = "";
    try
    {
        webData = dBrowser->page()->mainFrame()->toHtml();
        webData = webData.replace("%3a%2f%2f","://");
        webData = webData.replace("%3A%2F%2F","://");
        webData = webData.replace("http"," http");
    }
    catch(int e)
    {
        movingToNextPage(false);
    }

    emit sendHTMLList(webData);
    if(webData == previousSearchedData) // Поточна і попередня сторінка - ідентичні - значить ставим паузу на пошук - наверно банять :)
    {
        programPauseSearch(tr("Failed to load search results! Perhaps you banned by search engine. Wait some time and continue."));
        return;
    }
    // Перевірка, чи дійсно отриманий текст - текст з посиланнями
    if(!webData.size())
    {
        programPauseSearch(tr("Failed to connect! Unable to start / continue the search!"));
        return;
    }

    bool isLastPage = false;
    if(webData.contains(sSEParams.sLastPageCheck) == sSEParams.sLastPageCheckBox && !sSEParams.sLastPageCheck.isEmpty())// або макс количество нужно!!!!!!!!!!!!!!!!!!!!!!! // Якщо це остання сторінка
    {
        isLastPage = true;
    }
    if(webData.contains(sSEParams.sSpamCheck) == sSEParams.sSpamCheckBox && !sSEParams.sSpamCheck.isEmpty()) // Якщо це спам
    {
        if(dBrowser->settings()->testAttribute(QWebSettings::AutoLoadImages))
        {
            dBrowser->settings()->setAttribute(QWebSettings::AutoLoadImages,false);
            if(sUseCaptcha)
            {
                QByteArray captchaData = VBRUCaptchaEngine::getCaptchaImage(dBrowser,webData,sSEParams.sCaptchaParams);
                if(captchaData.isEmpty()) programPauseSearch(tr("Captcha selector is not correct. Please check it!"));
                else
                {
                    dCaptchaEngine->checkCAPTCHA(captchaData,sSEParams);
                    emit sigStatus(tr("Captcha recived. Recognizing..."));
                    //writeLog(tr("Captcha recived. Recognizing..."),false);
                    //QString preparedForm = VBRUCaptchaEngine::getPreparedCaptchaForm(data,captchaData);

                    ////////////////////////////////////// отправляем капчу
                }
            } else programPauseSearch(tr("Service is temporarily unavailable! Please try later!"));
        }
        else
        {
            dBrowser->settings()->setAttribute(QWebSettings::AutoLoadImages,true);
            if(isEnd) return;
            movingToNextPage(false);
        }
        return;
    }
    if(webData.contains(sSEParams.sNoResultsCheck) == sSEParams.sNoResultsCheckBox && !sSEParams.sNoResultsCheck.isEmpty()) // Якщо нема результатів
    {
        isLastPage = true;
    }
    //currSite = 0;
    emit parseNextUrlSig(webData, isLastPage); // Відправляє текст з ссилками для перевірки сайтів. Тепер треба парсити ссилки
    //extSite(data); // тут парситься текст і відправляються паралельні процеси
}

void VBSearshMachine::parseNextUrl(QString& searshData, bool isLastPage) ///////////////////////////////////////////////////// Пошук наступної ссилки
{
    if(!isActive)
    {
        sCurrentSearshData = searshData;
        return;
    }
    //qDebug() << "Count threads: " << countThreads;
    if(countThreads>=sMaxParallelThreads)// Параметр
    {
        saveSearshData = searshData;
        saveIsLastPage = isLastPage;
        QTimer::singleShot(500,this,SLOT(continueParseNextUrl())); //0,5 sec
        return;
    }
    if(currSite>=sCountResults && !sCheckOnlyMode)
    {
        sCurrFindKeyword = getNextKeyword(&sSearshText);
        emit sigStatus(tr("Search: ")+sCurrFindKeyword);
        writeLog(tr("It was iterated maximum number of links."),false);
        if(sCurrFindKeyword.isEmpty())
        {
            writeLog(tr("The search is over."),false);
        }else
        {
            writeLog(tr("Search: ")+sCurrFindKeyword,false);
        }
        if(isEnd) return;
        QTimer::singleShot(getWaitTime(),this,SLOT(movingToNextPage()));
        return;
    }

    QString partData = takeNextURL(&searshData);
    if(partData.isEmpty())
    {
        if(isLastPage)
        {
            writeLog(tr("It was reached the last page of results on the keyword."),false);
            sCurrFindKeyword = getNextKeyword(&sSearshText);
            if(sCurrFindKeyword.isEmpty())
            {
                emit sigStatus(tr("The search is over."));
                writeLog(tr("The search is over."),false);
            }else
            {
                emit sigStatus(tr("Search: ")+sCurrFindKeyword);
                writeLog(tr("Search: ")+sCurrFindKeyword,false);
            }
        }
        if(isEnd) return;
        QTimer::singleShot(getWaitTime(),this,SLOT(movingToNextPage()));
        return;
    }
    else if(partData.isNull())
    {
        emit parseNextUrlSig(searshData,isLastPage);
        return;
    }

//    // Обробка локальних урлів від поисковика (Зроблено вище)
//    if(partData[0]=='/')
//    {
//        partData = QUrl(sSEParams.sSearshHost).host() + partData;//////////////////////////////////////////////////////////*****************************/////////////////////////////////////////////////////////////////
//    }

    // in not casche
    if(!sPimaryFilter.isEmpty())
    {
        foreach(QString filterElement, sPimaryFilter)
            if(partData.contains(filterElement) && !filterElement.isEmpty())
            {
                emit parseNextUrlSig(searshData,isLastPage);
                return;
            };
    };

    // Якщо це закешована сторінка
    if(partData.size()<4)
    {
        emit parseNextUrlSig(searshData,isLastPage);
        return;
    }; // Якщо ссилка непонятна

    // Якщо в файліку є сайти без http:// то приєднувати http://
    if(sCheckOnlyMode && partData.left(4).toLower()!="http")
    {
        partData = "http://" + partData;
    }
    QUrl convUrl;

    try
    {
        //writeLog("*"+partData+"*",false);
        convUrl = QUrl(partData,QUrl::TolerantMode);

    }
    catch(int e)
    {
        emit parseNextUrlSig(searshData,isLastPage);
        return;
    }

    if(!convUrl.host().length())
    {
        emit parseNextUrlSig(searshData,isLastPage);
        return;
    }; // Якщо хост непонятний (локальний урл поисковика наприклад)

    // Створюю окрему змінну URL для збереження
    QUrl saveURL = convUrl;

    // Тут формуємо сторінку, яку будем парсити
    if(sFPParams.sCheckSpecificURL)
    {
        QString tmpHost = convUrl.scheme()+"://"+convUrl.host();
        //convUrl.setHost(convUrl.host()+sFPParams.sSpecificURL);
        convUrl.setUrl(tmpHost + sFPParams.sSpecificURL);
        //convUrl.setScheme(saveURL.scheme());
    }

    // Перевіряємо, чи останній урл нам траплявся останніх 3 рази
    if(sLastUrls.contains(convUrl))
    {
        emit parseNextUrlSig(searshData,isLastPage);
        return;
    }
    sLastUrls.append(convUrl);
    if(sLastUrls.count()>3)
    {
        sLastUrls.takeFirst();
    }
    //qDebug()<<url.host();

    currSite++;
    currSiteGlobal++;

    if(!sUseFilter)
    {
         emit siteExecSig();
         emit addSiteSig(convUrl);
         emit parseNextUrlSig(searshData,isLastPage);
         //QTimer::singleShot(100,this,parseNextUrl(searshData,isLastPage));
         return;
    }


    //////////////////////////////////////////////////////////////////////////////////////////// Перевірка урла завершена. Стартуєм перевірку сторінки

    // Пишем в лог то что проверяем, пишем перед мутексом бо сам лог використовує мутекси
    //if(!sAvalaibleThreads.count())
    //{
    //    writeLog(tr("Checking: ")+url.toString(),false);
    //}
    mtx.lock();
    QThread* thread;
    VBCheckURLThread* siteChecker;

    // QString tempUrl = url.toString();
    // QString tempSaveUrl = saveURL.toString();

    if(!sAvalaibleThreads.count())
    {
        thread = new QThread(this);
        if(thread == NULL)
        {
            writeLog("Pizdets thread null",false);
            emit parseNextUrlSig(searshData,isLastPage);
            return;
        }
        siteChecker = new VBCheckURLThread(currSiteGlobal, convUrl, saveURL, sFPParams,sCollectEmails, thread);
        if(siteChecker == NULL)
        {
            emit parseNextUrlSig(searshData,isLastPage);
            return;
        }
        siteChecker->moveToThread(thread);
        thread->start(QThread::LowPriority);
        connect(siteChecker,SIGNAL(quitNode(VBCheckURLThread*,bool)),this,SLOT(offClosedThread(VBCheckURLThread*,bool)));
        connect(siteChecker,SIGNAL(addSiteSig(const long&, const QUrl&)),this,SLOT(addSite(const long&, const QUrl&)));
        connect(siteChecker,SIGNAL(fragmentsFoundSig(const long&, const QStringList&)),this,SLOT(fragmentsFound(const long&, const QStringList&)));
        connect(siteChecker,SIGNAL(siteIsWrongSig(const long&, const QUrl&)),this,SLOT(siteIsWrong(const long&, const QUrl&)));
        connect(siteChecker,SIGNAL(siteErrorSig(const long&, const QUrl&, const QString&)),this,SLOT(siteError(const long&, const QUrl&, const QString&)));
        connect(this,SIGNAL(stopSearshSigThreads(bool)),siteChecker,SLOT(offNode(bool)));
    }
    else
    {
        siteChecker = sAvalaibleThreads.pop();
        siteChecker->setParameters(currSiteGlobal, convUrl, saveURL, sFPParams);
        //thread = siteChecker->getCurrentThread();
    }

    try
    {
    connect(this,SIGNAL(checkSite()),siteChecker,SLOT(runChecking()));
    emit siteExecSig();
    emit checkSite();
    disconnect(this,SIGNAL(checkSite()),siteChecker,SLOT(runChecking()));
    mtx.unlock();
    //thread->start();
    //mtx.lock();
    countThreads++;
    //mtx.unlock();
    }
    catch(int e){};
    emit parseNextUrlSig(searshData,isLastPage);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// КІНЕЦЬ ОСНОВНОГО ЦИКЛУ /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VBSearshMachine::offClosedThread(VBCheckURLThread* objectToDelete, bool deleting)
{
    if(deleting)
    {
        QThread* thread = objectToDelete->getCurrentThread();
        thread->quit();
        thread->wait();
        objectToDelete->deleteLater();
        thread->deleteLater();
    }
    else
    {
        mtx.lock();
        sAvalaibleThreads.push(objectToDelete);
        mtx.unlock();
    }
    countThreads--;
}

QString VBSearshMachine::takeNextURL(QString* searshData)
{
    long nextIndex = searshData->indexOf("http");
    if(nextIndex==-1 && !sCheckOnlyMode)
    {
        return "";
    }
    *searshData = searshData->mid(nextIndex);
    nextIndex = searshData->indexOf(" ");
    QString partData = "";
    if(nextIndex==-1)
    {
        partData = searshData->toLower();
        *searshData = "";
    }else
    {
        partData = searshData->left(nextIndex).toLower();
        *searshData = searshData->mid(nextIndex);
    }

// Локальні ссилки не працюють

        bool isRealLink = true;



        if(partData.isEmpty() || partData.length() < 4)
        {
            return takeNextURL(searshData); // Якщо в хрефі ссилка порожня
        }

        if(!isRealLink)// Якщо в хрефі ссилка на сам поисковик с редиректом
        {
            if(partData.contains("&")) partData.truncate(partData.indexOf("&"));
            if(partData.contains("%2b")) partData.truncate(partData.indexOf("%2b"));// Эта штука может удалять лишние плюсы и отсекать важную информацию
            if(partData.contains("+")) partData.truncate(partData.indexOf("+"));
        }
        if(partData.contains("&amp;")) partData.truncate(partData.indexOf("&amp;"));
        if(partData.contains("%252b%25d0%25ba")) partData.truncate(partData.indexOf("%252b%25d0%25ba"));

    unsigned short percentDeleterIteratior = 5;
    QString previousPartData = partData;
    while(partData.contains("%") && percentDeleterIteratior-- && !partData.contains("??") && !partData.contains("%E") && !partData.contains("%F")) // Делаем до 5 итераций преобраазования %-структур ссылок
    {
        QByteArray b = "";
        b += partData;
        previousPartData = partData;
        partData = QUrl::fromEncoded(b).toString();
        //partData = QUrl::fromEncoded(b).path(QUrl::FullyDecoded);
        if(partData.isEmpty()) partData = "Error";
    }
    if(partData.contains("??")) // Эсли есть знаки вопроса повторяющиеся - переборщили с итерациями преобразования
    {
        partData = previousPartData;
    }
    return partData;
}

void VBSearshMachine::continueParseNextUrl()
{
    emit parseNextUrlSig(saveSearshData,saveIsLastPage);
}

QString VBSearshMachine::getNextKeyword(QString* SearshText)
{
    QString findValue = *SearshText;
    if(findValue.isEmpty())
    {
        return "";
    }
    if(findValue.contains(","))
    {
        SearshText->clear();
        SearshText->append(findValue.mid(findValue.indexOf(",")+1));
        findValue.truncate(findValue.indexOf(","));
    }
    else SearshText->clear();
    findValue = findValue.simplified();
    if(findValue.isEmpty())
    {
        return getNextKeyword(SearshText);
    }
    //mtx.lock();
    currSite=0;
    currPage=sSEParams.sStartPage;
    //mtx.unlock();
    findValue.truncate(QImage(":/Main/test.gif").size().width()); // Promote protection level 2
    return findValue;
}

int VBSearshMachine::getWaitTime()
{
    int A = sSEParams.sMinWait*1000;
    int B = sSEParams.sMaxWait*1000;
    return qrand()%(B-A+1)+A;
}

void VBSearshMachine::siteIsWrong(const long& id, const QUrl &url)
{
    mtx.lock();
    if(currSiteGlobalReturn == id || true) // Пока отключим функцию сортировки (чтобы включить - удалить тру)
    {
        emit siteWrongSig(url);
        currSiteGlobalReturn++;
    }else
    {
        sURLBuffer[id] = GResultContainer(url,2);
        execUrlBuffer();
    }
    mtx.unlock();
    if(!countThreads && isEnd)
    {
        writeLog(tr("The search was finished!"),true);
        mtx.lock();
        emit endSearshSig();
        mtx.unlock();
    }
}

void VBSearshMachine::siteError(const long& id, const QUrl &url, const QString &msg)
{
    mtx.lock();
    ////////////////////////////////////////////////////////////////////////////// перевірка на порядок ДОРОБИТИ!!!!!!!!!!!!!!!!!
    if(currSiteGlobalReturn == id || true) // Пока отключим функцию сортировки (чтобы включить - удалить тру)
    {
        emit siteErrorSig(url, msg);
        currSiteGlobalReturn++;
    }else
    {
        sURLBuffer[id] = GResultContainer(url,3,msg);
        execUrlBuffer();
    }
    mtx.unlock();
    if(!countThreads && isEnd)
    {
        writeLog(tr("The search was finished!"),true);
        mtx.lock();
        emit endSearshSig();
        mtx.unlock();
    }
}

void VBSearshMachine::addSite(const long& id, const QUrl &url)
{
    mtx.lock();
    ////////////////////////////////////////////////////////////////////////////// перевірка на порядок ДОРОБИТИ!!!!!!!!!!!!!!!!!
    if(currSiteGlobalReturn == id || true) // Пока отключим функцию сортировки (чтобы включить - удалить тру)
    {
        emit addSiteSig(url);
        currSiteGlobalReturn++;
    }else
    {
        sURLBuffer[id] = GResultContainer(url,1);
        execUrlBuffer();
    }

    mtx.unlock();
    if(!countThreads && isEnd)
    {
        writeLog(tr("The search was finished!"),true);
        mtx.lock();
        emit endSearshSig();
        mtx.unlock();
    }
}

void VBSearshMachine::fragmentsFound(const long &id, const QStringList &list)
{
    mtx.lock();
    ////////////////////////////////////////////////////////////////////////////// перевірка на порядок ДОРОБИТИ!!!!!!!!!!!!!!!!!
    emit fragmentsFoundSig(list);
    currSiteGlobalReturn++;


    mtx.unlock();
    if(!countThreads && isEnd)
    {
        writeLog(tr("The search was finished!"),true);
        mtx.lock();
        emit endSearshSig();
        mtx.unlock();
    }
}

void VBSearshMachine::writeLog(const QString &message, bool showMessage)
{
    mtx.lock();
    emit sigLog(message,showMessage);
    mtx.unlock();
}

void VBSearshMachine::execUrlBuffer()
{
    while(sURLBuffer.contains(currSiteGlobalReturn))
    {
        GResultContainer c = sURLBuffer[currSiteGlobalReturn];
        switch(c.type)
        {
            case 1: emit addSiteSig(c.url); break;
            case 2: emit siteWrongSig(c.url); break;
            case 3: emit siteErrorSig(c.url, c.msg); break;
        }

        sURLBuffer.remove(currSiteGlobalReturn);
        currSiteGlobalReturn++;
    }
}

void VBSearshMachine::stopSearsh()
{
    timer->stop();
    //mtx.lock();
    emit stopSearshSigThreads(true);
    disconnect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));
    dBrowser->stop();
    isEnd = true;
    //mtx.unlock();
}

void VBSearshMachine::pauseSearsh()
{
    timer->stop();
    //mtx.lock();
    isActive = false;
    disconnect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));
    disconnect(dBrowser,SIGNAL(loadStarted()),this,SLOT(hideBrovserAndContinueWork()));
    dBrowser->stop();
    //mtx.unlock();
}

void VBSearshMachine::programPauseSearch(const QString &msg)
{
    timer->stop();
    //mtx.lock();
    isActive = false;
    disconnect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));
    //disconnect(dBrowser,SIGNAL(loadStarted()),this,SLOT(hideBrovserAndContinueWork()));
    dBrowser->stop();
    previousSearchedData = cEmptyData;
    emit endSearshSig();
    //mtx.unlock();
    writeLog(msg);
    dBrowser->show();
    QTimer::singleShot(600000,this,SLOT(hideBrovserAndContinueWork())); // 10 min
}

void VBSearshMachine::resumeSearsh()
{
    if(isActive) return;
    previousSearchedData = cEmptyData;
    isActive = true;
    emit sigStatus(tr("Search: ")+sCurrFindKeyword);
    writeLog(tr("Search: ")+sCurrFindKeyword,false);
    dBrowser->settings()->setAttribute(QWebSettings::AutoLoadImages,false);
    connect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));
    connect(dBrowser,SIGNAL(loadStarted()),this,SLOT(hideBrovserAndContinueWork()));
    if(sCurrentSearshData.isEmpty())
    {
        if(isEnd) return;
        //QTimer::singleShot(getWaitTime(),this,SLOT(movingToNextPage())); // Перевірити чи на ту саму сторінку переходить чи на наступну при помилці
        QTimer::singleShot(getWaitTime(),this,SLOT(searshFinished()));
    }
    else
    {
        emit parseNextUrlSig(sCurrentSearshData);
        sCurrentSearshData = "";
    }
}

void VBSearshMachine::hideBrovserAndContinueWork()
{
    if(dBrowser->isVisible())
    {
        dBrowser->hide();
    }
    if(!isActive)
    {
        emit programContinueSearchSig();
        QTimer::singleShot(10000,this,SLOT(resumeSearsh())); // 10 sec;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void VBSearshMachine::captchaCheckingResult(QString result)
{

        //writeLog(tr("Captcha recognized!"),false);
        //dBrowser->page()->setViewportSize(dBrowser->page()->mainFrame()->contentsSize());
        emit sigStatus(tr("Search: ")+sCurrFindKeyword);

        QWebElement webElement = dBrowser->page()->mainFrame()->findFirstElement(sSEParams.sCaptchaParams.sW3FormCaptchaSelector); //"IMG[class=b-captcha__image]"

        QWebElement resultElement = webElement.findFirst(sSEParams.sCaptchaParams.sW3ResultCaptchaSelector);

        resultElement.setAttribute("value",result);

        //QWebElement button = webElement.findFirst("input[type=submit]");
        QWebElement button = webElement.findFirst(sSEParams.sCaptchaParams.sW3SubmitButtonSelector);
        //if (button.isNull()) QMessageBox::critical(this, "", "error");

        connect(dBrowser,SIGNAL(loadFinished(bool)),this,SLOT(searshFinished(bool)));

        button.evaluateJavaScript("this.click();");

        timer->start(120000); // 2 мин


}

void VBSearshMachine::captchaErrorResult(QByteArray result)
{
    // reloading page Якщо капчу не вдалось розпізнати

    if(result.contains("ERROR_NO_SLOT_AVAILABLE"))
    {
        writeLog(tr("Capcha error: Your bid is too low. Please increase bid at rucaptcha.com"));
        movingToNextPage(false); // сингл шот???
    }
    else if(result.contains("ERROR"))
    {
        writeLog(tr("Capcha error: ")+result,true);
        stopSearsh();
    }
    else movingToNextPage(false); // сингл шот???
}

void VBSearshMachine::OnUrlChanged(const QUrl&)
{
    QString jScript = QString("var navigator = new Object; navigator.appName = '%1';").arg(cUserAgent);
    dBrowser->page()->mainFrame()->evaluateJavaScript(jScript);
}




