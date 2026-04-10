#include "smtpclient.h"

#include <QSslSocket>
#include <QByteArray>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

const QString SmtpClient::smtpHost     = "smtp.gmail.com";
const int     SmtpClient::smtpPort     = 587;
const QString SmtpClient::senderEmail    = "theelderdragonking@gmail.com";
const QString SmtpClient::senderPassword = "cwrn duha uccs enad";

// Helper: read one line (response) from socket, waiting up to timeoutMs.
// Returns the line without trailing CRLF.
static QString readLine(QSslSocket *socket, int timeoutMs = 5000)
{
    while (!socket->canReadLine()) {
        if (!socket->waitForReadyRead(timeoutMs)) {
            qDebug() << "[SMTP] readLine timeout";
            return QString();
        }
    }
    QByteArray line = socket->readLine();
    // Strip CRLF
    while (!line.isEmpty() && (line.endsWith('\n') || line.endsWith('\r')))
        line.chop(1);
    qDebug() << "[SMTP] <--" << line;
    return QString::fromUtf8(line);
}

// Helper: send a command and read the response.
// Returns the response line.
static QString sendCmd(QSslSocket *socket, const QByteArray &cmd)
{
    qDebug() << "[SMTP] -->" << cmd.trimmed();
    socket->write(cmd);
    socket->waitForBytesWritten(5000);
    return readLine(socket);
}

bool SmtpClient::sendVerificationCode(const QString &toEmail, const QString &code)
{
    QSslSocket socket;

    // Step 1: Connect plain TCP to port 587
    socket.connectToHost(smtpHost, smtpPort);
    if (!socket.waitForConnected(10000)) {
        qDebug() << "[SMTP] Failed to connect:" << socket.errorString();
        return false;
    }
    qDebug() << "[SMTP] Connected to" << smtpHost << ":" << smtpPort;

    // Step 2: Read greeting
    QString greeting = readLine(&socket);
    if (!greeting.startsWith("220")) {
        qDebug() << "[SMTP] Unexpected greeting:" << greeting;
        return false;
    }

    // Step 3: Send EHLO
    QString resp = sendCmd(&socket, "EHLO localhost\r\n");
    // Read multi-line EHLO response until we get a line starting with "250 "
    while (resp.startsWith("250-")) {
        resp = readLine(&socket);
    }
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] EHLO failed:" << resp;
        return false;
    }

    // Step 4: Send STARTTLS
    resp = sendCmd(&socket, "STARTTLS\r\n");
    if (!resp.startsWith("220")) {
        qDebug() << "[SMTP] STARTTLS failed:" << resp;
        return false;
    }

    // Step 5: Start TLS encryption
    socket.startClientEncryption();
    if (!socket.waitForEncrypted(10000)) {
        qDebug() << "[SMTP] TLS handshake failed:" << socket.errorString();
        return false;
    }
    qDebug() << "[SMTP] TLS established";

    // Step 6: Send EHLO again (after TLS)
    resp = sendCmd(&socket, "EHLO localhost\r\n");
    while (resp.startsWith("250-")) {
        resp = readLine(&socket);
    }
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] EHLO (after TLS) failed:" << resp;
        return false;
    }

    // Step 7: AUTH LOGIN
    resp = sendCmd(&socket, "AUTH LOGIN\r\n");
    if (!resp.startsWith("334")) {
        qDebug() << "[SMTP] AUTH LOGIN initiation failed:" << resp;
        return false;
    }

    // Send base64-encoded username
    QByteArray encodedUser = senderEmail.toUtf8().toBase64();
    resp = sendCmd(&socket, encodedUser + "\r\n");
    if (!resp.startsWith("334")) {
        qDebug() << "[SMTP] Username rejected:" << resp;
        return false;
    }

    // Send base64-encoded password
    QByteArray encodedPass = senderPassword.toUtf8().toBase64();
    resp = sendCmd(&socket, encodedPass + "\r\n");
    if (!resp.startsWith("235")) {
        qDebug() << "[SMTP] Authentication failed:" << resp;
        return false;
    }
    qDebug() << "[SMTP] Authenticated successfully";

    // Step 8: MAIL FROM
    QByteArray mailFrom = "MAIL FROM:<" + senderEmail.toUtf8() + ">\r\n";
    resp = sendCmd(&socket, mailFrom);
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] MAIL FROM failed:" << resp;
        return false;
    }

    // RCPT TO
    QByteArray rcptTo = "RCPT TO:<" + toEmail.toUtf8() + ">\r\n";
    resp = sendCmd(&socket, rcptTo);
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] RCPT TO failed:" << resp;
        return false;
    }

    // DATA
    resp = sendCmd(&socket, "DATA\r\n");
    if (!resp.startsWith("354")) {
        qDebug() << "[SMTP] DATA command failed:" << resp;
        return false;
    }

    // Step 9: Build email body
    // Subject encoded as UTF-8 Base64
    QString subjectText = QString::fromUtf8("Код подтверждения ТИМП");
    QByteArray subjectBase64 = subjectText.toUtf8().toBase64();

    QString bodyText = QString::fromUtf8("Ваш код подтверждения: ") + code;

    QByteArray emailData;
    emailData += "Subject: =?UTF-8?B?" + subjectBase64 + "?=\r\n";
    emailData += "From: " + senderEmail.toUtf8() + "\r\n";
    emailData += "To: " + toEmail.toUtf8() + "\r\n";
    emailData += "Content-Type: text/plain; charset=UTF-8\r\n";
    emailData += "\r\n";
    emailData += bodyText.toUtf8() + "\r\n";
    emailData += ".\r\n";

    qDebug() << "[SMTP] --> (email body)";
    socket.write(emailData);
    socket.waitForBytesWritten(5000);
    resp = readLine(&socket);
    if (!resp.startsWith("250")) {
        qDebug() << "[SMTP] Email body rejected:" << resp;
        return false;
    }

    // Step 10: QUIT
    sendCmd(&socket, "QUIT\r\n");
    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState)
        socket.waitForDisconnected(5000);

    qDebug() << "[SMTP] Email sent successfully to" << toEmail;
    return true;
}
