#include "LoginWindow.h"
#include "VaultManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginWindow::LoginWindow(VaultManager* vault, QWidget *parent)
    : QDialog(parent), m_vault(vault)
{
    setWindowTitle("Unlock Vault");
    setFixedSize(400, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("PassVault Locked");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(titleLabel);

    QLabel* descLabel = new QLabel("Enter your Master Password or Recovery Key to unlock.");
    layout->addWidget(descLabel);

    m_passEdit = new QLineEdit();
    m_passEdit->setEchoMode(QLineEdit::Password);
    m_passEdit->setPlaceholderText("Master Password or Recovery Key");
    layout->addWidget(m_passEdit);

    m_errorLabel = new QLabel("");
    m_errorLabel->setStyleSheet("color: red;");
    layout->addWidget(m_errorLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    m_loginBtn = new QPushButton("Unlock");
    QPushButton* exitBtn = new QPushButton("Exit");
    btnLayout->addStretch();
    btnLayout->addWidget(exitBtn);
    btnLayout->addWidget(m_loginBtn);
    
    layout->addLayout(btnLayout);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(exitBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_passEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::onLoginClicked() {
    QString pass = m_passEdit->text();

    if (pass.isEmpty()) {
        m_errorLabel->setText("Please enter your password.");
        return;
    }

    if (m_vault->unlock(pass)) {
        accept();
        emit loginSuccessful();
    } else {
        m_errorLabel->setText("Incorrect password or recovery key.");
        m_passEdit->clear();
    }
}
