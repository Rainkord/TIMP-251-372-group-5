#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class MyTcpServer : public QObject
{
    Q_OBJECT

public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *m_server;

    // Map from socket pointer to its accumulated buffer (for partial reads)
    QMap<QTcpSocket *, QString> m_buffers;

    static const quint16 SERVER_PORT = 33333;

    void processBuffer(QTcpSocket *socket);
    void sendResponse(QTcpSocket *socket, const QString &response);
};

#endif // MYTCPSERVER_H
