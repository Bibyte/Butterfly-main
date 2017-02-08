#include "VBOrAndWidget.h"

VBOrAndWidget::VBOrAndWidget(QWidget *parent) :
    QWidget(parent)
{

}

VBOrAndWidget::VBOrAndWidget(int x, int y, int w, int h, QWidget *parent) :
        QWidget(parent)
{
    // Create objects
    lMain = new QHBoxLayout();
    lOrBtn = new QHBoxLayout();
    lAndBtn = new QHBoxLayout();
    lOr = new QVBoxLayout();
    lAnd = new QVBoxLayout();

    dOrAddBtn = new QPushButton();
    dOrAddBtn->setIcon(QIcon(":Main/FindEngineProfileAdd.png"));
    dOrAddBtn->setIconSize(QSize(32,32));
    dOrAddBtn->setShortcut(QKeySequence(tr("Ctrl+Ins")));
    dOrAddBtn->setToolTip(tr("Insert new variant (Ctrl+Ins)"));
    dOrAddBtn->setAutoDefault(false);
    connect(dOrAddBtn,SIGNAL(clicked()),this,SLOT(addOr()));

    dOrDelBtn = new QPushButton();
    dOrDelBtn->setIcon(QIcon(":Main/FindEngineProfileDel.png"));
    dOrDelBtn->setIconSize(QSize(32,32));
    dOrDelBtn->setShortcut(QKeySequence(tr("Ctrl+Del")));
    dOrDelBtn->setToolTip(tr("Remove variant (Ctrl+Del)"));
    dOrDelBtn->setAutoDefault(false);
    connect(dOrDelBtn,SIGNAL(clicked()),this,SLOT(delOr()));

    dAndAddBtn = new QPushButton();
    dAndAddBtn->setIcon(QIcon(":Main/FindEngineProfileAdd.png"));
    dAndAddBtn->setIconSize(QSize(32,32));
    dAndAddBtn->setShortcut(QKeySequence(tr("Ins")));
    dAndAddBtn->setToolTip(tr("Insert new requirement (Ins)"));
    dAndAddBtn->setAutoDefault(false);
    connect(dAndAddBtn,SIGNAL(clicked()),this,SLOT(addAnd()));

    dAndDelBtn = new QPushButton();
    dAndDelBtn->setIcon(QIcon(":Main/FindEngineProfileDel.png"));
    dAndDelBtn->setIconSize(QSize(32,32));
    dAndDelBtn->setShortcut(QKeySequence(tr("Del")));
    dAndDelBtn->setToolTip(tr("Remove requirement (Del)"));
    dAndDelBtn->setAutoDefault(false);
    connect(dAndDelBtn,SIGNAL(clicked()),this,SLOT(delAnd()));

    dCurrRec = new QLineEdit();
    dCurrRec->setToolTip(tr("Edit current requiremen"));
    connect(dCurrRec,SIGNAL(editingFinished()),this,SLOT(requirmentChanged()));

    dOrList = new QListWidget();
    dOrList->setToolTip(tr("The list of variants. For the selection must meet the requirements of at least one variant."));
    dAndList = new QListWidget();
    dAndList->setToolTip(tr("The list of requirements. For the selection of the current variant, is need to satisfy all the requirements of this list."));
    connect(dCurrRec,SIGNAL(returnPressed()),dAndList,SLOT(setFocus()));
    connect(dOrList,SIGNAL(itemActivated(QListWidgetItem*)),this,SLOT(enterToOrList(QListWidgetItem*)));
    connect(dAndList,SIGNAL(itemActivated(QListWidgetItem*)),this,SLOT(enterToLineEdit(QListWidgetItem*)));

    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->setChildrenCollapsible(false);

    orGB = new QGroupBox(tr("Variants"),mainSplitter);
    andGB = new QGroupBox(tr("Requirements"),mainSplitter);

    // Place Widgets
    // Buttons
    lOrBtn->setAlignment(Qt::AlignLeft);
    lAndBtn->setAlignment(Qt::AlignLeft);
    lOrBtn->addWidget(dOrAddBtn);
    lOrBtn->addWidget(dOrDelBtn);
    lAndBtn->addWidget(dAndAddBtn);
    lAndBtn->addWidget(dAndDelBtn);

    // Or & And
    lOr->addLayout(lOrBtn);
    lOr->addWidget(dOrList);
    lAnd->addLayout(lAndBtn);
    lAnd->addWidget(dCurrRec);
    lAnd->addWidget(dAndList);

    // Main
    orGB->setLayout(lOr);
    andGB->setLayout(lAnd);

    lMain->addWidget(mainSplitter);


    //lMain->addWidget(orGB);
    //lMain->addWidget(andGB);
    setLayout(lMain);

     //dOrList->resize(dOrList->width()/2,dOrList->height());
    //orGB->setMinimumWidth(100);

    QSizePolicy policy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    policy.setHorizontalStretch(1);

    orGB->setSizePolicy(policy);

    policy.setHorizontalStretch(4);

    andGB->setSizePolicy(policy);

    // Set up mainsizes
    setGeometry(x,y,w,h);

    connect(dOrList,SIGNAL(itemSelectionChanged()),this,SLOT(currItemChanged()));
    connect(dAndList,SIGNAL(itemSelectionChanged()),this,SLOT(currRecChanged()));
}

VBOrAndWidget::~VBOrAndWidget()
{
    delete dCurrRec;

    delete dOrAddBtn;
    delete dOrDelBtn;
    delete dAndAddBtn;
    delete dAndDelBtn;

    delete dOrList;
    delete dAndList;


    delete lOrBtn;
    delete lAndBtn;
    delete lOr;
    delete lAnd;
    delete orGB;
    delete andGB;
    delete mainSplitter;
    delete lMain;
}

void VBOrAndWidget::addOr()
{
    mainList.append(QStringList());
    dOrList->addItem(tr("Variant ")+QString::number(mainList.count()));
    dOrList->setFocus();
    dOrList->setCurrentItem(dOrList->item(dOrList->count()-1));
}

void VBOrAndWidget::addAnd()
{
    if(!dOrList->count())
    {
        addOr();
    }
    if(!dOrList->currentIndex().isValid()) return;

    QStringList tempList = mainList.at(dOrList->currentIndex().row());
    tempList.append(QString(""));
    mainList.replace(dOrList->currentIndex().row(),tempList); // лише додається елемент в головну структуру (не в діалог)

    // А це в сам віджет
    dAndList->addItem(tr("(new requirement)"));
    dAndList->setCurrentItem(dAndList->item(dAndList->count()-1));
    dCurrRec->setFocus();
    dCurrRec->selectAll();
}

void VBOrAndWidget::delOr()
{
    if(!dOrList->currentIndex().isValid()) return;

    long cuttItem = dOrList->currentIndex().row();

    // Віджет
    dOrList->takeItem(cuttItem);

    // Головна структура
    mainList.removeAt(cuttItem);
}

void VBOrAndWidget::delAnd()
{
    long currItem = dAndList->currentIndex().row();

    // Віджет
    dAndList->takeItem(currItem);

    // Головна структура
    if(!dOrList->currentIndex().isValid()) return;
    if(!dAndList->currentIndex().isValid()) return;
    QStringList tempList = mainList.at(dOrList->currentIndex().row());
    tempList.removeAt(currItem);
    mainList.replace(dOrList->currentIndex().row(),tempList);
}

QStringList VBOrAndWidget::removeEmpty(QStringList pList)
{
    for(long i=0; i<pList.count(); i++)
    {
        if(pList.at(i).isEmpty()) pList.removeAt(i--);
    }
    return pList;
}

void VBOrAndWidget::currItemChanged()
{
    if(!dOrList->count()) return;
    dAndList->clear();
    if(!dOrList->currentIndex().isValid()) return;
    long orListRow = dOrList->currentIndex().row();
    mainList.replace(orListRow,removeEmpty(mainList.at(orListRow))); // Очистимо порожні елементи списку
    dAndList->addItems(mainList.at(orListRow));
    dCurrRec->setText("");
}

void VBOrAndWidget::currRecChanged()
{
    if(!dAndList->count()) return;
    if(!dAndList->currentIndex().isValid())
    {
        dCurrRec->setText("");
    }else
    {
        dCurrRec->setText(dAndList->currentItem()->text());
    }
}

void VBOrAndWidget::requirmentChanged()
{
    if(!dAndList->count())
    {
        QString temp = dCurrRec->text();
        addAnd();
        dCurrRec->setText(temp);
    }
    if(!dAndList->currentIndex().isValid()) return;
    QStringList tempList = mainList.at(dOrList->currentIndex().row());
    tempList.replace(dAndList->currentIndex().row(),dCurrRec->text());
    mainList.replace(dOrList->currentIndex().row(),tempList); // лише додається елемент в головну структуру (не в діалог)

    // А це в сам віджет
    QListWidgetItem* wItem = dAndList->currentItem();
    wItem->setText(dCurrRec->text());
    dAndList->setCurrentItem(wItem);
}

 void VBOrAndWidget::enterToLineEdit(QListWidgetItem*)
 {
    dCurrRec->setFocus();
    dCurrRec->selectAll();
 }

 void VBOrAndWidget::enterToOrList(QListWidgetItem*)
 {
    dAndList->setFocus();
 }

 void VBOrAndWidget::updateForm()
 {
    dCurrRec->clear();
    dAndList->clear();
    dOrList->clear();
    for(int i=1; i <= mainList.count(); i++)
    {
        dOrList->addItem(tr("Variant ")+QString::number(i));
    }
 }

 void VBOrAndWidget::setParams(QList<QStringList> lst)
 {
    disconnect(dOrList,SIGNAL(itemSelectionChanged()),this,SLOT(currItemChanged()));
    disconnect(dAndList,SIGNAL(itemSelectionChanged()),this,SLOT(currRecChanged()));
    mainList = lst;
    updateForm();
    connect(dOrList,SIGNAL(itemSelectionChanged()),this,SLOT(currItemChanged()));
    connect(dAndList,SIGNAL(itemSelectionChanged()),this,SLOT(currRecChanged()));
 }

 QList<QStringList> VBOrAndWidget::params()
 {
    return mainList;
 }
