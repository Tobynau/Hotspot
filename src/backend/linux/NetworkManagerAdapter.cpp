#include "NetworkManagerAdapter.h"
#include <QDebug>

NetworkManagerAdapter::NetworkManagerAdapter()
{
}

NetworkManagerAdapter::~NetworkManagerAdapter()
{
}

bool NetworkManagerAdapter::supportsApMode() const
{
    // TODO: query NetworkManager / nl80211 capabilities
    return true;
}

bool NetworkManagerAdapter::startAp(const QString &interface, const QString &ssid, const QString &psk)
{
    qDebug() << "NetworkManagerAdapter::startAp" << interface << ssid;
    // TODO: implement DBus calls to NetworkManager
    return true;
}

bool NetworkManagerAdapter::stopAp()
{
    qDebug() << "NetworkManagerAdapter::stopAp";
    return true;
}
