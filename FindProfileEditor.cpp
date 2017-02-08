#include "FindProfileEditor.h"
#include "ui_FindProfileEditor.h"

FindProfileEditor::FindProfileEditor(QWidget *parent)
{

}

FindProfileEditor::FindProfileEditor(QString pParamDir, QWidget *parent) :
    QDialog(parent),
    sParams(qApp->applicationDirPath()),
    ui(new Ui::FindProfileEditor)

{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    dWhiteList = new VBOrAndWidget(0, 0, 400, 400);
    dBlackList = new VBOrAndWidget(0, 0, 400, 400);

    ui->tabWidget->insertTab(2,dWhiteList,tr("White list"));
    ui->tabWidget->insertTab(3,dBlackList,tr("Black list"));


    dProfileList = new VBProfileListWidget(pParamDir,cFindProfileFolder,QIcon(":/Main/filter.png"),this);
    dProfileList->setToolTip("The filter profile list");
    connect(dProfileList,SIGNAL(ChangedElement(QString, QString, QString, bool, bool)),this,SLOT(ChangedElement(QString, QString, QString, bool, bool)));
    ui->groupSearshEngineProfile->layout()->addWidget(dProfileList);
    dProfileList->load();

    connect(this,SIGNAL(finished(int)),this,SLOT(saveCurrentProfileOnClose(int)));
    connect(ui->Name,SIGNAL(editingFinished()),this,SLOT(renameProfile()));
    connect(ui->FileName,SIGNAL(editingFinished()),this,SLOT(renameProfileFile()));
    connect(ui->toolButtonAdd,SIGNAL(clicked()),this,SLOT(addNewElement()));
    connect(ui->toolButtonAddCopy,SIGNAL(clicked()),dProfileList,SLOT(AddCopy()));
    connect(ui->toolButtonRemove,SIGNAL(clicked()),dProfileList,SLOT(RemoveCurrent()));
    connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
    //ui->Date->setLocale(QLocale::system());
}

FindProfileEditor::~FindProfileEditor()
{
    delete dProfileList;
    delete ui;
}

void FindProfileEditor::reloadProfileList()
{
    dProfileList->load();
    if(dProfileList->count())
    {
        ChangedElement(dProfileList->getCurrentName(),dProfileList->getCurrentFileName());
    }
}


VBFindProfile FindProfileEditor::getParametersFromForm()
{
    VBFindProfile seParams(
            ui->Name->text(),
            ui->Description->toPlainText(),
            ui->Date->date(),
            ui->CheckSpecificURL->isChecked(),
            ui->SpecificURL->text(),
            dWhiteList->params(),
            dBlackList->params());
    return seParams;
}

void FindProfileEditor::setParametersToForm(VBFindProfile seParams)
{
    ui->Name->setText(seParams.sName);
    ui->Description->setText(seParams.sDescription);
    ui->Date->setDate(seParams.sDate);
    ui->CheckSpecificURL->setChecked(seParams.sCheckSpecificURL);
    ui->SpecificURL->setText(seParams.sSpecificURL);
    dWhiteList->setParams(seParams.sWhiteList);
    dBlackList->setParams(seParams.sBlackList);
}

void FindProfileEditor::clearForm()
{
    ui->Name->setText("");
    ui->Description->setText("");
    ui->CheckSpecificURL->setChecked(false);
    ui->SpecificURL->setText("");
    ui->FileName->setText("");
    ui->Date->setDate(QDate::currentDate());
    QList<QStringList> emptyList;
    dWhiteList->setParams(emptyList);
    dBlackList->setParams(emptyList);
}

void FindProfileEditor::renameProfile()
{
    if(ui->Name->text().simplified().isEmpty())
    {
        QMessageBox::critical(this,tr("Error renaming profile"),tr("The profile name can not be empty!"),QMessageBox::Ok);
        ui->Name->setText(dProfileList->getCurrentName());
        return;
    }
    if(!dProfileList->renameCurrProfile(ui->Name->text().simplified()))
    {
        QMessageBox::critical(this,tr("Error renaming profile"),tr("The specified name already exists. Specify a different name!"),QMessageBox::Ok);
        ui->Name->setText(dProfileList->getCurrentName());
    }
}

void FindProfileEditor::renameProfileFile()
{
    if(ui->FileName->text().simplified().isEmpty())
    {
        QMessageBox::critical(this,tr("Error renaming profile filename"),tr("The profile filename can not be empty!"),QMessageBox::Ok);
        ui->FileName->setText(dProfileList->getCurrentFileName());
        return;
    }
    if(!dProfileList->reFileNameCurrProfile(ui->FileName->text().simplified()))
    {
        QMessageBox::critical(this,tr("Error renaming profile filename"),tr("The specified name already exists, or the file was deleted!"),QMessageBox::Ok);
        ui->FileName->setText(dProfileList->getCurrentFileName());
    }
}


// SLOTS

void FindProfileEditor::ChangedElement(QString pNewName, QString pNewFILEname, QString pOldFileName, bool isCopy, bool isNew)
{
    // Контроль, в разі якщо список профілів зпорожніє
    if(pNewName.isNull())
    {
        clearForm();
        ui->tabWidget->setEnabled(false);
        return;
    }
    ui->tabWidget->setEnabled(true);
    /////////

    if(!pOldFileName.isEmpty()) // Сохраняєм попередній
    {
        sParams.setFindProfile(getParametersFromForm(),pOldFileName);
    }
    if(!isCopy) // Якщо не копія то завантажуємо параметри, інакше залишаєм все як є
    {
        setParametersToForm(sParams.getFindProfile(pNewFILEname));
        ui->FileName->setText(dProfileList->getCurrentFileName()); // Підгружаємо імя файлу
    }
    if(isNew)
    {
        ui->Name->setText(pNewName);
        ui->FileName->setText(pNewFILEname);
        sParams.setFindProfile(getParametersFromForm(),pNewFILEname);
    }
}

void FindProfileEditor::addNewElement()
{
    dProfileList->AddNew(tr("New profile"));
    ui->Date->setDate(QDate::currentDate());
}

void FindProfileEditor::saveCurrentProfileOnClose(int)
{
    sParams.setFindProfile(getParametersFromForm(),ui->FileName->text());
    if(dProfileList->count())
    {
        emit profileChanged(dProfileList->currentItem()->text());
    }
}

void FindProfileEditor::setCurrentFP(QString pCurrentFP)
{
    QList<QListWidgetItem*> currChoiceIndex = dProfileList->findItems(pCurrentFP,Qt::MatchFixedString);
    if(!currChoiceIndex.isEmpty())
    {
        dProfileList->setCurrentItem(currChoiceIndex[0]);
    }
}
