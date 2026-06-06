#ifndef ENTRYDIALOG_H
#define ENTRYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include "VaultManager.h"

class EntryDialog : public QDialog {
    Q_OBJECT

public:
    explicit EntryDialog(QWidget *parent = nullptr);
    void setEntry(const CredentialEntry& entry);
    CredentialEntry getEntry() const;

private slots:
    void onSaveClicked();
    void onGeneratePasswordClicked();

private:
    QLineEdit* m_titleEdit;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QLineEdit* m_urlEdit;
    QTextEdit* m_notesEdit;
};

#endif // ENTRYDIALOG_H
