#include "authwidget.h"
#include "clientsingleton.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QCryptographicHash>
#include <QFont>
#include <QString>

AuthWidget::AuthWidget(QWidget *parent)
    : QWidget(parent),
      failedAttempts(0),
      lockLevel(0),
      isLocked(false)
{
    lockTimer = new QTimer(this);
    lockTimer->setSingleShot(true);
    connect(lockTimer, &QTimer::timeout, this, &AuthWidget::onLockTimerFired);

    setupUI();
}

AuthWidget::~AuthWidget()
{
}

void AuthWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(12);

    // Title
    QLabel *titleLabel = new QLabel("Авторизация", this);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(10);

    // Login field
    loginEdit = new QLineEdit(this);
    loginEdit->setPlaceholderText("Логин");
    loginEdit->setMinimumHeight(36);
    loginEdit->setStyleSheet("QLineEdit { padding: 4px 8px; border: 1px solid #cccccc; border-radius: 4px; }");
    mainLayout->addWidget(loginEdit);

    // Password field row (password + eye button)
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->setSpacing(6);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Пароль");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(36);
    passwordEdit->setStyleSheet("QLineEdit { padding: 4px 8px; border: 1px solid #cccccc; border-radius: 4px; }");
    passwordLayout->addWidget(passwordEdit);

    togglePasswordBtn = new QPushButton("👁", this);
    togglePasswordBtn->setFixedWidth(35);
    togglePasswordBtn->setFixedHeight(36);
    togglePasswordBtn->setToolTip("Показать/скрыть пароль");
    togglePasswordBtn->setStyleSheet(
        "QPushButton { border: 1px solid #cccccc; border-radius: 4px; background: #f5f5f5; }"
        "QPushButton:hover { background: #e0e0e0; }"
    );
    connect(togglePasswordBtn, &QPushButton::clicked, this, &AuthWidget::onTogglePassword);
    passwordLayout->addWidget(togglePasswordBtn);

    mainLayout->addLayout(passwordLayout);

    // Status label (errors)
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("QLabel { color: red; }");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->hide();
    mainLayout->addWidget(statusLabel);

    // Attempts label
    attemptsLabel = new QLabel(this);
    attemptsLabel->setStyleSheet("QLabel { color: #888888; font-size: 10pt; }");
    attemptsLabel->setAlignment(Qt::AlignCenter);
    attemptsLabel->hide();
    mainLayout->addWidget(attemptsLabel);

    mainLayout->addSpacing(8);

    // Login button
    loginBtn = new QPushButton("Войти", this);
    loginBtn->setMinimumHeight(38);
    loginBtn->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; border: none; border-radius: 4px; font-size: 13pt; }"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:disabled { background-color: #cccccc; color: #666666; }"
    );
    connect(loginBtn, &QPushButton::clicked, this, &AuthWidget::onLoginClicked);
    mainLayout->addWidget(loginBtn);

    mainLayout->addSpacing(8);

    // Register link button
    registerBtn = new QPushButton("Регистрация", this);
    registerBtn->setFlat(true);
    registerBtn->setStyleSheet(
        "QPushButton { color: #2196F3; border: none; text-decoration: underline; font-size: 11pt; }"
        "QPushButton:hover { color: #1565C0; }"
    );
    connect(registerBtn, &QPushButton::clicked, this, &AuthWidget::onRegisterClicked);
    mainLayout->addWidget(registerBtn, 0, Qt::AlignCenter);

    mainLayout->addStretch();
}

void AuthWidget::onTogglePassword()
{
    if (passwordEdit->echoMode() == QLineEdit::Password) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
    }
}

void AuthWidget::applyLock(int minutes, const QString &message)
{
    isLocked = true;
    loginBtn->setEnabled(false);
    statusLabel->setText(message);
    statusLabel->show();
    attemptsLabel->hide();
    if (minutes == 0) {
        // Special case: 30 seconds
        lockTimer->start(30 * 1000);
    } else {
        lockTimer->start(minutes * 60 * 1000);
    }
}

void AuthWidget::onLockTimerFired()
{
    isLocked = false;
    loginBtn->setEnabled(true);
    statusLabel->hide();
    attemptsLabel->hide();
}

void AuthWidget::onLoginClicked()
{
    if (isLocked) {
        int remainingMs = lockTimer->remainingTime();
        int remainingSec = remainingMs / 1000;
        int remainingMin = remainingSec / 60;
        int remainingSecMod = remainingSec % 60;

        QString timeStr;
        if (remainingMin > 0) {
            timeStr = QString("Осталось %1 мин %2 сек").arg(remainingMin).arg(remainingSecMod);
        } else {
            timeStr = QString("Осталось %1 сек").arg(remainingSec);
        }

        statusLabel->setText("Аккаунт заблокирован. " + timeStr);
        statusLabel->show();
        return;
    }

    QString login = loginEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Введите логин и пароль.");
        statusLabel->show();
        return;
    }

    // Hash password with SHA-256
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    QString passwordHash = QString::fromLatin1(hashBytes.toHex());

    // Send auth request
    QString request = QString("auth||%1||%2").arg(login, passwordHash);
    QString response = ClientSingleton::instance().sendRequest(request);

    if (response == "auth_code_sent") {
        emit showVerifyAuth(login);
        return;
    }

    if (response == "auth-") {
        failedAttempts++;

        if (failedAttempts < 4) {
            int remaining = 4 - failedAttempts;
            statusLabel->setText(QString("Неверный логин или пароль. Осталось попыток: %1").arg(remaining));
            statusLabel->show();
            attemptsLabel->hide();
            return;
        }

        if (failedAttempts == 4) {
            lockLevel = 1;
            applyLock(0, "Аккаунт заблокирован на 30 секунд");
            return;
        }

        if (failedAttempts == 5) {
            lockLevel = 2;
            applyLock(5, "Аккаунт заблокирован на 5 минут");
            return;
        }

        if (failedAttempts == 6) {
            lockLevel = 3;
            applyLock(10, "Аккаунт заблокирован на 10 минут");
            return;
        }

        // failedAttempts >= 7
        lockLevel = 4;
        applyLock(9999, "Аккаунт заблокирован на длительное время");
        return;
    }

    // Unexpected response
    statusLabel->setText("Ошибка соединения с сервером.");
    statusLabel->show();
}

void AuthWidget::onRegisterClicked()
{
    emit showRegister();
}
