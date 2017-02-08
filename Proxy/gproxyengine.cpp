#include "gproxyengine.h"

GProxyEngine::GProxyEngine(QObject *parent) :
    QObject(parent)
{
    LoadProxyListFromFile();
}

void GProxyEngine::LoadProxyListFromFile()
{

}
