#include "VaultManager.h"
#include "CryptoManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>

VaultManager::VaultManager(const QString& filePath)
    : m_filePath(filePath), m_isUnlocked(false)
{
}

bool VaultManager::vaultExists() const {
    return QFile::exists(m_filePath);
}

bool VaultManager::initializeVault(const QString& masterPassword, const QString& recoveryKey) {
    // Generate a random 32-byte vault key
    m_vaultKey.clear();
    for(int i = 0; i < 32; ++i) {
        m_vaultKey.append(static_cast<char>(QRandomGenerator::global()->generate()));
    }

    QByteArray masterKey = CryptoManager::hashPassword(masterPassword);
    QByteArray recoveryKeyHash = CryptoManager::hashPassword(recoveryKey);

    QByteArray encByMaster = CryptoManager::encrypt(m_vaultKey, masterKey);
    QByteArray encByRecovery = CryptoManager::encrypt(m_vaultKey, recoveryKeyHash);

    m_credentials.clear();
    m_isUnlocked = true;

    // Save the new empty vault
    QJsonObject root;
    root["vaultKey_encrypted_by_master"] = QString(encByMaster.toBase64());
    root["vaultKey_encrypted_by_recovery"] = QString(encByRecovery.toBase64());
    
    // Encrypt empty payload
    QJsonArray emptyPayload;
    QByteArray rawPayload = QJsonDocument(emptyPayload).toJson(QJsonDocument::Compact);
    QByteArray encPayload = CryptoManager::encrypt(rawPayload, m_vaultKey);
    
    root["payload"] = QString(encPayload.toBase64());

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(QJsonDocument(root).toJson());
    file.close();

    return true;
}

bool VaultManager::unlock(const QString& keyOrPassword) {
    QByteArray fileData = readVaultFile();
    if (fileData.isEmpty()) return false;

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (!doc.isObject()) return false;

    QJsonObject root = doc.object();
    QByteArray encByMaster = QByteArray::fromBase64(root["vaultKey_encrypted_by_master"].toString().toUtf8());
    QByteArray encByRecovery = QByteArray::fromBase64(root["vaultKey_encrypted_by_recovery"].toString().toUtf8());
    QByteArray encPayload = QByteArray::fromBase64(root["payload"].toString().toUtf8());

    QByteArray inputHash = CryptoManager::hashPassword(keyOrPassword);

    // Try unlocking with input acting as Master Password
    QByteArray decVaultKey = CryptoManager::decrypt(encByMaster, inputHash);
    
    // Test if this decrypted key successfully decrypts the payload
    QByteArray decPayload = CryptoManager::decrypt(encPayload, decVaultKey);
    
    if (decPayload.isEmpty() || !QJsonDocument::fromJson(decPayload).isArray()) {
        // Try unlocking with input acting as Recovery Key
        decVaultKey = CryptoManager::decrypt(encByRecovery, inputHash);
        decPayload = CryptoManager::decrypt(encPayload, decVaultKey);
        
        if (decPayload.isEmpty() || !QJsonDocument::fromJson(decPayload).isArray()) {
            return false; // Failed to decrypt with both
        }
    }

    m_vaultKey = decVaultKey;
    m_isUnlocked = true;
    loadPayload(decPayload);
    return true;
}

QList<CredentialEntry> VaultManager::getCredentials() const {
    return m_credentials;
}

void VaultManager::setCredentials(const QList<CredentialEntry>& entries) {
    m_credentials = entries;
}

void VaultManager::addCredential(const CredentialEntry& entry) {
    m_credentials.append(entry);
}

bool VaultManager::save() {
    if (!m_isUnlocked) return false;

    QByteArray fileData = readVaultFile();
    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    QJsonObject root = doc.object();

    QJsonArray payloadArr;
    for (const auto& entry : m_credentials) {
        QJsonObject obj;
        obj["title"] = entry.title;
        obj["username"] = entry.username;
        obj["password"] = entry.password;
        obj["url"] = entry.url;
        obj["notes"] = entry.notes;
        payloadArr.append(obj);
    }

    QByteArray rawPayload = QJsonDocument(payloadArr).toJson(QJsonDocument::Compact);
    QByteArray encPayload = CryptoManager::encrypt(rawPayload, m_vaultKey);

    root["payload"] = QString(encPayload.toBase64());

    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(QJsonDocument(root).toJson());
    file.close();

    return true;
}

bool VaultManager::isUnlocked() const {
    return m_isUnlocked;
}

QByteArray VaultManager::readVaultFile() const {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }
    return file.readAll();
}

void VaultManager::loadPayload(const QByteArray& decryptedPayload) {
    m_credentials.clear();
    QJsonDocument doc = QJsonDocument::fromJson(decryptedPayload);
    QJsonArray arr = doc.array();
    for (int i = 0; i < arr.size(); ++i) {
        QJsonObject obj = arr[i].toObject();
        CredentialEntry entry;
        entry.title = obj["title"].toString();
        entry.username = obj["username"].toString();
        entry.password = obj["password"].toString();
        entry.url = obj["url"].toString();
        entry.notes = obj["notes"].toString();
        m_credentials.append(entry);
    }
}
