#include "HotspotClient.h"
#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>

HotspotClient::HotspotClient(QObject *parent)
    : QObject(parent), m_iface(nullptr)
{
    // Connect to system bus service
    // Try system bus first, fall back to session bus for dev mode
    QDBusConnection bus = QDBusConnection::systemBus();
    m_iface = new QDBusInterface("org.example.hotspot",
                                 "/org/example/hotspot",
                                 "org.example.hotspot.Manager",
                                 bus, this);

    if (!m_iface->isValid()) {
        qWarning() << "HotspotClient: system bus lookup failed:" << bus.lastError().message();
        // try session bus
        bus = QDBusConnection::sessionBus();
        m_iface->deleteLater();
        m_iface = new QDBusInterface("org.example.hotspot",
                                     "/org/example/hotspot",
                                     "org.example.hotspot.Manager",
                                     bus, this);
        if (!m_iface->isValid()) {
            qWarning() << "HotspotClient: session bus lookup failed:" << bus.lastError().message();
        } else {
            qDebug() << "HotspotClient: connected via session bus (dev mode)";
        }
    }

    // Listen for StatusChanged signals from the daemon on whatever bus we selected
    bus.connect("org.example.hotspot",
                "/org/example/hotspot",
                "org.example.hotspot.Manager",
                "statusChanged",
                this,
                SLOT(onDbusStatusChanged(QString)));
}

bool HotspotClient::startHotspot(const QString &ssid, const QString &password)
{
    if (!m_iface || !m_iface->isValid()) return false;
    QDBusReply<bool> reply = m_iface->call("startHotspot", ssid, password);
    if (reply.isValid()) {
        bool ok = reply.value();
        if (!ok) {
            // query last error from daemon
            QDBusReply<QString> err = m_iface->call("lastError");
            if (err.isValid()) {
                m_lastError = err.value();
                emit lastErrorChanged(m_lastError);
            }
        }
        return ok;
    }
    qWarning() << "startHotspot DBus call failed:" << m_iface->lastError().message();
    // request lastError if possible
    QDBusReply<QString> err = m_iface->call("lastError");
    if (err.isValid()) { m_lastError = err.value(); emit lastErrorChanged(m_lastError); }
    return false;
}

bool HotspotClient::stopHotspot()
{
    if (!m_iface || !m_iface->isValid()) return false;
    QDBusReply<bool> reply = m_iface->call("stopHotspot");
    if (reply.isValid()) {
        bool ok = reply.value();
        if (!ok) {
            QDBusReply<QString> err = m_iface->call("lastError");
            if (err.isValid()) { m_lastError = err.value(); emit lastErrorChanged(m_lastError); }
        }
        return ok;
    }
    qWarning() << "stopHotspot DBus call failed:" << m_iface->lastError().message();
    QDBusReply<QString> err = m_iface->call("lastError");
    if (err.isValid()) { m_lastError = err.value(); emit lastErrorChanged(m_lastError); }
    return false;
}

void HotspotClient::onDbusStatusChanged(const QString &status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(m_status);
    }
}

QString HotspotClient::lastError() const { return m_lastError; }
