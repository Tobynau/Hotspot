#include "NatManager.h"
#include <QDebug>

#include <QProcess>
#include <QStandardPaths>

NatManager::NatManager() {}
NatManager::~NatManager() {}

bool NatManager::useNft() const
{
    QString nft = QStandardPaths::findExecutable("nft");
    return !nft.isEmpty();
}

static int runCommand(const QString &program, const QStringList &args)
{
    QProcess p;
    p.start(program, args);
    if (!p.waitForFinished(15000)) return -1;
    return p.exitCode();
}

bool NatManager::enableMasquerade(const QString &externalIface)
{
    qDebug() << "NatManager::enableMasquerade" << externalIface;
    m_externalIface = externalIface;

    // Enable IPv4 forwarding
    int rc = runCommand("sysctl", {"-w", "net.ipv4.ip_forward=1"});
    if (rc != 0) qWarning() << "sysctl failed rc=" << rc;

    if (useNft()) {
        // Create NAT table and POSTROUTING chain and add masquerade rule
        runCommand("nft", {"add", "table", "ip", "nat"});
        runCommand("nft", {"add", "chain", "ip", "nat", "POSTROUTING", "{", "type", "nat", "hook", "postrouting", "priority", "100", ";", "}"});
        QString rule = QString("add rule ip nat POSTROUTING oifname \"%1\" masquerade").arg(externalIface);
        runCommand("nft", {"-f", "-"}); // no-op to ensure nft is responsive
        // Use echo to feed the rule (safer to create via temp file but keep simple)
        QProcess p;
        p.start("sh", {"-c", QString("nft 'add rule ip nat POSTROUTING oifname \"%1\" masquerade'" ).arg(externalIface)});
        p.waitForFinished(5000);
        return p.exitCode() == 0;
    } else {
        // Fallback to iptables
        int rc2 = runCommand("iptables", {"-t", "nat", "-A", "POSTROUTING", "-o", externalIface, "-j", "MASQUERADE"});
        return rc2 == 0;
    }
}

bool NatManager::disableMasquerade()
{
    qDebug() << "NatManager::disableMasquerade";
    if (useNft()) {
        // Try to delete the rule and table
        if (!m_externalIface.isEmpty()) {
            QProcess p;
            p.start("sh", {"-c", QString("nft 'delete rule ip nat POSTROUTING oifname \"%1\" masquerade'" ).arg(m_externalIface)});
            p.waitForFinished(5000);
        }
        runCommand("nft", {"delete", "table", "ip", "nat"});
        return true;
    } else {
        if (m_externalIface.isEmpty()) return true;
        int rc = runCommand("iptables", {"-t", "nat", "-D", "POSTROUTING", "-o", m_externalIface, "-j", "MASQUERADE"});
        return rc == 0;
    }
}

