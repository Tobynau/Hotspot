#ifndef HOTSPOT_CLIENT_H
#define HOTSPOT_CLIENT_H

#include <QObject>
#include <QString>
#include <QDBusInterface>

class HotspotClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
public:
    explicit HotspotClient(QObject *parent = nullptr);

    QString lastError() const;

    Q_INVOKABLE bool startHotspot(const QString &ssid, const QString &password);
    Q_INVOKABLE bool stopHotspot();

    QString status() const { return m_status; }

signals:
    void statusChanged(const QString &status);
    void lastErrorChanged(const QString &err);

private slots:
    void onDbusStatusChanged(const QString &status);

private:
    QDBusInterface *m_iface;
    QString m_status;
    QString m_lastError;
};

#endif // HOTSPOT_CLIENT_H
