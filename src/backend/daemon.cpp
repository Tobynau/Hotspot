#include "daemon.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QProcess>

#include <QRegularExpression>
#include <QFile>

HotspotDaemon::HotspotDaemon(QObject *parent)
    : QObject(parent)
{
}

bool HotspotDaemon::startHotspot(const QString &ssid, const QString &password)
{
    qDebug() << "startHotspot called:" << ssid;

    // Validate inputs
    if (!validateSsid(ssid)) {
        qWarning() << "Invalid SSID";
        return false;
    }
    if (!validatePsk(password)) {
        qWarning() << "Invalid passphrase";
        return false;
    }

    setStatus("starting");

    // Choose a wireless interface to run hostapd on
    QString iface = chooseWirelessInterface();
    if (iface.isEmpty()) {
        qWarning() << "No wireless interface found";
        m_lastError = "No wireless interface detected";
        emit lastErrorChanged(m_lastError);
        setStatus("error: no interface");
        return false;
    }

    // Start hostapd
    if (!m_hostapd.start(iface, ssid, password)) {
        qWarning() << "hostapd failed to start";
        m_lastError = "hostapd failed to start (is hostapd installed and accessible?)";
        emit lastErrorChanged(m_lastError);
        setStatus("error: hostapd failed");
        return false;
    }

    // Attempt to detect an external internet-facing interface to enable NAT
    QString ext = detectDefaultExternalInterface();
    if (!ext.isEmpty()) {
        if (m_nat.enableMasquerade(ext)) {
            m_shared = true;
            m_externalIface = ext;
            setStatus("running (shared)");
        } else {
            qWarning() << "Failed to enable NAT on" << ext;
            m_lastError = "Failed to enable NAT (check nft/iptables)";
            emit lastErrorChanged(m_lastError);
            // still running as AP but without sharing
            setStatus("running (no-share)");
        }
    } else {
        setStatus("running");
    }

    return true;
}

bool HotspotDaemon::stopHotspot()
{
    qDebug() << "stopHotspot called";
    setStatus("stopping");

    // Disable NAT if enabled
    if (m_shared) {
        if (!m_nat.disableMasquerade()) {
            qWarning() << "Failed to disable NAT";
            m_lastError = "Failed to disable NAT";
            emit lastErrorChanged(m_lastError);
        }
        m_shared = false;
        m_externalIface.clear();
    }

    // Stop hostapd
    if (!m_hostapd.stop()) {
        qWarning() << "hostapd stop failed";
        setStatus("error: stop failed");
        return false;
    }

    m_lastError.clear();
    emit lastErrorChanged(m_lastError);
    setStatus("stopped");
    return true;
}

QString HotspotDaemon::chooseWirelessInterface() const
{
    // Simple candidate list; later replace with robust enumeration (nl80211/iw)
    const QStringList candidates = {"wlan0", "wlan1", "wlp3s0", "wlp2s0"};
    for (const QString &c : candidates) {
        if (QFile::exists(QString("/sys/class/net/%1").arg(c))) return c;
    }
    return QString();
}

QString HotspotDaemon::detectDefaultExternalInterface() const
{
    // Read /proc/net/route and find the interface with Destination == 00000000
    QFile f("/proc/net/route");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return QString();
    QTextStream in(&f);
    // skip header
    QString header = in.readLine();
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        QStringList parts = line.split('\t');
        if (parts.size() >= 2) {
            QString iface = parts[0];
            QString dest = parts[1];
            if (dest == "00000000") return iface;
        }
    }
    return QString();
}

bool HotspotDaemon::validateSsid(const QString &ssid) const
{
    // SSID: 1..32 bytes; disallow newlines and control chars
    if (ssid.isEmpty() || ssid.size() > 32) return false;
    QRegularExpression re("^[\x20-\x7E]+$");
    return re.match(ssid).hasMatch();
}

bool HotspotDaemon::validatePsk(const QString &psk) const
{
    // WPA2 PSK must be 8..63 characters when passphrase
    if (psk.size() < 8 || psk.size() > 63) return false;
    // simple printable chars check
    QRegularExpression re("^[\x20-\x7E]+$");
    return re.match(psk).hasMatch();
}
