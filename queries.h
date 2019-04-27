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
    bool checkUser(QString userName);
    bool checkPassword(QString userName, QString password);
    bool addUser(QString userName, QString password);
    bool createGame(int gameId, int serverId, int roomNum, int numPlayer, int turns);
    QString selectBestServer();
    void updateServerInfo(QString serverData);
    QList<QString> getServerData(int roomID);
    QList<QString> getUserGameData(QString userName);
    void expiredDormantServers();
    void isActive();
    void updateSeat(QString userName, int gameId);
    void updateNumPlayer(QString userName, int gameId, bool action);

private:
    //QSqlQuery query;
protected:


};

#endif // QUERIES_H
