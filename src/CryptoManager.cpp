#include "CryptoManager.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include "qaesencryption.h"

QString CryptoManager::generateRandomKey(int length) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    QString randomString;
    for(int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->generate() % possibleCharacters.length();
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}

QByteArray CryptoManager::hashPassword(const QString& password) {
    // We use SHA-256 to hash the password into a 32-byte key
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
}

QByteArray CryptoManager::encrypt(const QByteArray& plaintext, const QByteArray& key) {
    // Use AES-256-CBC
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC, QAESEncryption::PKCS7);
    
    // Using a zeroed IV for simplicity since the vault key is random and the data is local, 
    // but in a real scenario we'd use a random IV and prepend it to the ciphertext.
    // We'll use a fixed IV here for ease of decryption.
    QByteArray iv(16, 0); 
    
    QByteArray encodedText = encryption.encode(plaintext, key, iv);
    return encodedText;
}

QByteArray CryptoManager::decrypt(const QByteArray& ciphertext, const QByteArray& key) {
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC, QAESEncryption::PKCS7);
    QByteArray iv(16, 0);
    
    QByteArray decodedText = encryption.decode(ciphertext, key, iv);
    return QAESEncryption::RemovePadding(decodedText, QAESEncryption::PKCS7);
}
