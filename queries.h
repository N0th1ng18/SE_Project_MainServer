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

    bool checkUser(QString userName);
    bool checkPassword(QString userName, QString password);
    bool addUser(QString userName, QString password);
    QString selectBestServer();
    void updateServerInfo(QString serverData);
    QList<QString> getServerData(int roomID);
    QList<QString> getUserGameData(QString userName);
    void expiredDormantServers();
    void updateSeat(QString userName, int gameId);
    void updateNumPlayer(int gameID, int playerCount);
    void updateGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn);
    void updatePlayerScore(QString userName, int score);
    void createGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn);
    void setDormant(int serverID);
    QString getRoomCode(int gameID);
    void setSeat(QString userName, int gameID);

private:
    QSqlDatabase db;
protected:


};

#endif // QUERIES_H
