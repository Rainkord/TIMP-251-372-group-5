#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QString>

class SmtpClient
{
public:
    static bool sendVerificationCode(const QString &toEmail, const QString &code);
    static bool sendPasswordResetCode(const QString &toEmail, const QString &login, const QString &code);

    static const QString smtpHost;
    static const int smtpPort;
    static const QString senderEmail;
    static const QString senderPassword;
};

#endif // SMTPCLIENT_H
