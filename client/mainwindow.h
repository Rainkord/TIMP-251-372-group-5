#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "authwidget.h"
#include "regwidget.h"
#include "verifywidget.h"
#include "graphwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Routing slots
    void onShowRegister();
    void onShowAuth();
    void onShowVerifyAuth(const QString &login);
    void onVerificationSuccess(const QString &login);
    void onBackToAuth();
    void onRegistrationSuccess();
    void onLogout();

    // Toolbar button slots
    void onTaskBtnClicked();
    void onSchemaBtnClicked();

private:
    // ── Central widget & layouts ─────────────────
    QWidget         *centralWidget;
    QVBoxLayout     *mainVLayout;
    QHBoxLayout     *topBarLayout;

    // ── Persistent top-bar buttons ────────────────
    QPushButton     *taskBtn;
    QPushButton     *schemaBtn;
    QLabel          *appTitleLabel;

    // ── Stacked widget holding all screens ────────
    QStackedWidget  *stackedWidget;

    // ── Screen widgets ────────────────────────────
    AuthWidget      *authWidget;
    RegWidget       *regWidget;
    VerifyWidget    *verifyWidget;
    GraphWidget     *graphWidget;

    // ── Setup ─────────────────────────────────────
    void setupUI();
    void connectSignals();

    // Index constants (order of addWidget)
    static const int IDX_AUTH   = 0;
    static const int IDX_REG    = 1;
    static const int IDX_VERIFY = 2;
    static const int IDX_GRAPH  = 3;
};

#endif // MAINWINDOW_H
