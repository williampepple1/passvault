#ifndef VAULTMANAGER_H
#define VAULTMANAGER_H

#include <QString>
#include <QList>
#include <QByteArray>

struct CredentialEntry {
    QString title;
    QString username;
    QString password;
    QString url;
    QString notes;
};

class VaultManager {
public:
    VaultManager(const QString& filePath);

    // Checks if the vault file exists
    bool vaultExists() const;

    // Initializes a new vault with a given master password and recovery key
    // Returns true on success
    bool initializeVault(const QString& masterPassword, const QString& recoveryKey);

    // Attempts to unlock the vault using either the master password or recovery key
    // Returns true if successful
    bool unlock(const QString& keyOrPassword);

    // Get all credentials
    QList<CredentialEntry> getCredentials() const;

    // Set credentials (this doesn't save to disk)
    void setCredentials(const QList<CredentialEntry>& entries);

    // Add a single credential
    void addCredential(const CredentialEntry& entry);

    // Save current credentials to disk. Requires the vault to be unlocked.
    bool save();

    // Check if vault is unlocked
    bool isUnlocked() const;

private:
    QString m_filePath;
    bool m_isUnlocked;
    QByteArray m_vaultKey; // The decrypted raw 32-byte AES key for the payload
    QList<CredentialEntry> m_credentials;

    // Read the entire vault file
    QByteArray readVaultFile() const;

    // Parse the payload JSON to populate m_credentials
    void loadPayload(const QByteArray& decryptedPayload);
};

#endif // VAULTMANAGER_H
