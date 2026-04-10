#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStackedWidget>

class RegWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegWidget(QWidget *parent = nullptr);
    ~RegWidget();

signals:
    void registrationSuccess();
    void showAuth();

private slots:
    // Step 1 slots
    void onLoginTextChanged(const QString &text);
    void onPasswordTextChanged(const QString &text);
    void onConfirmPasswordTextChanged(const QString &text);
    void onTogglePassword1();
    void onTogglePassword2();
    void onContinueClicked();

    // Step 2 slots
    void onEmailTextChanged(const QString &text);
    void onBackClicked();
    void onConfirmEmailClicked();

    // Step 3 slots
    void onCodeTextChanged(const QString &text);
    void onVerifyCodeClicked();
    void onCodeLockTimerFired();

    // Back to auth
    void onShowAuthClicked();

private:
    // ---- Step 1 widgets ----
    QWidget     *step1Widget;
    QLineEdit   *loginEdit;
    QLabel      *loginErrorLabel;
    QLineEdit   *passwordEdit;
    QLabel      *passwordErrorLabel;
    QPushButton *togglePassBtn1;
    QLineEdit   *confirmPasswordEdit;
    QLabel      *confirmErrorLabel;
    QPushButton *togglePassBtn2;
    QPushButton *continueBtn;

    // ---- Step 2 widgets ----
    QWidget     *step2Widget;
    QLineEdit   *emailEdit;
    QLabel      *emailErrorLabel;
    QPushButton *backBtn;

    // ---- Step 3 widgets ----
    QWidget     *step3Widget;
    QPushButton *confirmEmailBtn;
    QLabel      *codeStatusLabel;
    QLineEdit   *codeEdit;
    QLabel      *codeErrorLabel;
    QPushButton *verifyCodeBtn;

    // ---- Navigation ----
    QPushButton *showAuthBtn;

    // ---- State ----
    int         codeFailedAttempts;
    int         codeLockLevel;
    QTimer      *codeLockTimer;
    bool        codeIsLocked;
    QString     currentLogin;

    // ---- Helpers ----
    void setupUI();
    void validateStep1();
    bool isEmailValid(const QString &email) const;
    void applyCodeLock(int minutes, const QString &message);
    void showStep(int step);
};

#endif // REGWIDGET_H
