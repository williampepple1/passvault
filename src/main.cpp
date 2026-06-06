#include <QApplication>
#include "VaultManager.h"
#include "SetupWindow.h"
#include "LoginWindow.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    VaultManager vault("vault.dat");

    if (!vault.vaultExists()) {
        SetupWindow setup(&vault);
        if (setup.exec() != QDialog::Accepted) {
            return 0; // Setup cancelled or exited
        }
    } else {
        LoginWindow login(&vault);
        if (login.exec() != QDialog::Accepted) {
            return 0; // Login cancelled or failed
        }
    }

    MainWindow mainWindow(&vault);
    mainWindow.show();

    return app.exec();
}
