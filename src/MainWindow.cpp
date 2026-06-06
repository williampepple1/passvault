#include "MainWindow.h"
#include "EntryDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QFile>

MainWindow::MainWindow(VaultManager* vault, QWidget *parent)
    : QMainWindow(parent), m_vault(vault), m_isDarkTheme(true)
{
    setWindowTitle("PassVault");
    resize(800, 500);

    setupUI();
    refreshList();
    applyTheme();
}

MainWindow::~MainWindow() {
    // Save on exit
    if (m_vault->isUnlocked()) {
        m_vault->save();
    }
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    QSplitter* splitter = new QSplitter(Qt::Horizontal);

    // Left Panel
    QWidget* leftPanel = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    
    m_listWidget = new QListWidget();
    connect(m_listWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onEntrySelected);
    
    QHBoxLayout* listBtnLayout = new QHBoxLayout();
    QPushButton* addBtn = new QPushButton("Add");
    QPushButton* themeBtn = new QPushButton("Toggle Theme");
    listBtnLayout->addWidget(addBtn);
    listBtnLayout->addWidget(themeBtn);

    leftLayout->addWidget(m_listWidget);
    leftLayout->addLayout(listBtnLayout);

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(themeBtn, &QPushButton::clicked, this, &MainWindow::onToggleThemeClicked);

    // Right Panel
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    m_titleLabel = new QLabel("Select an entry");
    m_titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    
    m_usernameLabel = new QLabel("Username: ");
    
    QHBoxLayout* passLayout = new QHBoxLayout();
    QLabel* passLbl = new QLabel("Password:");
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setReadOnly(true);
    QPushButton* copyBtn = new QPushButton("Copy");
    passLayout->addWidget(passLbl);
    passLayout->addWidget(m_passwordEdit);
    passLayout->addWidget(copyBtn);

    connect(copyBtn, &QPushButton::clicked, this, &MainWindow::onCopyPasswordClicked);

    m_urlLabel = new QLabel("URL: ");
    m_notesEdit = new QTextEdit();
    m_notesEdit->setReadOnly(true);

    QHBoxLayout* rightBtnLayout = new QHBoxLayout();
    m_editBtn = new QPushButton("Edit");
    m_deleteBtn = new QPushButton("Delete");
    rightBtnLayout->addStretch();
    rightBtnLayout->addWidget(m_editBtn);
    rightBtnLayout->addWidget(m_deleteBtn);

    connect(m_editBtn, &QPushButton::clicked, this, &MainWindow::onEditClicked);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);

    rightLayout->addWidget(m_titleLabel);
    rightLayout->addWidget(m_usernameLabel);
    rightLayout->addLayout(passLayout);
    rightLayout->addWidget(m_urlLabel);
    rightLayout->addWidget(new QLabel("Notes:"));
    rightLayout->addWidget(m_notesEdit);
    rightLayout->addLayout(rightBtnLayout);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setSizes({300, 500});

    mainLayout->addWidget(splitter);

    // Initially disable right panel buttons
    m_editBtn->setEnabled(false);
    m_deleteBtn->setEnabled(false);
}

void MainWindow::refreshList() {
    m_listWidget->clear();
    const auto& creds = m_vault->getCredentials();
    for (const auto& cred : creds) {
        m_listWidget->addItem(cred.title);
    }
}

void MainWindow::updateDetailsPanel(int index) {
    if (index >= 0 && index < m_vault->getCredentials().size()) {
        const auto& cred = m_vault->getCredentials().at(index);
        m_titleLabel->setText(cred.title);
        m_usernameLabel->setText("Username: " + cred.username);
        m_passwordEdit->setText(cred.password);
        m_urlLabel->setText("URL: " + cred.url);
        m_notesEdit->setPlainText(cred.notes);
        
        m_editBtn->setEnabled(true);
        m_deleteBtn->setEnabled(true);
    } else {
        m_titleLabel->setText("Select an entry");
        m_usernameLabel->setText("Username: ");
        m_passwordEdit->clear();
        m_urlLabel->setText("URL: ");
        m_notesEdit->clear();
        
        m_editBtn->setEnabled(false);
        m_deleteBtn->setEnabled(false);
    }
}

void MainWindow::onEntrySelected() {
    updateDetailsPanel(m_listWidget->currentRow());
}

void MainWindow::onCopyPasswordClicked() {
    if (m_listWidget->currentRow() >= 0) {
        QApplication::clipboard()->setText(m_passwordEdit->text());
    }
}

void MainWindow::onAddClicked() {
    EntryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        m_vault->addCredential(dialog.getEntry());
        m_vault->save();
        refreshList();
    }
}

void MainWindow::onEditClicked() {
    int index = m_listWidget->currentRow();
    if (index >= 0) {
        EntryDialog dialog(this);
        auto creds = m_vault->getCredentials();
        dialog.setEntry(creds.at(index));
        
        if (dialog.exec() == QDialog::Accepted) {
            creds[index] = dialog.getEntry();
            m_vault->setCredentials(creds);
            m_vault->save();
            refreshList();
            m_listWidget->setCurrentRow(index);
        }
    }
}

void MainWindow::onDeleteClicked() {
    int index = m_listWidget->currentRow();
    if (index >= 0) {
        auto reply = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this entry?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            auto creds = m_vault->getCredentials();
            creds.removeAt(index);
            m_vault->setCredentials(creds);
            m_vault->save();
            refreshList();
        }
    }
}

void MainWindow::applyTheme() {
    QString themeFile = m_isDarkTheme ? ":/themes/dark.qss" : ":/themes/light.qss";
    QFile file(themeFile);
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }
}

void MainWindow::onToggleThemeClicked() {
    m_isDarkTheme = !m_isDarkTheme;
    applyTheme();
}
