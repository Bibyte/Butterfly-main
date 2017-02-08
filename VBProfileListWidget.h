#ifndef VBPROFILELISTWIDGET_H
#define VBPROFILELISTWIDGET_H

#include <QListWidget>
#include <QString>
#include <QHash>
#include "VBParameters.h"
#include <QIcon>
#include <QMessageBox>
//#include "global.h"

class VBProfileListWidget : public QListWidget
{
    Q_OBJECT
private:
    QString sOldName, sProfileType;
    QHash<QString, QString> sProfileHash;
    VBParameters sParams;
    bool isCopy;
    bool isNew;
    QIcon sItemIcon;

protected:
    void loadProfileHash();
    QString getNewProfileName(QString baseName);
    QString getNewProfileFileName(QString baseName);
    QDir getChildDir(QString url, QString subFolder);

public:
    explicit VBProfileListWidget(QWidget *parent = 0);
    VBProfileListWidget(QString listUrl, QString pProfileType, QIcon pItemIcon, QWidget *parent = 0);
    ~VBProfileListWidget();
    void load();
    bool renameCurrProfile(QString newName);
    bool reFileNameCurrProfile(QString newFileName);
    QString getCurrentFileName();
    QString getCurrentName();

signals:
    void ChangedElement(QString pNewName, QString pNewFILEname, QString pOldFileName, bool isCopy, bool isNew);
    // Якщо pOldFileName = "" то попередній елемент видалили
    // Якщо isCopy = true - то це доданий елемент копіюванням

protected slots:
    void onItemSelectionChanged();

public slots:
    void AddNew(QString pName);
    void AddCopy();
    void RemoveCurrent();

};

#endif // VBPROFILELISTWIDGET_H
