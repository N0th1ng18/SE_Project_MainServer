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

    QSqlDatabase db;
    bool userLogin(QString userName, QString password);
    QString getUser(QString userName);
    bool checkUser(QString userName);
    bool checkPassword(QString userName, QString password);
    bool addUser(QString userName, QString password);
    QList<QString> selectBestServer();
    void updateServerInfo(QString serverData);
    QList<QString> getServerData(int roomID);
    QList<QString> getUserGameData(QString userName);
    void expiredDormantServers();
    Queries();
    ~Queries();
    void connectDB(QString hostName);
    void disconnectDB();
    void setSeat(QString userName, int gameID);
    void updateNumPlayer(int gameID);
    void updateSeat(QString userName, int gameId);
    void updateGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn);
    void updatePlayerScore(QString userName, int score);
    void createGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn);
    void setDormant(int serverID);
    QString getRoomCode(int gameID);
    void createServer(int serverID, QString serverAddress, int serverPort, int numGames, int maxGames);
    void createGameID();
    QList<QString> getAddressPort(int gameID);
    void updateGamePort(int gameID, QString gamePort);
    void clearSeats(int gameID);

private:

protected:
};

#endif // QUERIES_H
