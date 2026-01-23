#ifndef NAT_MANAGER_H
#define NAT_MANAGER_H

#include <QString>
#include <memory>

class QProcess;

class NatManager {
public:
    NatManager();
    ~NatManager();

    bool enableMasquerade(const QString &externalIface);
    bool disableMasquerade();

private:
    bool useNft() const;
    QString m_externalIface;
    std::unique_ptr<QProcess> m_helper;
};


#endif // NAT_MANAGER_H
