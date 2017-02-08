#ifndef VBPOSTSESSIONENGINE_H
#define VBPOSTSESSIONENGINE_H

#include <QObject>
#include <QStringList>
#include <QString>

class VBPOSTSessionEngine : public QObject
{
    Q_OBJECT
protected:
    QStringList sSessionVars;
    QString sSessionString;
    QString getSessionValue(QString pParamName, QString pParamString);
public:
    explicit VBPOSTSessionEngine(QObject *parent = 0);
    void addParam(QString pParamName);
    void writeParams(QString pParamString);
    QString getParamString();

signals:

public slots:

};

#endif // VBPOSTSESSIONENGINE_H
