#include "database.h"

Database::Database()
{
    qDebug() << QSqlDatabase::drivers();

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("mydb");
}
Database::~Database()
{

}
void Database::connect()
{


    if(db.open())
    {
        qDebug("Connected to Database!");

    }else{

        qDebug("Could not connect to database!");
        qDebug() << db.lastError();
    }
}
void Database::disconnect()
{

}
