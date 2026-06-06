#include "EntryDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "CryptoManager.h"

EntryDialog::EntryDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Credential Entry");
    setMinimumWidth(400);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout();

    m_titleEdit = new QLineEdit();
    m_usernameEdit = new QLineEdit();
    m_passwordEdit = new QLineEdit();
    m_urlEdit = new QLineEdit();
    m_notesEdit = new QTextEdit();

    formLayout->addRow("Title:", m_titleEdit);
    formLayout->addRow("Username:", m_usernameEdit);
    
    QHBoxLayout* passLayout = new QHBoxLayout();
    passLayout->addWidget(m_passwordEdit);
    QPushButton* generateBtn = new QPushButton("Generate");
    passLayout->addWidget(generateBtn);
    formLayout->addRow("Password:", passLayout);
    
    formLayout->addRow("URL:", m_urlEdit);
    formLayout->addRow("Notes:", m_notesEdit);

    mainLayout->addLayout(formLayout);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* saveBtn = new QPushButton("Save");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    btnLayout->addStretch();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);

    mainLayout->addLayout(btnLayout);

    connect(saveBtn, &QPushButton::clicked, this, &EntryDialog::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(generateBtn, &QPushButton::clicked, this, &EntryDialog::onGeneratePasswordClicked);
}

void EntryDialog::setEntry(const CredentialEntry& entry) {
    m_titleEdit->setText(entry.title);
    m_usernameEdit->setText(entry.username);
    m_passwordEdit->setText(entry.password);
    m_urlEdit->setText(entry.url);
    m_notesEdit->setPlainText(entry.notes);
}

CredentialEntry EntryDialog::getEntry() const {
    CredentialEntry entry;
    entry.title = m_titleEdit->text();
    entry.username = m_usernameEdit->text();
    entry.password = m_passwordEdit->text();
    entry.url = m_urlEdit->text();
    entry.notes = m_notesEdit->toPlainText();
    return entry;
}

void EntryDialog::onSaveClicked() {
    if (m_titleEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Title cannot be empty.");
        return;
    }
    accept();
}

void EntryDialog::onGeneratePasswordClicked() {
    m_passwordEdit->setText(CryptoManager::generateRandomKey(16));
}
