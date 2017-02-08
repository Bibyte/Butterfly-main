#ifndef VBFINDPROFILE_H
#define VBFINDPROFILE_H

#include <QString>
#include <QStringList>
#include <QDate>

class VBFindProfile
{
public:
    QString sName; // - Імя профілю
    QString sDescription; // - Опис профілю
    QDate sDate;

    // Проверка на PHP
    bool sCheckSpecificURL; //- Флаг проверки на PHP
    QString sSpecificURL; //- Локальный дарес страницы для проверки на PHP

    QList<QStringList> sWhiteList;
    QList<QStringList> sBlackList;
public:
    VBFindProfile();
    VBFindProfile(QString pName,QString pDescription, QDate pDate, bool pCheckSpecificURL, QString pSpecificURL, QList<QStringList> pWhiteList, QList<QStringList> pBlackList);
};

#endif // VBFINDPROFILE_H
