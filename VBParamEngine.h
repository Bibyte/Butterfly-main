#ifndef VBPARAMENGINE_H
#define VBPARAMENGINE_H

#include "VBParameters.h"
#include "SearshEngineEditor.h"
#include "FindProfileEditor.h"
#include <QLocale>

class VBParamEngine : public VBParameters
{
    Q_OBJECT
private:
    SearshEngineEditor* dSearshEngineEdditor;
    FindProfileEditor* dFindProfileEditor;
    VBParameters sParams;
public:
    explicit VBParamEngine(QObject *parent = 0);
    explicit VBParamEngine(QString pParamDir, QObject *parent = 0);
    ~VBParamEngine();

    // Діалоги
    void openSearshEngineDialog(QString pCurrentSE = "");
    void openFindProfileDialog(QString pCurrentFP = "");

    // Основні параметри
    void setMainParams(VBMainParams);
    VBMainParams getMainParams();

    // Параметри профілів
    QHash<QString,QString> getFindProfileParameterList();
    QHash<QString,QString> getSearshEngineParameterList();
    VBFindProfile getFindProfileParameter(QString pID);
    VBSearshEngineParameters getSearshEngineParameter(QString pID);

    // Мовні параметри
    void setLocale(QLocale);
public slots:
    void resendFPListChanged(QString);
    void resendSEListChnaged(QString);
signals:
    void FPListChanged(QString);
    void SEListChnaged(QString);

};

#endif // VBPARAMENGINE_H
