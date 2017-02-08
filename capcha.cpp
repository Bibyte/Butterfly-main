#include "capcha.h"
#include "ui_capcha.h"

Capcha::Capcha(QByteArray imgUrl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Capcha)
{
    ui->setupUi(this);
    ui->webView->setUrl(QUrl(imgUrl));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(btnClicked()));
}

Capcha::~Capcha()
{
    delete ui;
}

void Capcha::btnClicked()
{
    emit capchaEntered(ui->capchaText->text());
    hide();
}
