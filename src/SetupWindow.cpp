#include "SetupWindow.h"
#include "VaultManager.h"
#include "CryptoManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

SetupWindow::SetupWindow(VaultManager* vault, QWidget *parent)
    : QDialog(parent), m_vault(vault)
{
    setWindowTitle("Setup Master Password");
    setFixedSize(400, 250);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("Welcome to PassVault");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(titleLabel);

    QLabel* descLabel = new QLabel("Please create a strong master password to encrypt your vault.");
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);

    m_passEdit = new QLineEdit();
    m_passEdit->setEchoMode(QLineEdit::Password);
    m_passEdit->setPlaceholderText("Master Password");
    layout->addWidget(m_passEdit);

    m_confirmEdit = new QLineEdit();
    m_confirmEdit->setEchoMode(QLineEdit::Password);
    m_confirmEdit->setPlaceholderText("Confirm Password");
    layout->addWidget(m_confirmEdit);

    m_errorLabel = new QLabel("");
    m_errorLabel->setStyleSheet("color: red;");
    layout->addWidget(m_errorLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_createBtn = new QPushButton("Create Vault");
    QPushButton* exitBtn = new QPushButton("Exit");
    btnLayout->addStretch();
    btnLayout->addWidget(exitBtn);
    btnLayout->addWidget(m_createBtn);
    
    layout->addLayout(btnLayout);

    connect(m_createBtn, &QPushButton::clicked, this, &SetupWindow::onCreateClicked);
    connect(exitBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void SetupWindow::onCreateClicked() {
    QString pass = m_passEdit->text();
    QString confirm = m_confirmEdit->text();

    if (pass.isEmpty()) {
        m_errorLabel->setText("Password cannot be empty.");
        return;
    }

    if (pass != confirm) {
        m_errorLabel->setText("Passwords do not match.");
        return;
    }

    // Generate Recovery Key
    QString recoveryKey = CryptoManager::generateRandomKey(32);

    if (m_vault->initializeVault(pass, recoveryKey)) {
        showRecoveryKey(recoveryKey);
    } else {
        m_errorLabel->setText("Failed to create vault file.");
    }
}

void SetupWindow::showRecoveryKey(const QString& recoveryKey) {
    QDialog dialog(this);
    dialog.setWindowTitle("Save Your Recovery Key!");
    dialog.setFixedSize(450, 200);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    QLabel* infoLabel = new QLabel("Your vault has been created. A recovery key has been generated.\n\n"
                                   "If you forget your master password, you can use this key to unlock your vault.\n"
                                   "Please save this key in a safe place. It will not be shown again.");
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    QHBoxLayout* keyLayout = new QHBoxLayout();
    QLineEdit* keyEdit = new QLineEdit(recoveryKey);
    keyEdit->setReadOnly(true);
    keyLayout->addWidget(keyEdit);

    QPushButton* copyBtn = new QPushButton("Copy");
    keyLayout->addWidget(copyBtn);
    layout->addLayout(keyLayout);

    connect(copyBtn, &QPushButton::clicked, [&keyEdit]() {
        QApplication::clipboard()->setText(keyEdit->text());
        QMessageBox::information(keyEdit->window(), "Copied", "Recovery key copied to clipboard!");
    });

    QPushButton* okBtn = new QPushButton("I have saved it");
    layout->addStretch();
    layout->addWidget(okBtn, 0, Qt::AlignRight);

    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    dialog.exec();

    accept();
    emit setupComplete();
}
