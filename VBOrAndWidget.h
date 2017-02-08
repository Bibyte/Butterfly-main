#ifndef VBORANDWIDGET_H
#define VBORANDWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QStringList>
#include <QGroupBox>
#include <QSplitter>
class VBOrAndWidget : public QWidget
{
    Q_OBJECT
protected:
    QHBoxLayout *lMain, *lOrBtn, *lAndBtn;
    QVBoxLayout *lOr, *lAnd;
    QSplitter *mainSplitter;
    QPushButton *dOrAddBtn, *dOrDelBtn, *dAndAddBtn, *dAndDelBtn;
    QListWidget *dOrList, *dAndList;
    QGroupBox *orGB, *andGB;
    QLineEdit *dCurrRec;
    QList<QStringList> mainList;

protected:
    QStringList removeEmpty(QStringList pList);
    void updateForm();

protected slots:
    void addOr();
    void addAnd();
    void delOr();
    void delAnd();
    void currItemChanged();
    void currRecChanged();
    void requirmentChanged();
    void enterToLineEdit(QListWidgetItem*);
    void enterToOrList(QListWidgetItem*);

public:
    explicit VBOrAndWidget(QWidget *parent = 0);
    VBOrAndWidget(int x, int y, int w, int h, QWidget *parent = 0);
    ~VBOrAndWidget();

    void setParams(QList<QStringList> lst);
    QList<QStringList> params();


signals:

public slots:

};

#endif // VBORANDWIDGET_H
