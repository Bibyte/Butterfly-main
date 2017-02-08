#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets/QDialog>
#include <QUrl>
#include <QByteArray>
#include <QFile>
#include <QtWidgets/QFileDialog>
#include <QTextStream>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMenu>
#include <QEvent>
#include <QDesktopServices>
#include <QDateTime>
#include "VBParamEngine.h"
#include "VBSearshModule.h"
#include "VBBanner.h"
#include "Authentication/GAuth.h"
#include "Proxy/proxylist.h"
#include <QtWidgets/QTableWidget>
#include <GSearchParams.h>
#include <QTimer>

//#include <QtNetwork/QNetworkAccessManager>
//#include <QUrl>
//#include <QtNetwork/QNetworkRequest>
//#include <QtNetwork/QNetworkReply>
//#include <Authentication/GBProtocol.h>
//#include <Authentication/gmac.h>

#define cCurrentVersion 4.02

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    void addLangItems();
    void addCountryItems();
    void createActions();
    void createTrayIcon();
    void checkNewVersion();

private:
    //int currPage;
    int currSite;
    QString findString;
    QString currHTMLList;
    QString sCurrBannerName;
    QTextCodec* codec;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    VBParamEngine* dParamEngine;
    VBSearshModule* dSearshModule;
    QNetworkAccessManager* versionManager;
    int eventChanger;
    VBBanner* banner;
    bool sSaveErrorSites;
    GAuth* dAuthEngine;
    ProxyList* dProxyList;
    QMutex mtx;
    QTimer* dAuthTimer;

    long sPagesCount;
    long sDleCount;
    long sDleCountRepeat;
    long sWrongCount;
    long sErrCount;
    long sMaxParallelThreads;
    long sCurrPage;
protected:
    int getWaitTime();
    void execNextKeyword();
    void loadMainParams();
    void saveMainParams();
    void userAccessControl(int accessFlag);
    void baseAccessControl();
    bool initStart();
    void resetStartSerrings();
    void updateTrayIconToolTip();
    virtual bool eventFilter(QObject *obj, QEvent *event);
    QString getHTMLMessage(int code);
    QString getHTMLServiceMessage(int code, int satus);

signals:
    //void movePage();

private slots:
    void btnPause_clicked();
    void showBaloonMessage(QSystemTrayIcon::MessageIcon,const QString&);
    void trayAction(QSystemTrayIcon::ActivationReason reason);
    void bannerActivated();
    void writeLog(const QString& text, bool showMessage = true);
    void addSite(const QUrl &parUrl);
    void fragmentsFound(const QStringList& list);
    void addSiteError(const QUrl &parUrl, const QString &msg);
    void addSiteWrong(const QUrl &parUrl);
    void addSiteExec();
    void on_tabUrls_currentChanged(int index);
    void on_btnSaveLog_clicked();
    void on_btnSaveLastHtml_clicked();
    void reciveCurrHTMLList(const QString&);
    void programExit();
    void programContinueSearch();
    void on_btnHelp_clicked();
    void on_langCombo_activated(int index);
    void bannerLoaded(bool normal);
    void on_openSiteBtn_clicked();
    void on_btnWriteLogToFile_clicked();
    void on_isWriteLogToFile_stateChanged(int arg1);
    void on_SaveDomainOnly_clicked();
    void on_btnOpenSEFolder_clicked();
    void on_btnOpenFPFolder_clicked();
    void versionGetted(QNetworkReply* reply);
    void updateUserAuthData(QString htmlBody, bool isError);
    void EnableService(int code);
    void DisableService(int code);
    void SetAuthStatus(bool pStatus);
    void HaveNoServices(bool pStatus);
    void on_btnUpdateConnection_clicked();
    void on_AuthLogin_editingFinished();
    void on_btnAddProxy_clicked();
    void on_btnDeleteProxy_clicked();
    void GetPoxyList(QList<GProxyOptions>);
    void tableProxycellChanged(int row, int column);
    //void on_btnSaveProxy_clicked();
    void on_btnRefreshPRoxy_clicked();
    void on_UseFilter_clicked();
    void setCurrPage(long page);
    void updateConnectionAuto();

    void on_btnAddProxyList_clicked();





    void on_toolButton_1_pressed();

    void on_toolButton_2_pressed();

    void on_toolButton_3_pressed();

    void on_toolButton_4_pressed();

    void on_toolButton_5_pressed();

    void on_toolButton_6_pressed();

    void on_toolButton_1_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_5_clicked();

    void on_toolButton_6_clicked();

public slots:
    void btnStartSearchClick();
    void btnInfoClick();
    void btnStopClick();
    void btnContinueClick();
    void openDLG();
    void openDlgSearshResults();
    void inputDLG();
    void openSearshEngineDialog();
    void openFindProfileDialog();
    void loadSEComboBox(const QString& = "");
    void loadFPComboBox(const QString& = "");
    void stopSearsh();

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // DIALOG_H
