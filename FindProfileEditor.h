#ifndef FINDPROFILEEDITOR_H
#define FINDPROFILEEDITOR_H

#include <QtWidgets/QDialog>
#include <QHash>
#include "VBParameters.h"
#include "VBProfileListWidget.h"
#include <QtWidgets/QMessageBox>
#include "VBOrAndWidget.h"

namespace Ui {
    class FindProfileEditor;
}

class FindProfileEditor : public QDialog
{
    Q_OBJECT

protected:
    VBParameters sParams;
    VBProfileListWidget* dProfileList;
    VBOrAndWidget* dWhiteList;
    VBOrAndWidget* dBlackList;

protected:
    VBFindProfile getParametersFromForm();
    void setParametersToForm(VBFindProfile seParams);
    void clearForm();

protected slots:
    void ChangedElement(QString pNewName, QString pNewFILEname, QString pOldFileName = "", bool isCopy = false, bool isNew = false);
    void renameProfile();
    void renameProfileFile();
    void addNewElement();
    void saveCurrentProfileOnClose(int);

public:
    explicit FindProfileEditor(QWidget *parent = 0);
    FindProfileEditor(QString pParamDir, QWidget *parent = 0);
    ~FindProfileEditor();
    void reloadProfileList();
    void setCurrentFP(QString pCurrentFP);

private:
    Ui::FindProfileEditor *ui;

signals:
    void profileChanged(QString pProfileName);
};

#endif // FINDPROFILEEDITOR_H
