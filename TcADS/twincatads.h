#ifndef TWINCATADS_H
#define TWINCATADS_H

#include <QString>
#include <QObject>

#include <stdbool.h>

#define BOOL char

#include "TcADS/TcAdsDef.h"
#include "TcADS/TcAdsAPI.h"

class TwinCatAds : public QObject
{
    Q_OBJECT

public:
    TwinCatAds(QObject *parent = nullptr);

    int Open();
    int Close();

    bool IsOpen();
    int InitVariables();

    int SetTag(const QString& tag);
    int SetLocalTime(const QString& localTime);
    int AddCounter();
    uint32_t GetCounter();
    uint32_t GetLastError();
    uint16_t GetPort();

private:
    AmsAddr	m_localAddr;
    AmsAddr	m_serverAddr;

    //path
    QString m_varTag;
    QString m_varLocalTime;
    QString m_varCounter;

    //index
    uint32_t m_varIndexTag;
    uint32_t m_varIndexLocalTime;
    uint32_t m_varIndexCounter;

    //data
    char m_varDataTag[32];
    char m_varDataLocalTime[32];
    uint32_t m_varDataCounter;
};

Q_DECLARE_METATYPE(TwinCatAds*)
Q_DECLARE_METATYPE(const TwinCatAds*)

#endif // TWINCATADS_H
