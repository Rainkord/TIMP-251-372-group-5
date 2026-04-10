#include "mainwindow.h"
#include "taskdialog.h"
#include "schemadialog.h"

#include <QStyle>
#include <QApplication>
#include <QSpacerItem>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    connectSignals();

    // Start on auth screen
    stackedWidget->setCurrentIndex(IDX_AUTH);
}

MainWindow::~MainWindow() {}

// ─────────────────────────────────────────────
//  UI Setup
// ─────────────────────────────────────────────
void MainWindow::setupUI()
{
    setWindowTitle("ТМП — Подгруппа 5");
    setMinimumSize(1000, 700);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainVLayout = new QVBoxLayout(centralWidget);
    mainVLayout->setContentsMargins(0, 0, 0, 0);
    mainVLayout->setSpacing(0);

    // ── Top bar (persistent across all screens) ───
    QWidget *topBar = new QWidget(centralWidget);
    topBar->setFixedHeight(40);
    topBar->setStyleSheet(
        "QWidget { background-color: #2c3e50; }"
        "QPushButton { background-color: #34495e; color: white; border: none; "
        "border-radius: 4px; padding: 4px 10px; font-size: 11px; }"
        "QPushButton:hover { background-color: #4a6278; }"
        "QPushButton:pressed { background-color: #1a252f; }"
    );

    topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(8, 4, 8, 4);
    topBarLayout->setSpacing(6);

    // Task button
    taskBtn = new QPushButton(topBar);
    taskBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogInfoView));
    taskBtn->setText("  Задание");
    taskBtn->setToolTip("Показать задание");
    taskBtn->setMinimumWidth(100);

    // Schema button
    schemaBtn = new QPushButton(topBar);
    schemaBtn->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    schemaBtn->setText("  Блок-схема");
    schemaBtn->setToolTip("Показать блок-схему");
    schemaBtn->setMinimumWidth(110);

    // App title label (center)
    appTitleLabel = new QLabel("ТМП — Подгруппа 5", topBar);
    appTitleLabel->setStyleSheet(
        "QLabel { color: #ecf0f1; font-size: 12px; font-weight: bold; background: transparent; }"
    );
    appTitleLabel->setAlignment(Qt::AlignCenter);

    topBarLayout->addWidget(taskBtn);
    topBarLayout->addWidget(schemaBtn);
    topBarLayout->addStretch(1);
    topBarLayout->addWidget(appTitleLabel);
    topBarLayout->addStretch(1);

    // Thin separator line under top bar
    QFrame *separator = new QFrame(centralWidget);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Plain);
    separator->setStyleSheet("QFrame { color: #1a252f; }");
    separator->setFixedHeight(1);

    // ── Stacked widget ────────────────────────────
    stackedWidget = new QStackedWidget(centralWidget);

    // Create all screen widgets
    authWidget   = new AuthWidget(stackedWidget);
    regWidget    = new RegWidget(stackedWidget);
    verifyWidget = new VerifyWidget(stackedWidget);
    graphWidget  = new GraphWidget(stackedWidget);

    // Add in correct order (IDX_AUTH=0, IDX_REG=1, IDX_VERIFY=2, IDX_GRAPH=3)
    stackedWidget->addWidget(authWidget);    // 0
    stackedWidget->addWidget(regWidget);     // 1
    stackedWidget->addWidget(verifyWidget);  // 2
    stackedWidget->addWidget(graphWidget);   // 3

    // ── Assemble main layout ──────────────────────
    mainVLayout->addWidget(topBar);
    mainVLayout->addWidget(separator);
    mainVLayout->addWidget(stackedWidget, 1);

    centralWidget->setLayout(mainVLayout);
}

// ─────────────────────────────────────────────
//  Signal connections
// ─────────────────────────────────────────────
void MainWindow::connectSignals()
{
    // ── Top-bar buttons ───────────────────────────
    connect(taskBtn,   &QPushButton::clicked, this, &MainWindow::onTaskBtnClicked);
    connect(schemaBtn, &QPushButton::clicked, this, &MainWindow::onSchemaBtnClicked);

    // ── AuthWidget signals ────────────────────────
    // loginSuccess is not used here directly — auth sends showVerifyAuth instead
    connect(authWidget, &AuthWidget::showVerifyAuth,
            this, &MainWindow::onShowVerifyAuth);

    connect(authWidget, &AuthWidget::showRegister,
            this, &MainWindow::onShowRegister);

    // loginSuccess (direct login without verify) — switch to graph
    connect(authWidget, &AuthWidget::loginSuccess,
            this, [this](const QString &login) {
                graphWidget->setUserLogin(login);
                graphWidget->updateGraph();
                stackedWidget->setCurrentIndex(IDX_GRAPH);
            });

    // ── RegWidget signals ─────────────────────────
    connect(regWidget, &RegWidget::registrationSuccess,
            this, &MainWindow::onRegistrationSuccess);

    connect(regWidget, &RegWidget::showAuth,
            this, &MainWindow::onShowAuth);

    // ── VerifyWidget signals ──────────────────────
    connect(verifyWidget, &VerifyWidget::verificationSuccess,
            this, &MainWindow::onVerificationSuccess);

    connect(verifyWidget, &VerifyWidget::backToAuth,
            this, &MainWindow::onBackToAuth);

    // ── GraphWidget signals ───────────────────────
    connect(graphWidget, &GraphWidget::logout,
            this, &MainWindow::onLogout);
}

// ─────────────────────────────────────────────
//  Slots
// ─────────────────────────────────────────────
void MainWindow::onShowRegister()
{
    stackedWidget->setCurrentIndex(IDX_REG);
}

void MainWindow::onShowAuth()
{
    stackedWidget->setCurrentIndex(IDX_AUTH);
}

void MainWindow::onShowVerifyAuth(const QString &login)
{
    verifyWidget->setLogin(login);
    stackedWidget->setCurrentIndex(IDX_VERIFY);
}

void MainWindow::onVerificationSuccess(const QString &login)
{
    graphWidget->setUserLogin(login);
    graphWidget->updateGraph();
    stackedWidget->setCurrentIndex(IDX_GRAPH);
}

void MainWindow::onBackToAuth()
{
    stackedWidget->setCurrentIndex(IDX_AUTH);
}

void MainWindow::onRegistrationSuccess()
{
    // After registration, go back to login screen
    stackedWidget->setCurrentIndex(IDX_AUTH);
}

void MainWindow::onLogout()
{
    // Clear graph data and return to auth
    stackedWidget->setCurrentIndex(IDX_AUTH);
}

void MainWindow::onTaskBtnClicked()
{
    TaskDialog dlg(this);
    dlg.exec();
}

void MainWindow::onSchemaBtnClicked()
{
    SchemaDialog dlg(this);
    dlg.exec();
}
