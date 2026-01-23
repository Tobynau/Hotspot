#include "DummyAdapter.h"
#include <QDebug>

DummyAdapter::DummyAdapter() {}
DummyAdapter::~DummyAdapter() {}

bool DummyAdapter::supportsApMode() const
{
    qDebug() << "DummyAdapter::supportsApMode -> true";
    return true;
}

bool DummyAdapter::startAp(const QString &interface, const QString &ssid, const QString &psk)
{
    qDebug() << "DummyAdapter::startAp" << interface << ssid;
    return true;
}

bool DummyAdapter::stopAp()
{
    qDebug() << "DummyAdapter::stopAp";
    return true;
}
