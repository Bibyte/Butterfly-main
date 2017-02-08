#include "GResultContainer.h"

GResultContainer::GResultContainer()
{
}

GResultContainer::GResultContainer(QUrl pUrl, short pType, QString pMsg)
{
    url = pUrl;
    type = pType;
    msg = pMsg;
}
