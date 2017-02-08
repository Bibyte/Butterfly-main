#ifndef VBPARAMETERS_H
#define VBPARAMETERS_H

#include <QSettings>
#include <QString>
#include <QHash>
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include <QObject>
#include "global.h"
#include <QTranslator>
#include <VBOnlineParams.h>

#include <VBRUCapcha.h>

#define cSearshEngineFolder "search-profiles"
#define cFindProfileFolder "filter-profiles"

class VBParameters: public QObject
{
    Q_OBJECT
private:
    QString sParamPath;
    QString sError;
    QTextCodec* codec;
protected:
    QHash<QString,QString> getParamList(QString profileType);
    void addError(QString text);
public:
    explicit VBParameters(QObject* parent = 0);
    VBParameters(QString pParamPath, QObject* parent = 0);

    // Main params
    void setMainParams(VBMainParams pParams);
    VBMainParams getMainParams();
    QHash<QString,QString> getLangList();

    // Online params
    void updateOnlineParams(VBOnlineParams pParams);
    void resetOnlineParams();
    VBOnlineParams getOnlineParams();

    // Списки профилей отримуються (назви файлів тіпа)
    QHash<QString,QString> getSearshEnginesList();
    QHash<QString,QString> getFindProfileList();

    // Профілі отримуються і записуються
    VBSearshEngineParameters getSearshEngine(QString pID);
    VBFindProfile getFindProfile(QString pID);
    void setSearshEngine(VBSearshEngineParameters pParams, QString pID);
    void setFindProfile(VBFindProfile pParams, QString pID);

    bool delSearshEngine(QString pID);
    bool delFindProfile(QString pID);

    void setPath(QString path);
    QString getPath();
    QString getFPPath();
    QString getSEPath();

    // Фіксація помилок
    QString getLastError();
};

#endif // VBPARAMETERS_H
