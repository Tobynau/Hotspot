#ifndef HOSTAPD_CONTROLLER_H
#define HOSTAPD_CONTROLLER_H

#include <QString>
#include <memory>

class QProcess;

class HostapdController {
public:
    HostapdController();
    ~HostapdController();

    bool start(const QString &iface, const QString &ssid, const QString &psk);
    bool stop();
    bool isRunning() const;

private:
    QString makeConfig(const QString &iface, const QString &ssid, const QString &psk) const;
    QString m_configPath;
    std::unique_ptr<QProcess> m_process;
};

#endif // HOSTAPD_CONTROLLER_H