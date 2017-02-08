#include "VBBanner.h"

VBBanner::VBBanner(QWidget *parent) :
    QWebView(parent)
{
    setCursor(QCursor(Qt::PointingHandCursor));
    page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);


}

void VBBanner::mousePressEvent(QMouseEvent * event)
{
    emit pressed();
}
