#ifndef VBBANNER_H
#define VBBANNER_H

#include <QtWebKitWidgets/QWebView>
#include <QDesktopServices>
#include <QtWebKitWidgets/QWebFrame>

class VBBanner : public QWebView
{
    Q_OBJECT
protected:
    virtual void mousePressEvent(QMouseEvent * event);

public:
    explicit VBBanner(QWidget *parent = 0);
    
signals:
    void pressed();
    
public slots:
    
};

#endif // VBBANNER_H
