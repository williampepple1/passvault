#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class VaultManager;

class SetupWindow : public QDialog {
    Q_OBJECT

public:
    explicit SetupWindow(VaultManager* vault, QWidget *parent = nullptr);

signals:
    void setupComplete();

private slots:
    void onCreateClicked();

private:
    VaultManager* m_vault;
    
    QLineEdit* m_passEdit;
    QLineEdit* m_confirmEdit;
    QLabel* m_errorLabel;
    QPushButton* m_createBtn;
    
    void showRecoveryKey(const QString& recoveryKey);
};

#endif // SETUPWINDOW_H
