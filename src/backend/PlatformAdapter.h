#ifndef I_PLATFORM_ADAPTER_H
#define I_PLATFORM_ADAPTER_H

#include <QString>

class IPlatformAdapter {
public:
    virtual ~IPlatformAdapter() = default;
    virtual bool supportsApMode() const = 0;
    virtual bool startAp(const QString &interface, const QString &ssid, const QString &psk) = 0;
    virtual bool stopAp() = 0;
};

#endif // I_PLATFORM_ADAPTER_H
