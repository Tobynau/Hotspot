#ifndef SECRET_STORE_H
#define SECRET_STORE_H

#include <QString>

class SecretStore {
public:
    SecretStore();
    ~SecretStore();

    bool storePassphrase(const QString &id, const QString &passphrase);
    QString retrievePassphrase(const QString &id);
};

#endif // SECRET_STORE_H
