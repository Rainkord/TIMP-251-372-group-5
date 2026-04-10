#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>

class Database
{
public:
    static Database &instance();

    bool connect(const QString &dbName);
    bool addUser(const QString &name, const QString &passwordHash, const QString &email);
    bool checkUser(const QString &name, const QString &passwordHash);
    bool userExists(const QString &name);
    QString getUserEmail(const QString &name);

private:
    Database() = default;
    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    QSqlDatabase m_db;
    bool createTable();
};

#endif // DATABASE_H
