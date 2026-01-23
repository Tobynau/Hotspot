#include <QCoreApplication>
#include <QDebug>
#include "../backend/daemon.h"
#include <QDBusConnection>
#include <QDBusError>
#include <QDBusConnectionInterface>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    qDebug() << "hotspot-daemon: starting (privileged stub)";
    HotspotDaemon daemon;

    // Choose DBus bus: system by default, session when --dev provided
    bool devMode = false;
    for (int i = 1; i < argc; ++i) {
        if (QString(argv[i]) == "--dev") { devMode = true; break; }
    }

    QDBusConnection bus = devMode ? QDBusConnection::sessionBus() : QDBusConnection::systemBus();
    if (!bus.isConnected()) {
        qWarning() << "DBus bus not connected:" << bus.lastError().message();
    } else {
        const QString serviceName = "org.example.hotspot";
        if (!bus.registerService(serviceName)) {
            qWarning() << "Failed to register DBus service:" << bus.lastError().message();
            if (!devMode) {
                qWarning() << "Hint: when testing locally run with --dev to use the session bus.";
            }
        }

        bool ok = bus.registerObject("/org/example/hotspot", &daemon,
                                      QDBusConnection::ExportAllSlots |
                                      QDBusConnection::ExportAllSignals |
                                      QDBusConnection::ExportAllInvokables);
        if (!ok) {
            qWarning() << "Failed to register DBus object:" << bus.lastError().message();
        } else {
            qDebug() << "DBus service registered at" << serviceName << "/org/example/hotspot" << "(devMode=" << devMode << ")";
        }
    }

    return app.exec();
}
