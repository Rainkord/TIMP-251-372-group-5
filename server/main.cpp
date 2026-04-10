#include <QCoreApplication>
#include <QDebug>

#include "database.h"
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Connect to SQLite database (file persists between restarts)
    if (!Database::instance().connect("users.db")) {
        qDebug() << "[Main] Could not open database. Exiting.";
        return 1;
    }

    // Start TCP server on port 33333
    MyTcpServer server;

    qDebug() << "[Main] Server started. Waiting for connections...";

    return app.exec();
}
