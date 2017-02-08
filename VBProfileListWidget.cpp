#include "VBProfileListWidget.h"

VBProfileListWidget::VBProfileListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

VBProfileListWidget::VBProfileListWidget(QString listUrl, QString pProfileType, QIcon pItemIcon, QWidget *parent) :
        QListWidget(parent),sParams(listUrl)
{
    sItemIcon = pItemIcon;
    sOldName = "";
    sProfileType = pProfileType;
    //if(sProfileType.isEmpty()) sProfileType = "";
    isCopy = false;
    isNew = false;
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(onItemSelectionChanged()));
}

VBProfileListWidget::~VBProfileListWidget()
{
    //onItemSelectionChanged();
}

void VBProfileListWidget::load()
{
    clear();
    loadProfileHash();
}

void VBProfileListWidget::loadProfileHash()
{
    if(sProfileType == cSearshEngineFolder)
    {
        sProfileHash = sParams.getSearshEnginesList();
    }
    else // cFindProfileFolder
    {
        sProfileHash = sParams.getFindProfileList();
    }

    QHashIterator<QString, QString> i(sProfileHash);
    while (i.hasNext())
    {
        i.next();
        addItem(new QListWidgetItem(sItemIcon,i.key()));
    }
    if(sProfileHash.count())
    {
        sortItems();
        setCurrentItem(item(0));
        //sCurrentProfile = currentItem()->text(); // Це робиться попереднім пунктом автоматом
    }
}

QString VBProfileListWidget::getNewProfileName(QString baseName)
{
    QString newName = baseName;
    if(sProfileHash.contains(newName))
    {
        long newCount = 0;
        while(sProfileHash.contains(newName+" "+QString::number(++newCount)));
        newName = newName+" "+QString::number(newCount);
    }
    return newName;
}

QString VBProfileListWidget::getNewProfileFileName(QString baseName)
{
    QString newFileName = baseName;

    // Перевіряємо чи файл такий існує і якщо так, то дописуєм індекс
    QDir checkValidNewFile = getChildDir(sParams.getPath(),sProfileType);
    if(checkValidNewFile.exists(newFileName+".ini")) // Якщо вже існує такий файлік
    {
        long fileIter = 0;
        while(checkValidNewFile.exists(newFileName+"_"+QString::number(++fileIter)+".ini"));
        newFileName = newFileName + "_" + QString::number(fileIter);
    }
    return newFileName;
}

void VBProfileListWidget::onItemSelectionChanged()
{
    if(currentItem()==0)
    {
        emit ChangedElement(QString(),QString(),QString(),false,false);
        return;
    }
    QString currName = currentItem()->text();
    emit ChangedElement(currName, sProfileHash[currName],sProfileHash[sOldName],isCopy,isNew);
    if(isCopy) isCopy = false;
    if(isNew) isNew = false;
    sOldName = currName;
}


void VBProfileListWidget::AddNew(QString pName)
{
    QString newName = getNewProfileName(pName);
    sProfileHash[newName] = getNewProfileFileName(pName);
    addItem(new QListWidgetItem(sItemIcon,newName));
    isNew = true;
    setCurrentItem(item(count()-1));
}

void VBProfileListWidget::AddCopy()
{
    if(!count()) return;
    QString pName = currentItem()->text();
    QString newName = getNewProfileName(pName);
    sProfileHash[newName] = getNewProfileFileName(sProfileHash[pName]);
    addItem(new QListWidgetItem(sItemIcon,newName));
    isCopy = true;
    isNew = true;
    setCurrentItem(item(count()-1));
}

void VBProfileListWidget::RemoveCurrent()
{
    if(!count()) return;
    if(QMessageBox::question(this,tr("Profile deleting"), tr("Really delete a profile?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::No) return;
    QString currName = currentItem()->text();
    if(sProfileType == cSearshEngineFolder)
    {
        if(sParams.delSearshEngine(sProfileHash[currName]))
        {
            sProfileHash.remove(currName);
            sOldName = ""; // takeItem ініціює itemSelectionChanged, який відправляє сигнал
            takeItem(currentIndex().row());
        }
    }else
    {
        if(sParams.delFindProfile(sProfileHash[currName]))
        {
            sProfileHash.remove(currName);
            sOldName = ""; // takeItem ініціює itemSelectionChanged, який відправляє сигнал
            takeItem(currentIndex().row());
        }
    }
}

bool VBProfileListWidget::renameCurrProfile(QString newName)
{
    if(currentItem()->text() == newName) return true; // Якщо імя те саме
    if(!sProfileHash.contains(newName))
    {
        QString currName = currentItem()->text();
        sProfileHash[newName] = sProfileHash[currName];
        currentItem()->setText(newName);
        sProfileHash.remove(currName);
        sOldName = newName;
        return true;
    }
    else return false;
}

bool VBProfileListWidget::reFileNameCurrProfile(QString newFileName)
{
    if(getCurrentFileName() == newFileName) return true;  // Якщо імя те саме

    // Перевіряємо чи файл такий існує і якщо так, то дописуєм індекс

    QDir checkValidNewFile = getChildDir(sParams.getPath(),sProfileType);
    if(!checkValidNewFile.exists(newFileName+".ini")) // Якщо не існує такий файлік
    {
        QString currName = currentItem()->text();
        QString currFileName = sProfileHash[currName];
        bool result = checkValidNewFile.rename(currFileName+".ini",newFileName+".ini");
        if(result)
        {
            sProfileHash[currName] = newFileName;
            return true;
        }
        else return false;
    }
    else return false;
}

QString VBProfileListWidget::getCurrentFileName()
{
    return sProfileHash[currentItem()->text()];
}

QString VBProfileListWidget::getCurrentName()
{
    return currentItem()->text();
}

QDir VBProfileListWidget::getChildDir(QString url, QString subFolder)
{
    QDir dir(url);
    if(!dir.exists(subFolder))
    {
        dir.mkdir(subFolder);
    }
    dir.cd(subFolder);
    return dir;
}


