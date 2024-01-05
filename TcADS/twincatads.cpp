#include "twincatads.h"

#include <QDebug>

using namespace Qt::StringLiterals;

TwinCatAds::TwinCatAds(QObject *parent)
    : QObject(parent)
{
    //ADS
    uint32_t ads_ver = AdsGetDllVersion();
    qDebug() << u"AdsGetDllVersion: %1"_s.arg(ads_ver);

    //169.254.192.231.1.1
    AmsAddr	serverAddr = {{169, 254, 192, 231, 1, 1}, 851};
    m_serverAddr = serverAddr;

    m_varTag = "MAIN.fbPACK_Station.fbTurckRFID.Tag";
    m_varLocalTime = "MAIN.fbPACK_Station.fbTurckRFID.LocalTime";
    m_varCounter = "MAIN.fbPACK_Station.fbTurckRFID.Counter";

    memset(m_varDataTag, 0, sizeof(m_varDataTag));
    memset(m_varDataLocalTime, 0, sizeof(m_varDataLocalTime));
    m_varDataCounter = 0;
}

int TwinCatAds::Open()
{
    uint32_t ads_port = AdsPortOpen();
    qDebug() << u"AdsPortOpen: %1"_s.arg(ads_port);

    uint32_t ads_err = AdsGetLocalAddress(&m_localAddr);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << u"AdsGetLocalAddress error: %1"_s.arg(ads_err);
        return -1;
    }

    qDebug() << u"ADS local port: %1"_s.arg(m_localAddr.port);
    qDebug() << u"ADS local address: %1.%2.%3.%4.%5.%6"_s.arg(m_localAddr.netId.b[0])
                    .arg(m_localAddr.netId.b[1])
                    .arg(m_localAddr.netId.b[2])
                    .arg(m_localAddr.netId.b[3])
                    .arg(m_localAddr.netId.b[4])
                    .arg(m_localAddr.netId.b[5]);

    return 0;
}

uint32_t TwinCatAds::GetLastError()
{
    return AdsGetLastError();
}

uint16_t TwinCatAds::GetPort()
{
    return m_localAddr.port;
}

bool TwinCatAds::IsOpen()
{
    char open = false;
    uint32_t ads_err = AdsAmsPortEnabled(&open);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << u"AdsAmsPortEnabled error: %1"_s.arg(ads_err);
        return false;
    }

    return open;
}

int TwinCatAds::InitVariables()
{
    //Tag
    uint32_t ads_err = AdsSyncReadWriteReq(&m_serverAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(m_varIndexTag), &m_varIndexTag, strlen(m_varTag.toLatin1().data()), m_varTag.toLatin1().data());
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncReadWriteReq(" << m_varTag << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    ads_err = AdsSyncReadReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexTag, sizeof(m_varDataTag), m_varDataTag);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncReadReq(" << m_varTag << u") error: %1"_s.arg(ads_err);
        return -1;
    }
    qDebug() << m_varTag << ": "<< m_varDataTag;

    memset(m_varDataTag, 0, sizeof(m_varDataTag));
    ads_err = AdsSyncWriteReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexTag, sizeof(m_varDataTag), m_varDataTag);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncWriteReq(" << m_varTag << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    //LocalTime
    ads_err = AdsSyncReadWriteReq(&m_serverAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(m_varIndexLocalTime), &m_varIndexLocalTime, strlen(m_varLocalTime.toLatin1().data()), m_varLocalTime.toLatin1().data());
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncReadWriteReq(" << m_varLocalTime << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    ads_err = AdsSyncReadReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexLocalTime, sizeof(m_varDataLocalTime), m_varDataLocalTime);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncReadReq(" << m_varLocalTime << u") error: %1"_s.arg(ads_err);
        return -1;
    }
    qDebug() << m_varLocalTime << ": "<< m_varDataLocalTime;

    memset(m_varDataLocalTime, 0, sizeof(m_varDataLocalTime));
    ads_err = AdsSyncWriteReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexLocalTime, sizeof(m_varDataLocalTime), m_varDataLocalTime);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncWriteReq(" << m_varLocalTime << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    //Counter
    ads_err = AdsSyncReadWriteReq(&m_serverAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(m_varIndexCounter), &m_varIndexCounter, strlen(m_varCounter.toLatin1().data()), m_varCounter.toLatin1().data());
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncReadWriteReq(" << m_varCounter << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    ads_err = AdsSyncReadReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexCounter, sizeof(m_varDataCounter), &m_varDataCounter);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncReadReq(" << m_varCounter << u") error: %1"_s.arg(ads_err);
        return -1;
    }
    qDebug() << m_varCounter << ": "<< m_varDataCounter;

    return 0;
}

int TwinCatAds::SetTag(const QString& tag)
{
    memset(m_varDataTag, 0, sizeof(m_varDataTag));
    strncpy_s(m_varDataTag, tag.toLatin1().data(), sizeof(m_varDataTag));

    uint32_t ads_err = AdsSyncWriteReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexTag, sizeof(m_varDataTag), m_varDataTag);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncWriteReq(" << m_varTag << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    return 0;
}

int TwinCatAds::SetLocalTime(const QString& localTime)
{
    memset(m_varDataLocalTime, 0, sizeof(m_varDataLocalTime));
    strncpy_s(m_varDataLocalTime, localTime.toLatin1().data(), sizeof(m_varDataLocalTime));

    uint32_t ads_err = AdsSyncWriteReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexLocalTime, sizeof(m_varDataLocalTime), m_varDataLocalTime);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncWriteReq(" << m_varLocalTime << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    return 0;
}

int TwinCatAds::AddCounter()
{
    m_varDataCounter++;

    uint32_t ads_err = AdsSyncWriteReq(&m_serverAddr, ADSIGRP_SYM_VALBYHND, m_varIndexCounter, sizeof(m_varDataCounter), &m_varDataCounter);
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << "AdsSyncWriteReq(" << m_varCounter << u") error: %1"_s.arg(ads_err);
        return -1;
    }

    return 0;
}

uint32_t TwinCatAds::GetCounter()
{
    return m_varDataCounter;
}

int TwinCatAds::Close()
{
    uint32_t ads_err = AdsPortClose();
    if(ads_err != ADSERR_NOERR)
    {
        qDebug() << u"AdsPortClose error: %1"_s.arg(ads_err);
        return -1;
    }

    return 0;
}
