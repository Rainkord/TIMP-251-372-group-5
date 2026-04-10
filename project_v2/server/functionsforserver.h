#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QString>
#include <QMap>

// Holds pending registration data until email code is verified
struct TempRegData {
    QString name;
    QString passwordHash;
    QString email;
    QString code;
};

class FunctionsForServer
{
public:
    // Main entry point: parse incoming message and return response string
    static QString processMessage(const QString &message);

private:
    // Pending auth codes: login -> code
    static QMap<QString, QString> pendingCodes;

    // Pending registrations: login -> TempRegData
    static QMap<QString, TempRegData> pendingRegistrations;

    // Generates a 6-digit random code as a zero-padded string
    static QString generateCode();

    // Individual command handlers
    static QString handleRegistration(const QStringList &parts);
    static QString handleVerifyReg(const QStringList &parts);
    static QString handleAuth(const QStringList &parts);
    static QString handleVerifyAuth(const QStringList &parts);
    static QString handleGetGraph(const QStringList &parts);
    static QString handleGetTask();
};

#endif // FUNCTIONSFORSERVER_H
