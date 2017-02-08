#include "SearshEngineEditor.h"
#include "ui_SearshEngineEditor.h"

SearshEngineEditor::SearshEngineEditor(QWidget *parent)
{

}

SearshEngineEditor::SearshEngineEditor(QString pParamDir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearshEngineEditor),
    sParams(qApp->applicationDirPath())
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);
    dProfileList = new VBProfileListWidget(pParamDir,cSearshEngineFolder,QIcon(":/Main/wmploc_DLL_475.png"),this);
    dProfileList->setToolTip("The search engine list");
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
}

SearshEngineEditor::~SearshEngineEditor()
{
    delete dProfileList;
    delete ui;
}

void SearshEngineEditor::reloadProfileList()
{
    dProfileList->load();
    if(dProfileList->count())
    {
        ChangedElement(dProfileList->getCurrentName(),dProfileList->getCurrentFileName());
    }
}

VBSearshEngineParameters SearshEngineEditor::getParametersFromForm()
{
    VBRUCapcha ruCaptcha(ui->captcha_phrase->currentIndex(),
                         ui->captcha_regsense->currentIndex(),
                         ui->captcha_numeric->currentIndex(),
                         ui->captcha_calc->currentIndex(),
                         ui->captcha_min_len->currentIndex(),
                         ui->captcha_max_len->currentIndex(),
                         ui->captcha_language->currentIndex(),
                         ui->captcha_W3Selector->text(),
                         ui->captcha_formW3Selector->text(),
                         ui->captcha_resultW3Selector->text(),
                         ui->captcha_submitW3selector->text());

    VBSearshEngineParameters seParams(
            ui->Name->text(),
            ui->Description->toPlainText(),
            ui->Date->date(),
            ui->SearshHost->text(),
            ui->SearshMachineBrand->toPlainText(), // Перевірити як буде конвертувати
            ui->LastPageCheck->text(),
            ui->SpamCheck->text(),
            ui->NoResults->text(),
            ui->StartPage->value(),
            ui->PageStep->value(),
            ui->LastPageCheckBox->isChecked(),
            ui->SpamCheckBox->isChecked(),
            ui->NoResultsCheckBox->isChecked(),
            ui->MinWait->value(),
            ui->MaxWait->value(),
            ui->SessionVars->text(),
            ruCaptcha);
    return seParams;
}

void SearshEngineEditor::setParametersToForm(VBSearshEngineParameters seParams)
{
    QString sSearshBrand;
    sSearshBrand.append(seParams.sSearshMachineBrand);
    QString sSpamCheck;
    sSpamCheck.append(seParams.sSpamCheck);
    QString sLastPageChack;
    sLastPageChack.append(seParams.sLastPageCheck);
    ui->Name->setText(seParams.sName);
    ui->Description->setText(seParams.sDescription);
    ui->Date->setDate(seParams.sDate);
    ui->SearshHost->setText(seParams.sSearshHost);
    ui->SearshMachineBrand->setPlainText(sSearshBrand);
    ui->LastPageCheck->setText(sLastPageChack);
    ui->SpamCheck->setText(sSpamCheck);
    ui->StartPage->setValue(seParams.sStartPage);
    ui->PageStep->setValue(seParams.sPageStep);
    ui->NoResults->setText(seParams.sNoResultsCheck);
    ui->LastPageCheckBox->setChecked(seParams.sLastPageCheckBox);
    ui->SpamCheckBox->setChecked(seParams.sSpamCheckBox);
    ui->NoResultsCheckBox->setChecked(seParams.sNoResultsCheckBox);
    ui->SessionVars->setText(seParams.sSessionParams);
    if(ui->MaxWait->value() > seParams.sMinWait)
    {
        ui->MinWait->setValue(seParams.sMinWait);
        ui->MaxWait->setValue(seParams.sMaxWait);
    }
    else
    {
        ui->MaxWait->setValue(seParams.sMaxWait);
        ui->MinWait->setValue(seParams.sMinWait);
    }

    ui->captcha_phrase->setCurrentIndex(seParams.sCaptchaParams.sPhrase);
    ui->captcha_regsense->setCurrentIndex(seParams.sCaptchaParams.sRegsense);
    ui->captcha_numeric->setCurrentIndex(seParams.sCaptchaParams.sNumeric);
    ui->captcha_calc->setCurrentIndex(seParams.sCaptchaParams.sCalc);
    ui->captcha_min_len->setCurrentIndex(seParams.sCaptchaParams.sMin_len);
    ui->captcha_max_len->setCurrentIndex(seParams.sCaptchaParams.sMax_len);
    ui->captcha_language->setCurrentIndex(seParams.sCaptchaParams.sLanguage);
    ui->captcha_W3Selector->setText(seParams.sCaptchaParams.sW3CaptchaSelector);
    ui->captcha_formW3Selector->setText(seParams.sCaptchaParams.sW3FormCaptchaSelector);
    ui->captcha_resultW3Selector->setText(seParams.sCaptchaParams.sW3ResultCaptchaSelector);
    ui->captcha_submitW3selector->setText(seParams.sCaptchaParams.sW3SubmitButtonSelector);
}

void SearshEngineEditor::clearForm()
{
    ui->Name->setText("");
    ui->Description->setText("");
    ui->Date->setDate(QDate::currentDate());
    ui->SearshHost->setText("");
    ui->SearshMachineBrand->setPlainText("");
    ui->LastPageCheck->setText("");
    ui->SpamCheck->setText("");
    ui->StartPage->setValue(0);
    ui->PageStep->setValue(0);
    ui->NoResults->setText("");
    ui->LastPageCheckBox->setChecked(true);
    ui->SpamCheckBox->setChecked(true);
    ui->NoResultsCheckBox->setChecked(true);
    ui->MaxWait->setValue(8);
    ui->MinWait->setValue(3);   
    ui->FileName->setText("");
}

void SearshEngineEditor::renameProfile()
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

void SearshEngineEditor::renameProfileFile()
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

void SearshEngineEditor::ChangedElement(QString pNewName, QString pNewFILEname, QString pOldFileName, bool isCopy, bool isNew)
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
        sParams.setSearshEngine(getParametersFromForm(),pOldFileName);
    }
    if(!isCopy) // Якщо не копія то завантажуємо параметри, інакше залишаєм все як є
    {
        setParametersToForm(sParams.getSearshEngine(pNewFILEname));
        ui->FileName->setText(dProfileList->getCurrentFileName()); // Підгружаємо імя файлу
    }
    if(isNew)
    {
        ui->Name->setText(pNewName);
        ui->FileName->setText(pNewFILEname);
        sParams.setSearshEngine(getParametersFromForm(),pNewFILEname);
    }
}

void SearshEngineEditor::addNewElement()
{
    dProfileList->AddNew(tr("New profile"));
    ui->LastPageCheckBox->setChecked(true);
    ui->NoResultsCheckBox->setChecked(true);
    ui->SpamCheckBox->setChecked(true);
    ui->Date->setDate(QDate::currentDate());
}

void SearshEngineEditor::saveCurrentProfileOnClose(int)
{
    sParams.setSearshEngine(getParametersFromForm(),ui->FileName->text());
    if(dProfileList->count())
    {
        emit profileChanged(dProfileList->currentItem()->text());
    }
}

void SearshEngineEditor::setCurrentSE(QString pCurrentSE)
{
    QList<QListWidgetItem*> currChoiceIndex = dProfileList->findItems(pCurrentSE,Qt::MatchFixedString);
    if(!currChoiceIndex.isEmpty())
    {
        dProfileList->setCurrentItem(currChoiceIndex[0]);
    }
}

void SearshEngineEditor::on_MinWait_valueChanged(int arg1)
{
    if(arg1>ui->MaxWait->value())
    {
        ui->MaxWait->setValue(arg1);
    }
}

void SearshEngineEditor::on_MaxWait_valueChanged(int arg1)
{
    if(arg1<ui->MinWait->value())
    {
        ui->MinWait->setValue(arg1);
    }
}
