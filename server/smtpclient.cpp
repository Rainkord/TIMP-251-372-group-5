#include "smtpclient.h"

#include <QSslSocket>
#include <QByteArray>
#include <QDebug>

const QString SmtpClient::smtpHost       = "smtp.gmail.com";
const int     SmtpClient::smtpPort       = 465;
const QString SmtpClient::senderEmail    = "theelderdragonking@gmail.com";
const QString SmtpClient::senderPassword = "cwrnduhauccsenad";

static QByteArray waitForData(QSslSocket *socket, int timeoutMs = 15000)
{
    QByteArray result;
    if (socket->bytesAvailable() == 0) {
        socket->waitForReadyRead(timeoutMs);
    }
    result = socket->readAll();
    while (socket->waitForReadyRead(500)) {
        result.append(socket->readAll());
    }
    return result;
}

static QString readResponse(QSslSocket *socket, int timeoutMs = 15000)
{
    QByteArray data = waitForData(socket, timeoutMs);
    QString all = QString::fromUtf8(data).trimmed();
    if (!all.isEmpty()) {
        QStringList lines = all.split('\n');
        QString last = lines.last().trimmed();
        qDebug() << "[SMTP] <--" << last;
        return last;
    }
    qDebug() << "[SMTP] readResponse: no data received";
    return QString();
}

static QString sendCmd(QSslSocket *socket, const QByteArray &cmd)
{
    qDebug() << "[SMTP] -->" << cmd.trimmed();
    socket->write(cmd);
    socket->flush();
    socket->waitForBytesWritten(5000);
    return readResponse(socket);
}

static bool doSend(const QString &toEmail,
                   const QString &subjectText,
                   const QString &bodyText)
{
    QSslSocket socket;
    socket.connectToHostEncrypted(SmtpClient::smtpHost, SmtpClient::smtpPort);
    socket.ignoreSslErrors();
    if (!socket.waitForEncrypted(20000)) {
        qDebug() << "[SMTP] SSL connection failed:" << socket.errorString();
        return false;
    }
    qDebug() << "[SMTP] SSL connected";

    QString resp = readResponse(&socket, 15000);
    if (resp.isEmpty() || !resp.startsWith("220")) return false;

    resp = sendCmd(&socket, "EHLO localhost\r\n");
    if (!resp.startsWith("250")) return false;

    resp = sendCmd(&socket, "AUTH LOGIN\r\n");
    if (!resp.startsWith("334")) return false;

    resp = sendCmd(&socket, SmtpClient::senderEmail.toUtf8().toBase64() + "\r\n");
    if (!resp.startsWith("334")) return false;

    resp = sendCmd(&socket, SmtpClient::senderPassword.toUtf8().toBase64() + "\r\n");
    if (!resp.startsWith("235")) return false;
    qDebug() << "[SMTP] Authenticated";

    resp = sendCmd(&socket, "MAIL FROM:<" + SmtpClient::senderEmail.toUtf8() + ">\r\n");
    if (!resp.startsWith("250")) return false;

    resp = sendCmd(&socket, "RCPT TO:<" + toEmail.toUtf8() + ">\r\n");
    if (!resp.startsWith("250")) return false;

    resp = sendCmd(&socket, "DATA\r\n");
    if (!resp.startsWith("354")) return false;

    QByteArray subjectB64 = subjectText.toUtf8().toBase64();

    QByteArray email;
    email += "Subject: =?UTF-8?B?" + subjectB64 + "?=\r\n";
    email += "From: " + SmtpClient::senderEmail.toUtf8() + "\r\n";
    email += "To: " + toEmail.toUtf8() + "\r\n";
    email += "Content-Type: text/plain; charset=UTF-8\r\n";
    email += "\r\n";
    email += bodyText.toUtf8() + "\r\n";
    email += ".\r\n";

    socket.write(email);
    socket.flush();
    socket.waitForBytesWritten(5000);
    resp = readResponse(&socket);
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] Email rejected:" << resp;
        return false;
    }

    sendCmd(&socket, "QUIT\r\n");
    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState)
        socket.waitForDisconnected(5000);

    qDebug() << "[SMTP] Email sent to" << toEmail;
    return true;
}

bool SmtpClient::sendVerificationCode(const QString &toEmail, const QString &code)
{
    QString subject = QString::fromUtf8("Код подтверждения ТИМП");
    QString body    = QString::fromUtf8("Ваш код подтверждения: ") + code;
    return doSend(toEmail, subject, body);
}

bool SmtpClient::sendPasswordResetCode(const QString &toEmail,
                                        const QString &login,
                                        const QString &code)
{
    QString subject = QString::fromUtf8("Восстановление пароля ТИМП");
    QString body    = QString::fromUtf8("Здравствуйте, ") + login +
                      QString::fromUtf8(".\n\nВаш код для сброса пароля: ") + code +
                      QString::fromUtf8("\n\nЕсли вы не запрашивали сброс пароля — игнорируйте это письмо.");
    return doSend(toEmail, subject, body);
}
