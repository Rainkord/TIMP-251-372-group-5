#include "mytcpserver.h"
#include "functionsforserver.h"

#include <QDebug>

MyTcpServer::MyTcpServer(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &MyTcpServer::onNewConnection);

    if (!m_server->listen(QHostAddress::Any, SERVER_PORT)) {
        qDebug() << "[Server] Failed to start:" << m_server->errorString();
    } else {
        qDebug() << "[Server] Listening on port" << SERVER_PORT;
    }
}

MyTcpServer::~MyTcpServer()
{
    m_server->close();
}

void MyTcpServer::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket *socket = m_server->nextPendingConnection();

        connect(socket, &QTcpSocket::readyRead,
                this, &MyTcpServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected,
                this, &MyTcpServer::onClientDisconnected);

        m_buffers[socket] = QString();

        qDebug() << "[Server] New connection from"
                 << socket->peerAddress().toString()
                 << "port" << socket->peerPort();
    }
}

void MyTcpServer::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    // Append incoming data to the buffer for this socket
    QByteArray data = socket->readAll();
    m_buffers[socket] += QString::fromUtf8(data);

    processBuffer(socket);
}

void MyTcpServer::processBuffer(QTcpSocket *socket)
{
    // Messages are delimited by '\n'. Process all complete messages.
    QString &buf = m_buffers[socket];

    while (buf.contains('\n')) {
        int idx = buf.indexOf('\n');
        QString message = buf.left(idx).trimmed();
        buf = buf.mid(idx + 1);

        if (message.isEmpty()) continue;

        qDebug() << "[Server] Received from"
                 << socket->peerAddress().toString()
                 << ":" << message;

        QString response = FunctionsForServer::processMessage(message);

        qDebug() << "[Server] Sending response:" << response;
        sendResponse(socket, response);
    }
}

void MyTcpServer::sendResponse(QTcpSocket *socket, const QString &response)
{
    if (!socket || !socket->isOpen()) return;

    // Append newline as message terminator
    QByteArray data = (response + "\n").toUtf8();
    socket->write(data);
    socket->flush();
}

void MyTcpServer::onClientDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    qDebug() << "[Server] Client disconnected:"
             << socket->peerAddress().toString();

    m_buffers.remove(socket);
    socket->deleteLater();
}
