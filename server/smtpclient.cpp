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
    // First wait
    if (socket->bytesAvailable() == 0) {
        socket->waitForReadyRead(timeoutMs);
    }
    result = socket->readAll();

    // Give server a moment to send all lines (multi-line responses)
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
        // Return last line (the final status line)
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

bool SmtpClient::sendVerificationCode(const QString &toEmail, const QString &code)
{
    QSslSocket socket;

    // Port 465: connect with SSL directly (no STARTTLS needed)
    socket.connectToHostEncrypted(smtpHost, smtpPort);
    // Ignore any SSL certificate errors
    socket.ignoreSslErrors();
    if (!socket.waitForEncrypted(20000)) {
        qDebug() << "[SMTP] SSL connection failed:" << socket.errorString();
        return false;
    }
    qDebug() << "[SMTP] SSL connected to" << smtpHost << ":" << smtpPort;

    // Read greeting
    QString greeting = readResponse(&socket, 15000);
    if (greeting.isEmpty() || !greeting.startsWith("220")) {
        qDebug() << "[SMTP] Bad greeting:" << greeting;
        return false;
    }

    // EHLO
    QString resp = sendCmd(&socket, "EHLO localhost\r\n");
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] EHLO failed:" << resp;
        return false;
    }

    // AUTH LOGIN
    resp = sendCmd(&socket, "AUTH LOGIN\r\n");
    if (!resp.startsWith("334")) {
        qDebug() << "[SMTP] AUTH LOGIN failed:" << resp;
        return false;
    }

    // Username
    resp = sendCmd(&socket, senderEmail.toUtf8().toBase64() + "\r\n");
    if (!resp.startsWith("334")) {
        qDebug() << "[SMTP] Username rejected:" << resp;
        return false;
    }

    // Password
    resp = sendCmd(&socket, senderPassword.toUtf8().toBase64() + "\r\n");
    if (!resp.startsWith("235")) {
        qDebug() << "[SMTP] Auth failed:" << resp;
        return false;
    }
    qDebug() << "[SMTP] Authenticated";

    // MAIL FROM
    resp = sendCmd(&socket, "MAIL FROM:<" + senderEmail.toUtf8() + ">\r\n");
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] MAIL FROM failed:" << resp;
        return false;
    }

    // RCPT TO
    resp = sendCmd(&socket, "RCPT TO:<" + toEmail.toUtf8() + ">\r\n");
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] RCPT TO failed:" << resp;
        return false;
    }

    // DATA
    resp = sendCmd(&socket, "DATA\r\n");
    if (!resp.startsWith("354")) {
        qDebug() << "[SMTP] DATA failed:" << resp;
        return false;
    }

    // Email body
    QString subjectText = QString::fromUtf8("Код подтверждения ТИМП");
    QByteArray subjectB64 = subjectText.toUtf8().toBase64();
    QString bodyText = QString::fromUtf8("Ваш код подтверждения: ") + code;

    QByteArray email;
    email += "Subject: =?UTF-8?B?" + subjectB64 + "?=\r\n";
    email += "From: " + senderEmail.toUtf8() + "\r\n";
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
