#ifndef QUERIES_H
#define QUERIES_H
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
using namespace std;

class Queries
{
public:
    Queries();
    ~Queries();
    void connect(int threadID);
    void disconnect();
    bool checkUser(QString userName);
    bool checkPassword(QString userName, QString password);
    bool addUser(QString userName, QString password);
    QString selectBestServer();
    void updateServerInfo(QString serverData);
    QList<QString> getServerData(int roomID);
    QList<QString> getUserGameData(QString userName);
    void expiredDormantServers();
    void updateSeat(QString userName, int gameId);
    void updateNumPlayer(QString userName, int gameId, bool action);

private:
    QSqlDatabase db;


};

#endif // QUERIES_H
