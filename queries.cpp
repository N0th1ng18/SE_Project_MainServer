#include "queries.h"

/*
 * Description:
 *  Checks if user name is already in database using variable userName
 *
 * Contributors:
 *  John
*/
bool Queries::checkUser(QString userName)
{
    qDebug() << "Checking User Names For: " << userName;
    QSqlQuery query(db);
    query.prepare("SELECT userName FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    if(query.exec())
    {
        if (query.next())
        {
            qDebug() << "User " + userName + " Found";
            query.finish();
            return true;
        }
        else
        {
            qDebug() << "User " + userName + " Not Found";
            query.finish();
            return false;
        }
    } return false;
}

/*
 * Description:
 *  Checks for correct password on the database for user with variable(s) userName and password
 *
 * Contributors:
 *  John
*/
bool Queries::checkPassword(QString userName, QString password)
{
    qDebug() << "Checking Passwords For: " << userName;
    QSqlQuery query(db);
    QString passCheck;
    query.prepare("SELECT password FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    if(query.exec())
    {
        while(query.next())
        {
           passCheck = query.value(0).toString();
        }
    }
    query.finish();
    if (password == passCheck)
    {
        qDebug() << "Passwords For User:" << userName << "Are The Same";
        return true;
    }
    else
    {
        qDebug() << "Passwords For User: " << userName << " Are Not The Same";
        return false;
    }
}

/*
 * Description:
 *  Adds the new user to the database with variables userName and password
 *
 * Contributors:
 *  John
*/
bool Queries::addUser(QString userName, QString password)
{
    int score = 0;
    qDebug("Adding New User");
    QSqlQuery query(db);
    query.prepare("INSERT INTO Player (userName, password, highScore) "
                  "VALUES (:userName, :password, :highScore)");
    query.bindValue(":userName", userName);
    query.bindValue(":password", password);
    query.bindValue(":highScore", score);
    if (query.exec())
    {
        qDebug("Successful Add");
        query.finish();
        return true;
    }
    else
    {
        qDebug("Failed");
        qDebug() << query.lastError();
        query.finish();
        return false;
    }
}

/*
 * Description:
 *  Selects and returns the best server from the database based on lowest games
 *
 * Contributors:
 *  John
*/
QString Queries::selectBestServer()
{
    QString bestServer;
    qDebug("Getting Best Server");
    QSqlQuery query(db);
    query.prepare("SELECT serverAddress FROM Server WHERE numGames = (SELECT MIN(numGames) FROM Server WHERE numGames < maxGames)");
    if(query.exec())
    {
        while(query.next())
        {
            bestServer.append(query.value(0).toString());
            qDebug("Found Best Server");
        }
    }
    if(bestServer == "")
    {
        qDebug() << "No server avalible";
    }
    qDebug() << "Server Address: " << bestServer;
    query.finish();
    return bestServer;
}

/*
 * Description:
 *  Updates server information from database using variable(s) serverData
 *  ONLY USED IF GROUP GETS SAVES WORKING
 *
 * Contributors:
 *  John
*/
void Queries::updateServerInfo(QString serverData)
{
    qDebug("Updating Server Infomation");
    QSqlQuery query(db);
    query.prepare("INSERT INTO something VALUES (:serverData)");
    query.bindValue(":serverData", serverData);
    if(query.exec())
    {
        qDebug("Server Updated");
        query.finish();
    }
    else
    {
        qDebug("Error");
        qDebug() << query.lastError();
        query.finish();
    }
}

/*
 * Description:
 *  Gets server data from database using variable(s) gameID
 *  ONLY USED IF GROUP GETS SAVES WORKING
 *
 * Contributors:
 *  John
*/
QList<QString> Queries::getServerData(int gameID)
{
    qDebug("Getting Server Data");
    QList<QString> serverList;
    QSqlQuery query(db);
    query.prepare("SELECT gameID FROM Seat WHERE gameID = (:gameID)");
    query.bindValue(":gameID", gameID);
    if(query.exec())
    {
        while(query.next())
        {
           QString serverData = query.value(0).toString();
           serverList.append(serverData);
        }
    }
    return serverList;
}

/*
 * Description:
 *  Gets all games a user is in and appends them to a list which is returned
 *
 * Contributors:
 *  John
*/
QList<QString> Queries::getUserGameData(QString userName)
{
    qDebug("Getting User Games List Data");
    QList<QString> gameList;
    QSqlQuery query(db);
    query.prepare("SELECT gameID FROM Seat WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    if(query.exec())
    {
        while(query.next())
        {
            QString gamesIn = query.value(0).toString();
            gameList.append(gamesIn);
        }
    }
    return gameList;
}

/*
 * Description:
 *  Removes from database all expired/dormant servers
 *  ONLY USED IF GROUP GETS SAVES WORKING
 *
 * Contributors:
 *  John
*/
void Queries::expiredDormantServers()
{
    qDebug("Removing Dormant Servers");
    QSqlQuery query(db);
    int dormant = 1;
    query.prepare("DELETE FROM Server WHERE Dormant = (:dormant)");
    query.bindValue(":dormant", dormant);
    if(query.exec())
    {
        qDebug("All Dormant Servers Removed");
    }
    else {
        qDebug("Error Occured");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Constructor
 *
 * Contributors:
 *  John
*/
Queries::Queries()
{

}

/*
 * Description:
 *  Destructor
 *
 * Contributors:
 *  John
*/
Queries::~Queries()
{

}

/*
 * Description:
 *  Connects to database/recieve new incoming connections with variable hostName
 *
 * Contributors:
 *  Isaac, John
*/
void Queries::connectDB(QString hostName)
{
    qDebug("Connecting to DB");
    hostName.append("Connection");
    db = QSqlDatabase::addDatabase("QSQLITE", hostName);
    db.setDatabaseName("C:/Database/db.sqlite");
    if(db.open())
    {
        qDebug("Connected to Database");
    }
    else
    {
        qDebug("Could not connect to DB");
        qDebug() << db.lastError();
    }
}

/*
 * Description:
 *  Disconnects from connected database
 *
 * Contributors:
 *  John
*/
void Queries::disconnectDB()
{
    db.close();
    qDebug("Closed DB");
}

/*
 * Description:
 *  Sets the seat in the data base using variable(s) userName and gameID
 *
 * Contributors:
 *  John
*/
void Queries::setSeat(QString userName, int gameID)
{
    qDebug("Setting Seats");
    QSqlQuery query(db);
    query.prepare("INSERT INTO Seat (userName, gameID) VALUES (:userName, :gameID)");
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    if(query.exec())
    {
        qDebug("Seat Settings Done");
    }
    else
    {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Updates the number of players in the database using variable(s) gameID
 *
 * Contributors:
 *  John, Katie
*/
void Queries::updateNumPlayer(int gameID)
{
    qDebug ("Updating number of players in Game");
    QSqlQuery query(db);
    query.prepare("UPDATE Game SET numPlayers = numPlayers + 1 WHERE gameID = (:gameID)");
    query.bindValue(":gameID", gameID);
    if (query.exec())
    {
        qDebug("Successfully Updated Player Count");
    }
    else {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Updates the current seats in the database that the user is currently in using variable(s) userName and gameID
 *
 * Contributors:
 *  John, Katie
*/
void Queries::updateSeat(QString userName, int gameID){
    qDebug ("Updating Seats");
    QSqlQuery query(db);
    query.prepare("UPDATE Seat SET gameID = (:gameID) WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    if (query.exec())
    {
        qDebug("Successfully Updated Seats");
    }
    else
    {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Updates the created games in the database with the variable(s) gameID, serverID, roomCode, numPlayers, currentTurn
 *
 * Contributors:
 *  John
*/
void Queries::updateGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn)
{
    qDebug("Updating Game Table");
    QSqlQuery query(db);
    query.prepare("UPDATE Game SET serverID = (:serverID), roomCode = (:roomCode), "
                  "numPlayers = (:numPlayers), currentTurn = (:currentTurn) WHERE gameID = (:gameID)");
    query.bindValue(":gameID", gameID);
    query.bindValue(":serverID", serverID);
    query.bindValue(":roomCode", roomCode);
    query.bindValue(":numPlayers", numPlayers);
    query.bindValue(":currentTurn", currentTurn);
    if(query.exec())
    {
        qDebug("Successful Game Table Update");
    }
    else
    {
        qDebug("Error");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Updates the players score in the database using variable(s) userName and score
 *
 * Contributors:
 *  John
*/
void Queries::updatePlayerScore(QString userName, int score)
{
    qDebug("Updating Player Score");
    QSqlQuery query(db);
    query.prepare("UPDATE Player SET highScore = (:score) + highScore WHERE userName = (:userName)");
    query.bindValue(":score",score);
    query.bindValue(":userName", userName);
    if (query.exec())
    {
        qDebug("Successfull Score Update");
    }
    else
    {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Creates a game in the database with variable(s) gameID, serverID, roomCode, numPlayers, currentTurn
 *
 * Contributors:
 *  John
*/
void Queries::createGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn)
{
    qDebug("Creating Game Table");
    QSqlQuery query(db);
    query.prepare("INSERT INTO Game (gameID, serverID, roomCode, numPlayers, currentTurn)"
                  "VALUES (:gameID, :serverID, :roomCode, :numPlayers, :currentTurn)");
    query.bindValue(":gameID", gameID);
    query.bindValue(":serverID", serverID);
    query.bindValue(":roomCode", roomCode);
    query.bindValue(":numPlayers", numPlayers);
    query.bindValue(":currentTurn", currentTurn);
    if(query.exec())
    {
        qDebug("Successful Game Table Update");
    }
    else
    {
        qDebug("Error");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Sets servers in the database and flags them for garbage collection using variable(s) serverID
 *  ONLY USED IF GROUP GETS SAVES WORKING
 *
 * Contributors:
 *  John
*/
void Queries::setDormant(int serverID)
{
    qDebug("Setting server to dormant");
    QSqlQuery query(db);
    query.prepare("UPDATE Server SET Dormant = 1 WHERE serverID = (:serverID)");
    query.bindValue(":serverID", serverID);
    if(query.exec())
    {
        qDebug("Successfully Flagged Server For Garbage");
    }
    else
    {
        qDebug("Error");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Gets the room code from the database using variable(s) gameID
 *
 * Contributors:
 *  John
*/
QString Queries::getRoomCode(int gameID)
{
    qDebug("Getting Room Code");
    QSqlQuery query(db);
    QString roomCode;
    query.prepare("SELECT roomCode FROM Game WHERE gameID = (:gameID)");
    query.bindValue(":gameID",gameID);
    if(query.exec())
    {
        while(query.next())
        {
            roomCode = query.value(0).toString();
        }
    }
        qDebug("Suecessfully Got Room Code");
        return roomCode;
 }
