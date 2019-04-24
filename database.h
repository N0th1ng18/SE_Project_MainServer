#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>


class Database
{
public:
    Database();
    ~Database();
    void connect();
    void disconnect();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
