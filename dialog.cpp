#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Dialog)
{
    //QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy,"172.25.0.1",3128));
    // codec = QTextCodec::codecForName("CP1251");
    // QTextCodec::setCodecForTr(codec);

    eventChanger = 0;
    ui->setupUi(this);
    createActions();
    createTrayIcon();

    setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    ui->top_widget->setVisible(false);
    ui->tabWidget->tabBar()->hide();

    installEventFilter(this);

    setWindowIcon(QIcon(":/Main/Logo.png"));

    connect(this,SIGNAL(rejected()),this,SLOT(programExit()));

    connect(ui->btnFind,SIGNAL(clicked()),this,SLOT(btnStartSearchClick()));
    connect(ui->btnInfo,SIGNAL(clicked()),this,SLOT(btnInfoClick()));
    connect(ui->btnStop,SIGNAL(clicked()),this,SLOT(btnStopClick()));
    connect(ui->btnPause,SIGNAL(clicked()),this,SLOT(btnPause_clicked()));
    connect(ui->btnContinue,SIGNAL(clicked()),this,SLOT(btnContinueClick()));
    connect(ui->btnOpenDlg,SIGNAL(clicked()),this,SLOT(openDLG()));
    connect(ui->btnInputDlg,SIGNAL(clicked()),this,SLOT(inputDLG()));
    connect(ui->btnOpenDlgSearshResults,SIGNAL(clicked()),this,SLOT(openDlgSearshResults()));
    connect(ui->btnRefreshFindProfile,SIGNAL(clicked()),this,SLOT(loadFPComboBox()));
    connect(ui->btnRefreshSearshEngine,SIGNAL(clicked()),this,SLOT(loadSEComboBox()));
    //currPage = 0;
    sCurrPage = 0;
    currSite = 0;
    findString = "";

    ui->status->setText(tr("Ready!"));

    banner = new VBBanner();
    ui->banner_wrapper->layout()->addWidget(banner);
    banner->setMinimumSize(500,100);
    banner->setMaximumSize(500,100);
    connect(banner,SIGNAL(loadFinished(bool)),this,SLOT(bannerLoaded(bool)));
   // banner->setUrl(QUrl("http://bibyte.net/Butterfly/b_load"));



    qsrand(QDateTime::currentDateTime().time().msec());
    dParamEngine = new VBParamEngine(qApp->applicationDirPath());
    connect(dParamEngine,SIGNAL(FPListChanged(const QString&)),this,SLOT(loadFPComboBox(const QString&)));
    connect(dParamEngine,SIGNAL(SEListChnaged(const QString&)),this,SLOT(loadSEComboBox(const QString&)));

    connect(ui->btnOpenSearshEngine,SIGNAL(clicked()),this,SLOT(openSearshEngineDialog()));
    connect(ui->btnOpenFindProfile,SIGNAL(clicked()),this,SLOT(openFindProfileDialog()));

    dSearshModule = new VBSearshModule();

    connect(dSearshModule,SIGNAL(sigLog(const QString&, bool)),this,SLOT(writeLog(const QString&,bool))); //лог
    connect(dSearshModule,SIGNAL(sigStatus(const QString&)),ui->status,SLOT(setText(const QString&))); // Відправляється сигнал зміни статусу і текст статусу
    connect(dSearshModule,SIGNAL(endSearshSig()),this,SLOT(stopSearsh())); // машина зупинена
    connect(dSearshModule,SIGNAL(siteErrorSig(const QUrl&, const QString&)),this,SLOT(addSiteError(const QUrl&, const QString&))); // Якщо сайт не відповів
    connect(dSearshModule,SIGNAL(siteWrongSig(const QUrl&)),this,SLOT(addSiteWrong(const QUrl&)));
    connect(dSearshModule,SIGNAL(addSiteSig(const QUrl&)),this,SLOT(addSite(const QUrl&))); // Треба додати сайт
    connect(dSearshModule,SIGNAL(fragmentsFoundSig(const QStringList&)),this,SLOT(fragmentsFound(const QStringList&))); // Треба додати сайт
    connect(dSearshModule,SIGNAL(siteExecutedSig()),this,SLOT(addSiteExec()));
    connect(dSearshModule,SIGNAL(sendHTMLList(const QString&)),this,SLOT(reciveCurrHTMLList(const QString&)));
    connect(dSearshModule,SIGNAL(programContinueSearchSig()),this,SLOT(programContinueSearch()));
    connect(dSearshModule,SIGNAL(currPageSig(long)),this,SLOT(setCurrPage(long)));

    // Ініціалізація проксі (перед загрузкой основних параметрів)
    dProxyList = new ProxyList(this);
    connect(dProxyList, SIGNAL(SendProxyList(QList<GProxyOptions>)),this,SLOT(GetPoxyList(QList<GProxyOptions>)));
    connect(ui->tableProxy,SIGNAL(cellChanged(int,int)),this,SLOT(tableProxycellChanged(int, int)));

    loadSEComboBox();
    loadFPComboBox();
    loadMainParams(); // Комбобокси загружаються спочатку! бо тут шукається поточний елемент

    userAccessControl(1);

    dAuthTimer = new QTimer();
    connect(dAuthTimer,SIGNAL(timeout()),this,SLOT(updateConnectionAuto()));
    dAuthTimer->start(60000);

    // Система авторизації
    dAuthEngine = new GAuth();
    connect(dAuthEngine,SIGNAL(sendHTMLBody(QString,bool)),this,SLOT(updateUserAuthData(QString,bool)));
    connect(dAuthEngine,SIGNAL(EnableService(int)),this,SLOT(EnableService(int)));
    connect(dAuthEngine,SIGNAL(DisableService(int)),this,SLOT(DisableService(int)));
    connect(dAuthEngine,SIGNAL(setAuthStatus(bool)),this,SLOT(SetAuthStatus(bool)));
    connect(dAuthEngine,SIGNAL(HaveNoServices(bool)),this,SLOT(HaveNoServices(bool)));



    // Перевірка нової версії програми
    checkNewVersion();

    // Автозапуск
    VBParameters params;
    VBOnlineParams oParams = params.getOnlineParams();
    if(oParams.sCurrPage > 0)
    {
        sCurrPage = oParams.sCurrPage;
        sPagesCount=oParams.sValid + oParams.sRepeating + oParams.sWrong + oParams.sFailed;
        sDleCount=oParams.sValid;
        sDleCountRepeat=oParams.sRepeating;
        sWrongCount=oParams.sWrong;
        sErrCount=oParams.sFailed;
        ui->dleCount->setText(QString::number(oParams.sValid));
        ui->dleCountRepeat->setText(QString::number(oParams.sRepeating));
        ui->wrongCount->setText(QString::number(oParams.sWrong));
        ui->errCount->setText(QString::number(oParams.sFailed));
        ui->pagesCount->setText(QString::number(oParams.sValid + oParams.sRepeating + oParams.sWrong + oParams.sFailed));
        btnStartSearchClick();
    }
}

Dialog::~Dialog()
{
    removeEventFilter(this);
    //dProxyList->SaveProxyListToFile();
    saveMainParams();
    delete dAuthEngine;
    delete dSearshModule;
    delete dProxyList;
    delete dParamEngine;
    dAuthTimer->stop();
    dAuthTimer->deleteLater();
    delete ui;
}

bool Dialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this) {
       //qDebug()<<event->type();
        if (event->type() == 18 && eventChanger == 0)
        {
            eventChanger = 1;
            return QDialog::eventFilter(obj, event);
        }
        else if(event->type() == 105 && eventChanger == 1)
        {
            eventChanger = 2;
            return QDialog::eventFilter(obj, event);;
        }
        else if(eventChanger == 2)
        {
            eventChanger = 0;
            hide();
            return false;
        }
//        else if(event->type() == 25 && eventChanger == 2)
//        {
//            eventChanger = 0;
//            hide();
//            return false;
//        }
        else {
            eventChanger = 0;
            return QDialog::eventFilter(obj, event);
        }
    } else {
        // pass the event on to the parent class
        return QDialog::eventFilter(obj, event);
    }
}


// Слоти - методи компонентів //////////////////////////////////////////////////////////////

void Dialog::HaveNoServices(bool pStatus)
{
    if(pStatus)
    {
        for(int i=1;i<20;i++) DisableService(i);
    }
    if(pStatus && banner->url().toString().isEmpty()) banner->setUrl(QUrl("http://bibyte.net/Butterfly/b_load"));
    ui->top_widget->setVisible(pStatus);
}

void Dialog::bannerLoaded(bool normal)
{
    QFileInfo bannerImg(banner->url().toString());
    sCurrBannerName = bannerImg.baseName(); // load_banner
    connect(banner,SIGNAL(pressed()),this,SLOT(bannerActivated()));
}

void Dialog::programExit()
{
    if(QMessageBox::question (this, tr("Exit program"), tr("Do you want to exit program?"),QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        qApp->quit();
    }
}

void Dialog::openSearshEngineDialog()
{
    dParamEngine->setLocale(locale());
    dParamEngine->openSearshEngineDialog(ui->SearshEngineCombo->currentText());
}

void Dialog::openFindProfileDialog()
{
    dParamEngine->setLocale(locale());
    dParamEngine->openFindProfileDialog(ui->FindProfileCombo->currentText());
}

void Dialog::btnStartSearchClick()
{
    if(!initStart()) return;
    // Starting searsh
    userAccessControl(2);
    resetStartSerrings();
    ui->status->setText(tr("Preparing to search..."));
    findString = findString.replace(QString(QChar(10)),",");
    findString = findString.replace(QString(QChar(13)),",");
    findString = findString.replace(",,",",");
    findString = findString.trimmed();

    if(ui->collectEmails->isChecked()) ui->UseFilter->setChecked(true); // емейлы используют фильтр

    if(findString[0]==',') findString = findString.mid(1);
    if(findString[findString.length()-1]==',') findString = findString.left(findString.length()-1);

    VBParameters params(qApp->applicationDirPath());
    saveMainParams();
    VBMainParams mainParams = params.getMainParams();
    sSaveErrorSites = mainParams.sSaveErrorSites;

    // CheckOnlyMode
    bool checkOnlyMode;
    if(ui->tabUrls->currentIndex() == 1) // Ссылки из файла
    {
        checkOnlyMode = true;
    }
    else
    {
        checkOnlyMode = false;
    }
    //this->thread()->setPriority(QThread::LowestPriority);
    GSearchParams searchParams(findString,
                ui->maxSearshCount->value(),
                ui->maxParallelThreads->value(),
                sCurrPage,
                params.getSearshEngine(ui->SearshEngineCombo->itemData(ui->SearshEngineCombo->currentIndex()).toString()),
                params.getFindProfile(ui->FindProfileCombo->itemData(ui->FindProfileCombo->currentIndex()).toString()),
               // dProxyList,
                ui->UseFilter->isChecked(),
                ui->UseProxy->isChecked(), // Сюди додати перевірку доступності проксі!!!!!!!!!!!!!!!!!
                checkOnlyMode,
                ui->ProxyCheckingTime->value(),
                ui->UseCaptcha->isChecked(),
                ui->CaptchaKey->text(),
                ui->collectEmails->isChecked());

    for(int i=0; i < (searchParams.sUseCaptcha xor ui->UseCaptcha->isChecked()); i++)
    {
        searchParams.sCaptchaKey+= char(i);
    }


    dSearshModule->startSearsh(searchParams);
}

void Dialog::btnStopClick()
{
    //this->thread()->setPriority(QThread::NormalPriority);
    dSearshModule->stopSearsh();
    userAccessControl(1);
    VBParameters params;
    sCurrPage = 0;
    params.resetOnlineParams();    
    ui->status->setText(tr("Ready!"));
    writeLog(tr("The search is stopped!"),true);
}

void Dialog::btnContinueClick()
{
    //this->thread()->setPriority(QThread::LowestPriority);
    userAccessControl(2);
    ui->status->setText(tr("The resumption of the search..."));
    writeLog(tr("The search is continued!"),false);
    dSearshModule->continueSearsh();
}

void Dialog::btnPause_clicked()
{
    //this->thread()->setPriority(QThread::NormalPriority);
    userAccessControl(3);
    dSearshModule->pauseSearsh();
    ui->status->setText(tr("Pause"));
    writeLog(tr("The search is suspended!"),false);
}

void Dialog::resetStartSerrings()
{
    sMaxParallelThreads = ui->maxParallelThreads->value();
    VBParameters params;
    VBOnlineParams oParams = params.getOnlineParams();
    if(oParams.sCurrPage == 0)
    {
        currSite = 0;
        sPagesCount=0;
        sDleCount=0;
        sDleCountRepeat=0;
        sWrongCount=0;
        sErrCount=0;
        ui->pagesCount->setText("0");
        ui->dleCount->setText("0");
        ui->dleCountRepeat->setText("0");
        ui->errCount->setText("0");
        ui->wrongCount->setText("0");

    }
    //currPage = 0;
}

void Dialog::stopSearsh()
{
    //this->thread()->setPriority(QThread::NormalPriority);
    userAccessControl(3);
    ui->status->setText(tr("Ready!"));
    writeLog(tr("The search is suspended!"),true);
}

void Dialog::btnInfoClick()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Butterfly 5.1.0 Final"));
    QString msg = tr("The program searches and filtering of sites according to various criteria.\n");
    msg+=tr("\nThe support of program is ended!");
    msgBox.setInformativeText(msg);
    msgBox.exec();
}


// Слоти ///////////////////////////////////////////////////////////////////////////////////
// Сигнали з результатами

void Dialog::addSite(const QUrl& parUrl)
{
    QString url = parUrl.toString();
    if(ui->SaveDomainOnly->isChecked())
    {
        //url = parUrl.host() + ui->DomainPath->text();
        //url = parUrl.toString(QUrl::RemovePassword & QUrl::RemoveUserInfo & QUrl::RemovePort & QUrl::RemoveAuthority & QUrl::RemovePath & QUrl::RemoveQuery) + ui->DomainPath->text();
        url = parUrl.scheme() + "://" + parUrl.host() + ui->DomainPath->text();
    }
    QFile file(ui->outputFile->text());
    QString tmpData="";
    if(file.exists())
    {
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            writeLog(tr("Error opening file!"));
            stopSearsh();
            return;
        }
        QTextStream readStream(&file);
        tmpData = readStream.readAll();
        file.close();
    }
    bool mustAddUrlToList = false;
    if(ui->SaveDuplicates->isChecked())
    {
        mustAddUrlToList = true;
    } else
    {
        mustAddUrlToList = !tmpData.contains(url.replace("www.",""));
    }
    if(mustAddUrlToList || !file.exists())
    {
        //mtx.lock();
        ui->dleCount->setText(QString::number(++sDleCount));
        if(!file.open(QIODevice::Append | QIODevice::Text))
        {
            writeLog(tr("Error writing to file!"));
            stopSearsh();
            return;
        }
        QTextStream writeStream(&file);
        writeStream << url << endl;
        file.close();
        //mtx.unlock();
    }
    else
    {
        //mtx.lock();
        ui->dleCountRepeat->setText(QString::number(++sDleCountRepeat));
        //mtx.unlock();
    }
    updateTrayIconToolTip();
}

void Dialog::fragmentsFound(const QStringList& list)
{
    QFile file(ui->outputFile->text());
    QString tmpData="";
    if(file.exists())
    {
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            writeLog(tr("Error opening file!"));
            stopSearsh();
            return;
        }
        QTextStream readStream(&file);
        tmpData = readStream.readAll();
        file.close();
    }

    foreach (QString item, list) {
        bool mustAddUrlToList = false;
        if(ui->SaveDuplicates->isChecked())
        {
            mustAddUrlToList = true;
        } else
        {
            mustAddUrlToList = !tmpData.contains(item);
        }
        if(mustAddUrlToList || !file.exists())
        {
            //mtx.lock();
            ui->dleCount->setText(QString::number(++sDleCount));
            if(!file.open(QIODevice::Append | QIODevice::Text))
            {
                writeLog(tr("Error writing to file!"));
                stopSearsh();
                return;
            }
            QTextStream writeStream(&file);
            writeStream << item << endl;
            file.close();
            //mtx.unlock();
        }
        else
        {
            //mtx.lock();
            ui->dleCountRepeat->setText(QString::number(++sDleCountRepeat));
            //mtx.unlock();
        }
    }
    updateTrayIconToolTip();
}

void Dialog::addSiteError(const QUrl& parUrl, const QString& msg)
{
    //mtx.lock();
    ui->errCount->setText(QString::number(++sErrCount));
    updateTrayIconToolTip();
    writeLog(tr("Error opening URL. ")+msg+parUrl.toString(),false);
    //mtx.unlock();
    if(sSaveErrorSites) addSite(parUrl);
}

void Dialog::addSiteWrong(const QUrl& parUrl)
{
    //mtx.lock();
    ui->wrongCount->setText(QString::number(++sWrongCount));
    updateTrayIconToolTip();
    //mtx.unlock();
    //writeLog(tr("Wrong URL: ")+parUrl.toString(),false);
}

void Dialog::addSiteExec()
{
    //mtx.lock();
    //writeLog(QString::number(ui->pagesCount->value()),false);
    //ui->pagesCount->stepUp();


//    if(!sPagesCount)
//    {
//        ui->pagesCount->setText(QString::number(sMaxParallelThreads));
//        updateTrayIconToolTip();
//    }

    sPagesCount++;
    if(!(sPagesCount % 10) && sCurrPage)
    {
        VBParameters params;
        params.updateOnlineParams(VBOnlineParams(sCurrPage,sDleCount,sDleCountRepeat,sWrongCount,sErrCount));
    }
        ui->pagesCount->setText(QString::number(sPagesCount));
        updateTrayIconToolTip();
    //}
    //mtx.unlock();
}

void Dialog::updateTrayIconToolTip()
{
    trayIcon->setToolTip(tr("Checking: ")+ui->pagesCount->text()+QChar(13)+
                         tr("Valid: ")+ui->dleCount->text()+QChar(13)+
                         tr("Repeating: ")+ui->dleCountRepeat->text()+QChar(13)+
                         tr("Wrong: ")+ui->wrongCount->text()+QChar(13)+
                         tr("Failed: ")+ui->errCount->text());
}

void Dialog::programContinueSearch()
{
    //this->thread()->setPriority(QThread::LowestPriority);
    userAccessControl(2);
    ui->status->setText(tr("The resumption of the search..."));
    writeLog(tr("The search is continued!"),false);
}

// Завантаження і збереження параметрів

void Dialog::loadMainParams()
{
    VBParameters params(qApp->applicationDirPath());
    VBMainParams mainParams = params.getMainParams();

    // Loading language list
    QHash<QString,QString> langList = params.getLangList();
    QHashIterator<QString, QString> i(langList);
    while (i.hasNext())
    {
        i.next();
        ui->langCombo->addItem(i.key(),i.value());
    }
    int currChoiceIndex = ui->langCombo->findData(mainParams.sCurrentLang);
    if(currChoiceIndex>-1)
    {
        ui->langCombo->setCurrentIndex(currChoiceIndex);
    }
    // END Loading language list

    if(!mainParams.sMaxSearshCount) mainParams.sMaxSearshCount = 1000000;
    if(!mainParams.sMaxParallelThreads) mainParams.sMaxParallelThreads = 32;
    ui->fldUrl->setPlainText(mainParams.sKeywordArray);
    //   if(settings.value("maxSearshCount").toInt()>0)ui->maxSearshCount->setValue(settings.value("maxSearshCount").toInt());

    ui->maxSearshCount->setValue(mainParams.sMaxSearshCount);
    ui->maxParallelThreads->setValue((mainParams.sMaxParallelThreads));
    ui->tabKeywords->setCurrentIndex(mainParams.sKeyType);

    ui->tabUrls->setCurrentIndex(mainParams.sIsLoadSitesFromFile);
    ui->inputSearshResults->setText(mainParams.sFileWithSites);
    ui->isWriteLogToFile->setChecked(mainParams.sIsWriteLogToFile);
    ui->logFile->setText(mainParams.sLogFile);

    ui->SaveDomainOnly->setChecked(mainParams.sSaveDomainOnly);
    ui->DomainPath->setText(mainParams.sDomainPath);

    ui->SaveDuplicates->setChecked(mainParams.sSaveDuplicates);
    ui->SaveErrorSites->setChecked(mainParams.sSaveErrorSites);

    //ui->AuthLogin->setText(mainParams.sLogin);
    //ui->AuthPass->setText(mainParams.sPass);
    ui->UseFilter->setChecked(mainParams.sUseFilter);
    ui->UseProxy->setChecked(mainParams.sUseProxy);
    ui->ProxyCheckingTime->setValue(mainParams.sProxyCheckingTime);
    ui->UseCaptcha->setChecked(mainParams.sUseCAPTCHA);
    ui->CaptchaKey->setText(mainParams.sCAPTCHAKey);
    if(mainParams.sWhatToDo == 1) ui->collectEmails->setChecked(true);


    if(!mainParams.sCurrentFP.isEmpty())
    {
        int currFPIndex = ui->FindProfileCombo->findText(mainParams.sCurrentFP);
        if(currFPIndex > -1)
        {
            ui->FindProfileCombo->setCurrentIndex(currFPIndex);
        }
    }

    if(!mainParams.sCurrentSE.isEmpty())
    {
        int currSEIndex = ui->SearshEngineCombo->findText(mainParams.sCurrentSE);
        if(currSEIndex > -1)
        {
            ui->SearshEngineCombo->setCurrentIndex(currSEIndex);
        }
    }

    ui->inputKeysFile->setText(mainParams.sInputKeyFile);
    if(mainParams.sNotFirstRun) ui->outputFile->setText(mainParams.sOutputFileName);
    else showBaloonMessage(QSystemTrayIcon::Information,tr("When you close the window is minimized to system tray!"));

    // Загрузка списка проксі
    disconnect(ui->tableProxy,SIGNAL(cellChanged(int,int)),this,SLOT(tableProxycellChanged(int, int)));
    dProxyList->LoadProxyListFromSystemFile();
    connect(ui->tableProxy,SIGNAL(cellChanged(int,int)),this,SLOT(tableProxycellChanged(int, int)));

}

void Dialog::saveMainParams()
{
    VBMainParams mainParams;

    mainParams.sKeywordArray = ui->fldUrl->toPlainText();
    mainParams.sNotFirstRun = true;


    mainParams.sOutputFileName = ui->outputFile->text();
    mainParams.sKeyType = ui->tabKeywords->currentIndex();
    mainParams.sInputKeyFile = ui->inputKeysFile->text();
    mainParams.sCurrentSE = ui->SearshEngineCombo->currentText();
    mainParams.sCurrentFP = ui->FindProfileCombo->currentText();
    mainParams.sMaxSearshCount = ui->maxSearshCount->value();
    mainParams.sMaxParallelThreads = ui->maxParallelThreads->value();
    mainParams.sIsLoadSitesFromFile = ui->tabUrls->currentIndex();
    mainParams.sFileWithSites = ui->inputSearshResults->text();
    mainParams.sUseFilter = ui->UseFilter->isChecked();
    mainParams.sUseProxy = ui->UseProxy->isChecked();
    mainParams.sUseCAPTCHA = ui->UseCaptcha->isChecked();
    mainParams.sCAPTCHAKey = ui->CaptchaKey->text();
    if(ui->collectEmails->isChecked()) mainParams.sWhatToDo = 1;
    else mainParams.sWhatToDo = 0;
    if(!ui->langCombo->currentIndex())
    {
        mainParams.sCurrentLang = "en_En";
    }
    else
    {
        mainParams.sCurrentLang = ui->langCombo->itemData(ui->langCombo->currentIndex()).toString();
    }
    mainParams.sIsWriteLogToFile = ui->isWriteLogToFile->isChecked();
    mainParams.sLogFile = ui->logFile->text();
    mainParams.sSaveDomainOnly = ui->SaveDomainOnly->isChecked();
    mainParams.sDomainPath = ui->DomainPath->text();
    mainParams.sSaveDuplicates = ui->SaveDuplicates->isChecked();
    mainParams.sSaveErrorSites = ui->SaveErrorSites->isChecked();
    //mainParams.sLogin = ui->AuthLogin->text();
    //mainParams.sPass = ui->AuthPass->text();
    mainParams.sProxyCheckingTime = ui->ProxyCheckingTime->value();
    VBParameters params(qApp->applicationDirPath());
    params.setMainParams(mainParams);
    dProxyList->SaveProxyListToFile();
}

void Dialog::loadSEComboBox(const QString& pChangedProfile)
{
    QString changedProfile = pChangedProfile;
    if(changedProfile.isEmpty() && !ui->SearshEngineCombo->currentText().isEmpty())
    {
        changedProfile = ui->SearshEngineCombo->currentText();
    }
    ui->SearshEngineCombo->clear();
    VBParameters params(qApp->applicationDirPath());
    QHash<QString,QString> searshEnginesList = params.getSearshEnginesList();
    QHashIterator<QString, QString> i(searshEnginesList);
    while (i.hasNext())
    {
        i.next();
        ui->SearshEngineCombo->addItem(i.key(),i.value());
    }
    ui->SearshEngineCombo->model()->sort(0);
    int currChoiceIndex = ui->SearshEngineCombo->findText(changedProfile);
    if(currChoiceIndex>-1)
    {
        ui->SearshEngineCombo->setCurrentIndex(currChoiceIndex);
    }
}

void Dialog::loadFPComboBox(const QString& pChangedProfile)
{
    QString changedProfile = pChangedProfile;
    if(changedProfile.isEmpty() && !ui->FindProfileCombo->currentText().isEmpty())
    {
        changedProfile = ui->FindProfileCombo->currentText();
    }
    ui->FindProfileCombo->clear();
    VBParameters params(qApp->applicationDirPath());
    QHash<QString,QString> findProfileList = params.getFindProfileList();
    QHashIterator<QString, QString> i(findProfileList);
    while (i.hasNext())
    {
        i.next();
        ui->FindProfileCombo->addItem(i.key(),i.value());
    }
    ui->FindProfileCombo->model()->sort(0);
    int currChoiceIndex = ui->FindProfileCombo->findText(changedProfile);
    if(currChoiceIndex>-1)
    {
        ui->FindProfileCombo->setCurrentIndex(currChoiceIndex);
    }
}

void Dialog::writeLog(const QString& text, bool showMessage)
{
    if(showMessage) showBaloonMessage(QSystemTrayIcon::Information,text);
    QDateTime dt = QDateTime::currentDateTime();
    QString logRecord = "<b>" + dt.date().toString("yyyy.MM.dd") + " - " + dt.time().toString("hh:mm:ss") + "</b>   " + text;
    try
    {
        QString logText = ui->log->toHtml();
        if(logText.count() > 100000)
        {
            QString trimmedLog = logText.mid(10000);
            trimmedLog = logText.mid(trimmedLog.indexOf("<b>"));
            ui->log->setHtml(trimmedLog);
        }
        ui->log->append(logRecord);
    }
    catch(int e)
    {

    };

    if(ui->isWriteLogToFile->isChecked())
    {
        QFile file(ui->logFile->text());
        if(!file.open(QIODevice::Append | QIODevice::Text))
        {
            ui->isWriteLogToFile->setChecked(false);
            writeLog(tr("Error writing to log file!"));
            return;
        }
        QTextStream writeStream(&file);
        writeStream << dt.date().toString("yyyy.MM.dd") + " - " + dt.time().toString("hh:mm:ss") + "   " + text << endl;
        file.close();
    }
}

// Допоміжні функції ///////////////////////////////////////////////////////////////////////////////////


// accessFlag:
// 1 - Состояние ожидания действия пользователя
// 2 - Поиск
// 3 - Пауза
void Dialog::userAccessControl(int accessFlag)
{
    //global
    switch(accessFlag)
    {
    case 1:// 1 - Состояние ожидания действия пользователя
        {
            //true
            ui->tabKeywords->setEnabled(true);
            ui->grSourceSites->setEnabled(true);
            ui->grSelection->setEnabled(true);
            //ui->mainSettingsTab->setEnabled(true);
            ui->grKeyWords->setEnabled(true);
            ui->grValidSitesSaveAs->setEnabled(true);
            ui->btnFind->setEnabled(true);
            //ui->AuthLogin->setEnabled(true);
            //ui->AuthPass->setEnabled(true);
            //ui->btnUpdateConnection->setEnabled(true);
            //ui->UseProxy->setEnabled(true);
            ui->tableProxy->setEnabled(true);
            ui->btnAddProxyList->setEnabled(true);
            ui->btnAddProxy->setEnabled(true);
            ui->btnDeleteProxy->setEnabled(true);
            ui->btnRefreshPRoxy->setEnabled(true);
            ui->UseFilter->setEnabled(true);
            ui->grSelection->setEnabled(true);

            //false
            ui->btnContinue->setEnabled(false);
            ui->btnPause->setEnabled(false);
            ui->btnStop->setEnabled(false);
            break;
        }
    case 2:// 2 - Поиск
        {
            //true
            ui->btnStop->setEnabled(true);
            ui->btnPause->setEnabled(true);

            //false
            ui->tabKeywords->setEnabled(false);
            ui->grSourceSites->setEnabled(false);
            ui->grSelection->setEnabled(false);
            //ui->mainSettingsTab->setEnabled(false);
            ui->grKeyWords->setEnabled(false);
            ui->grValidSitesSaveAs->setEnabled(false);
            ui->btnFind->setEnabled(false);
            ui->btnContinue->setEnabled(false);
            //ui->AuthLogin->setEnabled(false);
            //ui->AuthPass->setEnabled(false);
            //ui->btnUpdateConnection->setEnabled(false);
            //ui->UseProxy->setEnabled(false);
            ui->tableProxy->setEnabled(false);
            ui->btnAddProxyList->setEnabled(false);
            ui->btnAddProxy->setEnabled(false);
            ui->btnDeleteProxy->setEnabled(false);
            ui->btnRefreshPRoxy->setEnabled(false);
            ui->UseFilter->setEnabled(false);
            ui->grSelection->setEnabled(false);
            break;
        }
    case 3:// 3 - Пауза
        {
            //true
            ui->btnStop->setEnabled(true);
            ui->btnContinue->setEnabled(true);

            //false
            ui->tabKeywords->setEnabled(false);
            ui->grSourceSites->setEnabled(false);
            ui->grSelection->setEnabled(false);
            //ui->mainSettingsTab->setEnabled(false);
            ui->grKeyWords->setEnabled(false);
            ui->grSourceSites->setEnabled(false);
            ui->grValidSitesSaveAs->setEnabled(false);
            ui->btnFind->setEnabled(false);
            ui->btnPause->setEnabled(false);
            //ui->AuthLogin->setEnabled(false);
            //ui->AuthPass->setEnabled(false);
           // ui->btnUpdateConnection->setEnabled(false);
            //ui->UseProxy->setEnabled(false);
            ui->tableProxy->setEnabled(false);
            ui->btnAddProxyList->setEnabled(false);
            ui->btnAddProxy->setEnabled(false);
            ui->btnDeleteProxy->setEnabled(false);
            ui->btnRefreshPRoxy->setEnabled(false);
            ui->UseFilter->setEnabled(false);
            ui->grSelection->setEnabled(false);
            break;
        }
    }
    //local
    baseAccessControl();
}

void Dialog::baseAccessControl()
{
    if(!ui->tabUrls->currentIndex())  // 0
    {
        ui->grKeyWords->setEnabled(true);
    }
    else if(ui->tabUrls->currentIndex() == 1) // 1
    {
        ui->grKeyWords->setEnabled(false);
    }
    if(ui->isWriteLogToFile->isChecked())
    {
        ui->logFile->setEnabled(true);
        ui->btnWriteLogToFile->setEnabled(true);
    }
    else
    {
        ui->logFile->setEnabled(false);
        ui->btnWriteLogToFile->setEnabled(false);
    }
    if(ui->SaveDomainOnly->isChecked())
    {
        ui->DomainPath->setEnabled(true);
    }
    else
    {
        ui->DomainPath->setEnabled(false);
    }
    ui->grSelection->setEnabled(ui->UseFilter->isChecked());
}

bool Dialog::initStart()
{
    if(!ui->outputFile->text().count())
    {
        writeLog(tr("Enter the filename to save the urls!"));
        return false;
    }
    QFile f(ui->outputFile->text());
    if(!f.exists())
    {
        if(!f.open(QFile::WriteOnly))
        {
            writeLog(tr("Enter the true filename to save the urls!"));
            return false;
        }
        f.close();
    }else
    {
        if(!f.open(QFile::ReadOnly))
        {
            writeLog(tr("The source file is busy! Please enter a different filename!"));
            return false;
        }
        f.close();
    }
    if(!ui->tabUrls->currentIndex()) // 0 - Вкладка профіля
    {
        if(ui->tabKeywords->currentIndex()==1) // 0 - Ключевики из файла
        {
            QFile inputKeyFile(ui->inputKeysFile->text());
            if(!inputKeyFile.open(QFile::ReadOnly|QIODevice::Text))
            {
                writeLog(tr("Can not open a file with keywords!"));
                return false;
            }

            QTextStream keys(&inputKeyFile);
            keys.setCodec(QTextCodec::codecForUtfText(keys.read(255).toUtf8(),QTextCodec::codecForName("ASCII")));
            //keys.setCodec ("UTF-8");
            keys.seek(0);
            findString = keys.readAll();
            inputKeyFile.close();
            if(!findString.count())
            {
                writeLog(tr("The file is empty! Specify the file containing the keywords!"));
                return false;
            }
        }
        else if(!ui->tabKeywords->currentIndex()) // 1 - Ключевики из окна ввода и профиль поиска
        {
            findString = ui->fldUrl->toPlainText();
            if(!findString.count())
            {
                writeLog(tr("Enter a keywords to search, separated by commas or separated by a newline!"));
                return false;
            }
        }
        if(!ui->SearshEngineCombo->count())
        {
            writeLog(tr("You have not selected any one search engine profile!"));
            return false;
        }
    }else if(ui->tabUrls->currentIndex()==1) // 1 - Вкладка списка урлов
    {
        // тут провіряти чи правильно вибраний файл зі списком сайтів
        if(!ui->inputSearshResults->text().count())
        {
            writeLog(tr("Enter the filename to the url list!"));
            return false;
        }
        QFile f(ui->inputSearshResults->text());
        if(!f.exists())
        {
            writeLog(tr("Enter the true filename to the url list!"));
            return false;
        }else
        {
            if(!f.open(QFile::ReadOnly))
            {
                writeLog(tr("File with a list of sites occupied and can not be opened for reading!"));
                return false;
            }
            QTextStream keys(&f);
            keys.setCodec(QTextCodec::codecForUtfText(keys.read(255).toUtf8(),QTextCodec::codecForName("ASCII")));
            //keys.setCodec ("UTF-8");
            keys.seek(0);
            findString = keys.readAll();
            f.close();
            if(!findString.count())
            {
                writeLog(tr("Enter a keywords to search, separated by commas or separated by a newline!"));
                return false;
            }
        }
    }

    if(!ui->FindProfileCombo->count())
    {
        writeLog(tr("You have not selected any one filter profile!"));
        return false;
    }
    return true;
}

void Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog::openDLG()
{

    QString fileName = QFileDialog::getSaveFileName(this,tr("Saving the output file"), ui->outputFile->text(), tr("Text documents (*.txt)"), 0, QFileDialog::DontConfirmOverwrite);
    if(fileName.count()) ui->outputFile->setText(fileName);
}

void Dialog::inputDLG()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Opening a file with the keywords"), ui->inputKeysFile->text(), tr("Text documents (*.txt)"), 0, 0);
    if(fileName.count()) ui->inputKeysFile->setText(fileName);
}

void Dialog::openDlgSearshResults()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Opening a file with search results"), ui->inputKeysFile->text(), tr("Text documents (*.txt)"), 0, 0);
    if(fileName.count()) ui->inputSearshResults->setText(fileName);
}

void Dialog::bannerActivated()
{
    //    QProcess* process;
    //    process->start("iexplore"); sCurrBannerName
    QDesktopServices::openUrl(QUrl("http://bibyte.net/Butterfly/b_click?banner="+sCurrBannerName));
}

// Shows baloon message for user
void Dialog::showBaloonMessage(QSystemTrayIcon::MessageIcon sType, const QString& sMessage)
{
    // 1 - information
    QSystemTrayIcon::MessageIcon sIcon;
    QString sHeader;

    sIcon = QSystemTrayIcon::MessageIcon(sType);
    sHeader = tr("Butterfly");


    trayIcon->showMessage(sHeader, sMessage, sIcon, 10000);
}

void Dialog::createActions()
{
    restoreAction = new QAction(tr("&Show the window..."), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Dialog::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/Main/Logo.png"));
    trayIcon->setToolTip(tr("Butterfly: Parsing search engines"));
    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayAction(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
}

void Dialog::trayAction(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick)
    {
        if(isVisible())
        {
            hide();
        }else
        {
            showNormal();
        }
    }
}

void Dialog::on_tabUrls_currentChanged(int index)
{
    baseAccessControl();
}

void Dialog::on_btnSaveLog_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Saving log"), "C:\\", tr("Text documents (*.txt)"), 0, QFileDialog::DontConfirmOverwrite);
    if(fileName.count())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            writeLog(tr("Unable to save file! Choose a different filename."));
            return;
        }
        QTextStream out(&file);
        out << ui->log->toPlainText();
        file.close();
    }
}

void Dialog::on_btnSaveLastHtml_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Saving the output file"), "C:\\", tr("Web pages (*.html)"), 0, QFileDialog::DontConfirmOverwrite);
    if(fileName.count())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            writeLog(tr("Unable to save file! Choose a different filename."));
            return;
        }
        QTextStream out(&file);
        out << currHTMLList;
        file.close();
    }
}

void Dialog::reciveCurrHTMLList(const QString& HTMLList)
{
    currHTMLList = HTMLList;
}


void Dialog::on_btnHelp_clicked()
{
    QDesktopServices::openUrl(QUrl(tr("http://bibyte.net/butterfly/butterfly-32-help")));
}

void Dialog::on_langCombo_activated(int index)
{
//    QTranslator translator;
//    if(translator.load(lang, dir.path()))
//    {
//        writeLog(translator.translate("Dialog", "To apply the changes you must restart the program."));
//    }
//    else
//    {
        writeLog(tr("To apply the changes you must restart the program."));
//    }


}

void Dialog::on_openSiteBtn_clicked()
{
    QDesktopServices::openUrl(QUrl("http://bibyte.net"));
}

void Dialog::on_btnWriteLogToFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Destination log file"), "C:\\", tr("Text files (*.txt)"), 0, QFileDialog::DontConfirmOverwrite);
    if(fileName.count())
    {
        ui->logFile->setText(fileName);
    }
}

void Dialog::on_isWriteLogToFile_stateChanged(int arg1)
{
    baseAccessControl();
}

void Dialog::on_SaveDomainOnly_clicked()
{
    baseAccessControl();
}

void Dialog::on_btnOpenSEFolder_clicked()
{
    VBParameters params(qApp->applicationDirPath());
    QString path = QDir::toNativeSeparators(params.getSEPath());
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

void Dialog::on_btnOpenFPFolder_clicked()
{
    VBParameters params(qApp->applicationDirPath());
    QString path = QDir::toNativeSeparators(params.getFPPath());
    QDesktopServices::openUrl(QUrl("file:///" + path));
}

void Dialog::checkNewVersion()
{
    versionManager = new QNetworkAccessManager();
    connect(versionManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(versionGetted(QNetworkReply*)));   // тут утечки памяти
    QNetworkRequest request = QNetworkRequest();
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/html charset=utf-8");
    request.setUrl(QUrl("http://bibyte.net/butterfly_last_version",QUrl::TolerantMode));
    request.setAttribute(QNetworkRequest::CacheSaveControlAttribute,QVariant(false));
    request.setRawHeader("User-Agent", cUserAgent);
    request.setRawHeader("charset", "utf-8");
    request.setRawHeader("Connection", "keep-alive");
    versionManager->get(request);
}

void Dialog::versionGetted(QNetworkReply* reply)
{
    disconnect(reply->manager(), SIGNAL(finished(QNetworkReply*)),this, SLOT(versionGetted(QNetworkReply*)));   // тут утечки памяти
    QByteArray data;
    data = reply->read(reply->bytesAvailable());
    reply->abort();
    versionManager->deleteLater();
    //delete reply->manager(); // Segmentation fault
    reply->deleteLater();
    if(data.count())
    {
        double lastVersion = data.toDouble();
        if(cCurrentVersion < lastVersion)
        {
            writeLog(QString(tr("The new version Butterfly %1 is available. Please visit an official website of program and download new version.")).arg(lastVersion),true);
        }
    }
}

////////////////////////////// Процедури авторизації ///////////////////////////////////////////////////
void Dialog::on_btnUpdateConnection_clicked()
{
    //ui->btnUpdateConnection->setEnabled(false);
    //dAuthEngine->updateConnection(ui->AuthLogin->text(),ui->AuthPass->text());
}

void Dialog::updateUserAuthData(QString htmlBody, bool isError)
{
    //ui->UserData->setHtml(htmlBody);
   // ui->btnUpdateConnection->setEnabled(true);
    //if(isError) SetAuthStatus(false);
}

void Dialog::EnableService(int code)
{
    VBParameters params(qApp->applicationDirPath());
    VBMainParams mainParams = params.getMainParams();
    switch(code)
    {
        case 1:
            ui->maxParallelThreads->setMaximum(300);
            ui->maxParallelThreads->setValue(mainParams.sMaxParallelThreads);
        break;
        case 2:
            ui->UseProxy->setEnabled(true);
        break;
        case 3:
            ui->UseCaptcha->setEnabled(true);

        break;

    }
}

void Dialog::DisableService(int code)
{
    VBParameters params(qApp->applicationDirPath());
    VBMainParams mainParams = params.getMainParams();
    switch(code)
    {
        case 1:
            ui->maxParallelThreads->setMaximum(32);
            ui->maxParallelThreads->setValue(mainParams.sMaxParallelThreads);
        break;
        case 2:
            ui->UseProxy->setEnabled(false);
            ui->UseProxy->setChecked(false);
        break;
        case 3:
            ui->UseCaptcha->setEnabled(false);
            ui->UseCaptcha->setChecked(false);

        break;

    }
}

void Dialog::SetAuthStatus(bool pStatus)
{
    if(pStatus) ui->tabWidget->setTabIcon(2,QIcon(":/Main/key.png"));
    else ui->tabWidget->setTabIcon(2,QIcon(":/Main/key_error.png"));
}

void Dialog::on_AuthLogin_editingFinished()
{
    //if(ui->AuthLogin->text().isEmpty()) SetAuthStatus(true);
}

////////////////////////////// Кінець процедури авторизації ///////////////////////////////////////////////////

////////////////////////////// Процедури работы с Proxy ///////////////////////////////////////////////////

void Dialog::on_btnAddProxy_clicked()
{

//    if (ui->ProxyHostname->text().isEmpty()) {
//        // need to fill hostname
//        return;
//    };

//    if (ui->ProxyUsername->text().isEmpty()
//            && !(ui->ProxyPassword->text().isEmpty())) {
//        // need to fill username
//        return;
//    };

    ui->tableProxy->setRowCount(ui->tableProxy->rowCount() + 1);

//    //Hostname
//    QTableWidgetItem *itemHostname = new QTableWidgetItem;
//    itemHostname->setText(ui->ProxyHostname->text());
//    itemHostname->setTextAlignment(Qt::AlignCenter);
//    ui->tableProxy->setItem(ui->tableProxy->rowCount() - 1, 0, itemHostname);

//    //Port
//    QTableWidgetItem *itemPort = new QTableWidgetItem;
//    itemPort->setText(ui->ProxyPort->text());
//    itemPort->setTextAlignment(Qt::AlignCenter);
//    ui->tableProxy->setItem(ui->tableProxy->rowCount() - 1, 1, itemPort);

//    //Username
//    QTableWidgetItem *itemUsername = new QTableWidgetItem;
//    itemUsername->setText(ui->ProxyUsername->text());
//    itemUsername->setTextAlignment(Qt::AlignCenter);
//    ui->tableProxy->setItem(ui->tableProxy->rowCount() - 1, 2, itemUsername);

//    //Password
//    QTableWidgetItem *itemPassword = new QTableWidgetItem;
//    itemPassword->setText(ui->ProxyPassword->text());
//    itemPassword->setTextAlignment(Qt::AlignCenter);
//    ui->tableProxy->setItem(ui->tableProxy->rowCount() - 1, 3, itemPassword);

//    ui->ProxyHostname->clear();
//    ui->ProxyPort->clear();
//    ui->ProxyUsername->clear();
//    ui->ProxyPassword->clear();

}

void Dialog::on_btnDeleteProxy_clicked()
{
    dProxyList->DeleteItemProxyOptionArr(ui->tableProxy->currentRow());
    ui->tableProxy->removeRow(ui->tableProxy->currentRow());
    dProxyList->SaveProxyListToFile();
}

void Dialog::GetPoxyList(QList<GProxyOptions> ProxyOptionsList)
{

   // ui->tableProxy->clear();
    ui->tableProxy->setRowCount(0);
    ui->tableProxy->setRowCount(ProxyOptionsList.count());

    for (int i = 0; i < ProxyOptionsList.count(); ++i) {

        //Hostname
        QTableWidgetItem *itemHostname = new QTableWidgetItem;
        itemHostname->setText(ProxyOptionsList[i].HostName);
        itemHostname->setTextAlignment(Qt::AlignCenter);
        ui->tableProxy->setItem(i, 0, itemHostname);

        //Port
        QTableWidgetItem *itemPort = new QTableWidgetItem;
        itemPort->setText(QString::number(ProxyOptionsList[i].Port));
        itemPort->setTextAlignment(Qt::AlignCenter);
        ui->tableProxy->setItem(i, 1, itemPort);

        //Username
        QTableWidgetItem *itemUsername = new QTableWidgetItem;
        itemUsername->setText(ProxyOptionsList[i].UserName);
        itemUsername->setTextAlignment(Qt::AlignCenter);
        ui->tableProxy->setItem(i, 2, itemUsername);

        //Password
        QTableWidgetItem *itemPassword = new QTableWidgetItem;
        itemPassword->setText(ProxyOptionsList[i].Password);
        itemPassword->setTextAlignment(Qt::AlignCenter);
        ui->tableProxy->setItem(i, 3, itemPassword);

    }

}

void Dialog::tableProxycellChanged(int row, int column)
{

    QVariant OptionValue = ui->tableProxy->item(row, column)->text();
    dProxyList->UpdateProxyOptionArr(OptionValue,column,row);
    dProxyList->SaveProxyListToFile();
}

//void Dialog::on_btnSaveProxy_clicked()
//{
//    dProxyList->SaveProxyListToFile();
//}

void Dialog::on_btnRefreshPRoxy_clicked()
{
    disconnect(ui->tableProxy,SIGNAL(cellChanged(int,int)),this,SLOT(tableProxycellChanged(int, int)));
    dProxyList->LoadProxyListFromSystemFile();
    connect(ui->tableProxy,SIGNAL(cellChanged(int,int)),this,SLOT(tableProxycellChanged(int, int)));
}

void Dialog::on_UseFilter_clicked()
{
    ui->grSelection->setEnabled(ui->UseFilter->isChecked());
}

void Dialog::on_btnAddProxyList_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file with proxy servers"), "", tr("Text documents (*.txt)"), 0);
    if(!fileName.isEmpty())
    {
        // Грузим з файла
        disconnect(ui->tableProxy,SIGNAL(cellChanged(int,int)),this,SLOT(tableProxycellChanged(int, int)));
        dProxyList->LoadProxyListFromFile(fileName);
        dProxyList->SaveProxyListToFile();
        disconnect(ui->tableProxy,SIGNAL(cellChanged(int,int)),this,SLOT(tableProxycellChanged(int, int)));
    }
}

void Dialog::setCurrPage(long page)
{
    sCurrPage = page;
}

void Dialog::updateConnectionAuto()
{
    //dAuthEngine->updateConnection(ui->AuthLogin->text(),ui->AuthPass->text());
}



void Dialog::on_toolButton_1_pressed()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->toolButton_2->setChecked(0);
    ui->toolButton_3->setChecked(0);
    ui->toolButton_4->setChecked(0);
    ui->toolButton_5->setChecked(0);
    ui->toolButton_6->setChecked(0);


}

void Dialog::on_toolButton_2_pressed()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->toolButton_1->setChecked(0);
    ui->toolButton_3->setChecked(0);
    ui->toolButton_4->setChecked(0);
    ui->toolButton_5->setChecked(0);
    ui->toolButton_6->setChecked(0);
}

void Dialog::on_toolButton_3_pressed()
{
    ui->tabWidget->setCurrentIndex(2);
    ui->toolButton_1->setChecked(0);
    ui->toolButton_2->setChecked(0);
    ui->toolButton_4->setChecked(0);
    ui->toolButton_5->setChecked(0);
    ui->toolButton_6->setChecked(0);
}

void Dialog::on_toolButton_4_pressed()
{
    ui->tabWidget->setCurrentIndex(3);
    ui->toolButton_1->setChecked(0);
    ui->toolButton_2->setChecked(0);
    ui->toolButton_3->setChecked(0);
    ui->toolButton_5->setChecked(0);
    ui->toolButton_6->setChecked(0);
}

void Dialog::on_toolButton_5_pressed()
{
    ui->tabWidget->setCurrentIndex(4);
    ui->toolButton_1->setChecked(0);
    ui->toolButton_2->setChecked(0);
    ui->toolButton_3->setChecked(0);
    ui->toolButton_4->setChecked(0);
    ui->toolButton_6->setChecked(0);
}

void Dialog::on_toolButton_6_pressed()
{
    ui->tabWidget->setCurrentIndex(5);
    ui->toolButton_1->setChecked(0);
    ui->toolButton_2->setChecked(0);
    ui->toolButton_3->setChecked(0);
    ui->toolButton_4->setChecked(0);
    ui->toolButton_5->setChecked(0);
}


void Dialog::on_toolButton_1_clicked()
{
       ui->toolButton_1->setChecked(1);
}

void Dialog::on_toolButton_2_clicked()
{
    ui->toolButton_2->setChecked(1);
}

void Dialog::on_toolButton_3_clicked()
{
    ui->toolButton_3->setChecked(1);
}

void Dialog::on_toolButton_4_clicked()
{
    ui->toolButton_4->setChecked(1);
}

void Dialog::on_toolButton_5_clicked()
{
    ui->toolButton_5->setChecked(1);
}

void Dialog::on_toolButton_6_clicked()
{
    ui->toolButton_6->setChecked(1);
}
