#ifndef NETWORKMANAGER_ADAPTER_H
#define NETWORKMANAGER_ADAPTER_H

#include "../PlatformAdapter.h"
#include <QString>

class NetworkManagerAdapter : public IPlatformAdapter
{
public:
    NetworkManagerAdapter();
    ~NetworkManagerAdapter() override;

    bool supportsApMode() const override;
    bool startAp(const QString &interface, const QString &ssid, const QString &psk) override;
    bool stopAp() override;
};

#endif // NETWORKMANAGER_ADAPTER_H
