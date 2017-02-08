#ifndef SEARSHENGINEEDITOR_H
#define SEARSHENGINEEDITOR_H

#include <QtWidgets/QDialog>
#include <QHash>
#include "VBParameters.h"
#include "VBProfileListWidget.h"
#include <QtWidgets/QMessageBox>
#include <VBRUCapcha.h>

namespace Ui {
    class SearshEngineEditor;
}

class SearshEngineEditor : public QDialog
{
    Q_OBJECT

private:
    Ui::SearshEngineEditor *ui;

protected:
    VBParameters sParams;
    VBProfileListWidget* dProfileList;

protected:
    VBSearshEngineParameters getParametersFromForm();
    void setParametersToForm(VBSearshEngineParameters seParams);
    void clearForm();

protected slots:
    void ChangedElement(QString pNewName, QString pNewFILEname, QString pOldFileName = "", bool isCopy = false, bool isNew = false);
    void renameProfile();
    void renameProfileFile();
    void addNewElement();
    void saveCurrentProfileOnClose(int);

public:
    explicit SearshEngineEditor(QWidget *parent = 0);
    SearshEngineEditor(QString pParamDir, QWidget *parent = 0);
    ~SearshEngineEditor();
    void reloadProfileList();
    void setCurrentSE(QString pCurrentSE);

signals:
    void profileChanged(QString pProfileName);

private slots:
    void on_MinWait_valueChanged(int arg1);
    void on_MaxWait_valueChanged(int arg1);
};

#endif // SEARSHENGINEEDITOR_H
