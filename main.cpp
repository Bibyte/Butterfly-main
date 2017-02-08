#include <QtWidgets/QApplication>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMessageBox>
#include "dialog.h"
#include <QTranslator>
#include <QSettings>
#include <QDir>
#include <QImage>
#include <QFileInfo>
#include <QDate>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(MainResource);

    QTextCodec *codec = QTextCodec::codecForLocale();
    //QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    //QTextCodec::setCodecForTr(codec);

    QApplication app(argc, argv);

    // Loading language
    QSettings settings("BiByte","Butterfly");
    QString currlang = settings.value("currentLang").toString();
    if(currlang.isEmpty())
    {
        QSettings settings("VBI","Butterfly");
        currlang = settings.value("currentLang").toString();
    }
    QString localeName = "";
    QTranslator* translator = new QTranslator();
    if(!currlang.isEmpty() && currlang != "en_En")
    {
        QDir dir(qApp->applicationDirPath());
        if(!dir.exists("lang"))
        {
            dir.mkdir("lang");
        }
        dir.cd("lang"); //qDebug() << currlang;
        if(translator->load(currlang,dir.path()))
        {
            app.installTranslator(translator);
            QFileInfo langInfo = QFileInfo(dir.absoluteFilePath(currlang));
            localeName = langInfo.baseName();
        }
        else
        {
            QMessageBox::critical(0, QObject::tr("Butterfly"),
                                  QObject::tr("Failed to load language. Perhaps the language file has been deleted."));
        }
    }
    else
    {
        localeName = "en_En";
    }
    // End loading language

    // Test system tray
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Butterfly"),
                              QObject::tr("The system tray is not found! "
                                          "The program requires the system tray!"));
        return 1;
    }
    // END Test system tray

    // Time use limites
//    QDate curDate = QDate::currentDate();
//    if((curDate.year()>2012) || (curDate.month()>8 && curDate.year()==2012))
//    {
//        QMessageBox::critical(0, QObject::tr("Butterfly"),
//                              QObject::tr("The new version of program is available! "
//                                          "Please download it from bibyte.net!"));
//       return 3;
//    }
    // END Time use limites

    // Promote protection level 1
    if(!QImage(":/Main/test.gif").size().width())
    {
        QMessageBox::critical(0, QObject::tr("Butterfly"),
                              QObject::tr("The image engine is damaged! "
                                          "Please reinstall the program!"));
       return 2;
    }
    // End Promote protection level 1

    QApplication::setQuitOnLastWindowClosed(false);

    Dialog w;
    if(!localeName.isEmpty()) w.setLocale(QLocale(localeName));

    // Time use limites protection level 2
//    curDate = QDate::currentDate();
//    if((curDate.year()>2012) || (curDate.month()>8 && curDate.year()==2012))
//    {
//       return 0;
//    }
    // END Time use limites
    w.setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
    w.show();
    int result = 0;

    try
    {
        result = app.exec();
    }
    catch(int e)
    {
        //qDebug()<<"Error try again";
        result = app.exec();
    }
    delete translator;
    return result;
}
