#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include "VaultManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(VaultManager* vault, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onEntrySelected();
    void onCopyPasswordClicked();
    void onToggleThemeClicked();

private:
    VaultManager* m_vault;
    bool m_isDarkTheme;

    QListWidget* m_listWidget;
    
    QLabel* m_titleLabel;
    QLabel* m_usernameLabel;
    QLineEdit* m_passwordEdit;
    QLabel* m_urlLabel;
    QTextEdit* m_notesEdit;

    QPushButton* m_editBtn;
    QPushButton* m_deleteBtn;

    void refreshList();
    void updateDetailsPanel(int index);
    void setupUI();
    void applyTheme();
};

#endif // MAINWINDOW_H
