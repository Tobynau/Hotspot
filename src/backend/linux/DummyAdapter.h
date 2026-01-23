#ifndef DUMMY_ADAPTER_H
#define DUMMY_ADAPTER_H

#include "../PlatformAdapter.h"

class DummyAdapter : public IPlatformAdapter
{
public:
    DummyAdapter();
    ~DummyAdapter() override;

    bool supportsApMode() const override;
    bool startAp(const QString &interface, const QString &ssid, const QString &psk) override;
    bool stopAp() override;
};

#endif // DUMMY_ADAPTER_H
