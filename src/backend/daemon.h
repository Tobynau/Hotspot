#ifndef HOTSPOT_DAEMON_H
#define HOTSPOT_DAEMON_H

#include <QObject>
#include <QString>
#include "HostapdController.h"
#include "nat/NatManager.h"

class HotspotDaemon : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.example.hotspot.Manager")
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
public:
    explicit HotspotDaemon(QObject *parent = nullptr);

    Q_INVOKABLE bool startHotspot(const QString &ssid, const QString &password);
    Q_INVOKABLE bool stopHotspot();

    QString status() const { return m_status; }
    QString lastError() const { return m_lastError; }

signals:
    void statusChanged(const QString &status);
    void lastErrorChanged(const QString &err);

private:
    void setStatus(const QString &s) {
        if (m_status != s) {
            m_status = s;
            emit statusChanged(m_status);
        }
    }

    QString m_status;
    QString m_lastError;
    HostapdController m_hostapd;
    NatManager m_nat;
    bool m_shared = false;
    QString m_externalIface;
    QString chooseWirelessInterface() const;
    QString detectDefaultExternalInterface() const;
    bool validateSsid(const QString &ssid) const;
    bool validatePsk(const QString &psk) const;
};

#endif // HOTSPOT_DAEMON_H
