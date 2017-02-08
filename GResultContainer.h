#ifndef GRESULTCONTAINER_H
#define GRESULTCONTAINER_H

#include <QString>
#include <QUrl>

class GResultContainer
{
public:
    GResultContainer();
    GResultContainer(QUrl pUrl, short pType, QString pMsg = "");
    QUrl url;
    QString msg;
    short type;
    // Добавлено - 1;
    // Не прошел фильтр - 2
    // Ошибка - 3
};

#endif // GRESULTCONTAINER_H
