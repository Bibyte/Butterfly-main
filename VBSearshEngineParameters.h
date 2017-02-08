#ifndef VBSEARSHENGINEPARAMETERS_H
#define VBSEARSHENGINEPARAMETERS_H

#include <QString>
#include <QDate>
#include <VBRUCapcha.h>

class VBSearshEngineParameters
{
public:
    QString sName; // - Імя профілю
    QString sDescription; //- Описание поискового профиля
    QDate sDate;
    QString sSearshHost; // - Адрес страницы поиска у поисковика
    QString sSearshMachineBrand; // - Фраза, определяющая ссылку как ссылку на кеш.
    QString sLastPageCheck; // - фраза, определяющая страницу, которая означает, что результаты у поисковика закончились
    QString sSpamCheck; // - фраза, определяющая страницу, которую поисковик выдал в подозрении на спам
    QString sNoResultsCheck; // - фраза, определяющая страницу, которая означает, что результатов нет
    int sStartPage; // - Начальная страница поиска
    int sPageStep; // - Шаг (у гугл например шаг равняется количеству результатов)
    bool sLastPageCheckBox; //Якщо істина, то строка має бути в документі, якщо лож - то не має бути
    bool sSpamCheckBox; //Якщо істина, то строка має бути в документі, якщо лож - то не має бути
    bool sNoResultsCheckBox; //Якщо істина, то строка має бути в документі, якщо лож - то не має бути
    int sMinWait; // Мінімальна кількість секунд очікування перед зміною сторінки пошуковика
    int sMaxWait; // Максимальна кількість секунд очікування перед зміною сторінки пошуковика
    QString sSessionParams;
    VBRUCapcha sCaptchaParams;
public:
    VBSearshEngineParameters();
    VBSearshEngineParameters(
            QString pName,
            QString pDescription,
            QDate pDate,
            QString pSearshHost,
            QString pSearshMachineBrand,
            QString pLastPageCheck,
            QString pSpamCheck,
            QString pNoResultsCheck,
            int pStartPage,
            int pPageStep,
            bool pLastPageCheckBox,
            bool pSpamCheckBox,
            bool pNoResultsCheckBox,
            int pMinWait,
            int pMaxWait,
            QString pSessionParams,
            VBRUCapcha pCaptchaParams);
};

#endif // VBSEARSHENGINEPARAMETERS_H
