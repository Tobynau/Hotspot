#include "HostapdController.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QStandardPaths>
#include <QProcess>

HostapdController::HostapdController() {}
HostapdController::~HostapdController() {}

QString HostapdController::makeConfig(const QString &iface, const QString &ssid, const QString &psk) const
{
    QString cfg;
    cfg += "interface=" + iface + "\n";
    cfg += "driver=nl80211\n";
    cfg += "ssid=" + ssid + "\n";
    cfg += "hw_mode=g\n";
    cfg += "channel=6\n";
    cfg += "wmm_enabled=1\n";
    cfg += "macaddr_acl=0\n";
    cfg += "auth_algs=1\n";
    cfg += "ignore_broadcast_ssid=0\n";
    cfg += "wpa=2\n";
    cfg += "wpa_key_mgmt=WPA-PSK\n";
    cfg += "rsn_pairwise=CCMP\n";
    cfg += "wpa_passphrase=" + psk + "\n";
    return cfg;
}

bool HostapdController::start(const QString &iface, const QString &ssid, const QString &psk)
{
    qDebug() << "HostapdController::start" << iface << ssid;

    QString cfg = makeConfig(iface, ssid, psk);

    QString dataDir = "/var/lib/hotspot";
    if (!QDir(dataDir).exists()) {
        if (!QDir().mkpath(dataDir)) {
            // fallback to /tmp/hotspot
            dataDir = QDir::tempPath() + "/hotspot";
            QDir().mkpath(dataDir);
        }
    }

    m_configPath = dataDir + "/hostapd.conf";
    QFile f(m_configPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to write hostapd config to" << m_configPath;
        return false;
    }
    QTextStream ts(&f);
    ts << cfg;
    f.close();

    // Restrict permissions to owner read/write only
    QFile::setPermissions(m_configPath, QFile::ReadOwner | QFile::WriteOwner);

    m_process.reset(new QProcess());
    QString program = QStandardPaths::findExecutable("hostapd");
    if (program.isEmpty()) {
        qWarning() << "hostapd executable not found in PATH";
        return false;
    }

    QStringList args;
    args << m_configPath;

    m_process->start(program, args);
    if (!m_process->waitForStarted(3000)) {
        qWarning() << "Failed to start hostapd:" << m_process->errorString();
        m_process.reset();
        return false;
    }

    qDebug() << "hostapd started with PID" << m_process->processId();
    return true;
}

bool HostapdController::stop()
{
    qDebug() << "HostapdController::stop";
    if (m_process && m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
        if (!m_process->waitForFinished(3000)) {
            m_process->kill();
            m_process->waitForFinished(2000);
        }
        m_process.reset();
        return true;
    }
    return false;
}

bool HostapdController::isRunning() const
{
    return m_process && m_process->state() != QProcess::NotRunning;
}
