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
    void connectDB(QString hostName);
    void disconnectDB();
    void setSeat(QString userName, int gameID);
    void updateServerInfo(QString serverData);
    void expiredDormantServers();
    void updateSeat(QString userName, int gameId);
    void updateNumPlayer(int gameID);
    void updateGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn);
    void updatePlayerScore(QString userName, int score);
    void createGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn);
    void setDormant(int serverID);
    bool checkUser(QString userName);
    bool checkPassword(QString userName, QString password);
    bool addUser(QString userName, QString password);
    QString getRoomCode(int gameID);
    QString selectBestServer();
    QList<QString> getServerData(int roomID);
    QList<QString> getUserGameData(QString userName);
    QSqlDatabase db;

private:

protected:
};

#endif // QUERIES_H
