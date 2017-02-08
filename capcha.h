#ifndef CAPCHA_H
#define CAPCHA_H

#include <QDialog>

namespace Ui {
    class Capcha;
}

class Capcha : public QDialog
{
    Q_OBJECT

public:
    explicit Capcha(QByteArray imgUrl, QWidget *parent = 0);
    ~Capcha();

private:
    Ui::Capcha *ui;

signals:
    void capchaEntered(QString capcha);

private slots:
    void btnClicked();
};

#endif // CAPCHA_H
