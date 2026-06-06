#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QString>
#include <QByteArray>

class CryptoManager {
public:
    // Generate a random string of a given length (used for generating Recovery Key)
    static QString generateRandomKey(int length = 32);

    // Hash a string (e.g., Master Password) to create a fixed length key
    static QByteArray hashPassword(const QString& password);

    // Encrypts plaintext using AES-256-CBC
    // The key must be exactly 32 bytes.
    static QByteArray encrypt(const QByteArray& plaintext, const QByteArray& key);

    // Decrypts ciphertext using AES-256-CBC
    // The key must be exactly 32 bytes.
    static QByteArray decrypt(const QByteArray& ciphertext, const QByteArray& key);
};

#endif // CRYPTOMANAGER_H
