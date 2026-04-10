#include "clientsingleton.h"
#include <QTcpSocket>
#include <QString>

ClientSingleton::ClientSingleton()
{
    socket = new QTcpSocket();
}

ClientSingleton::~ClientSingleton()
{
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }
}

ClientSingleton& ClientSingleton::instance()
{
    static ClientSingleton inst;
    return inst;
}

bool ClientSingleton::connectToServer(const QString &host, int port)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        return true;
    }
    socket->connectToHost(host, static_cast<quint16>(port));
    return socket->waitForConnected(3000);
}

QString ClientSingleton::sendRequest(const QString &request)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        // Try to reconnect to default server
        socket->connectToHost("127.0.0.1", 33333);
        if (!socket->waitForConnected(3000)) {
            return QString();
        }
    }

    QByteArray data = (request + "\n").toUtf8();
    socket->write(data);
    socket->flush();

    if (!socket->waitForBytesWritten(3000)) {
        return QString();
    }

    if (!socket->waitForReadyRead(5000)) {
        return QString();
    }

    QByteArray response = socket->readAll();
    return QString::fromUtf8(response).trimmed();
}

void ClientSingleton::disconnectFromServer()
{
    socket->disconnectFromHost();
}

bool ClientSingleton::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}
