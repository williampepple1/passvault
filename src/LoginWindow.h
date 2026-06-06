#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class VaultManager;

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    explicit LoginWindow(VaultManager* vault, QWidget *parent = nullptr);

signals:
    void loginSuccessful();

private slots:
    void onLoginClicked();

private:
    VaultManager* m_vault;
    
    QLineEdit* m_passEdit;
    QLabel* m_errorLabel;
    QPushButton* m_loginBtn;
};

#endif // LOGINWINDOW_H
