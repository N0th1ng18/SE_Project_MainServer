#include "database.h"

Database::Database()
{

}
Database::~Database()
{

}
void Database::connect()
{
    qDebug("Connect Database");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Database/db.sqlite");

    if(db.open())
    {
        qDebug("Connected to Database!");

        QSqlQuery query;
        query.prepare("SELECT * FROM Player");
        if(query.exec()){
            while(query.next()){
                qDebug() << query.value(0);
                qDebug() << query.value(1);
                qDebug() << query.value(2);
            }
        }

    }else{

        qDebug("Could not connect to database!");
        qDebug() << db.lastError();
    }
}
void Database::disconnect()
{

}
