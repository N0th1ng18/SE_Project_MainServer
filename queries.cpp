#include "queries.h"

/*
 * Description:
 *  Checks if user name and password are already in database and login was successfull using variable userName
 *
 * Contributors:
 *  John
 *
 * Notes:
 *  Needed?
*/
bool Queries::userLogin(QString userName, QString password)
{
    QSqlQuery query(db);
    query.prepare("SELECT userName, password FROM Player WHERE "
                  "userName = (:userName) AND password = (:password)");
    query.bindValue(":userName", userName);
    query.bindValue(":password", password);
    if(query.exec())
    {
        query.finish();
        return true;
    }
    else
    {
        qDebug("User and Password dont match");
        query.finish();
        return false;
    }
}

/*
 * Description:
 *  Gets user name from database using variable userName
 *
 * Contributors:
 *  John
 *
 * Notes:
 *  Needed?
*/
QString Queries::getUser(QString userName)
{
    QSqlQuery query(db);
    QString user;
    query.prepare("SELECT userName FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    if(query.exec())
    {
        while(query.next())
        {
            user.append(query.value(0).toString());
        }
    }
    else
    {
        qDebug("User Not found");
        qDebug() << query.lastError();
    }
    query.finish();
    return user;
}

/*
 * Description:
 *  Checks if user name is already in database using variable userName
 *
 * Contributors:
 *  John
*/
bool Queries::checkUser(QString userName)
{
    QSqlQuery query(db);
    query.prepare("SELECT userName FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    if(query.exec())
    {
        if (query.next())
        {
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
    if(checkUser(userName) == false)
    {
        int score = 0;
        QSqlQuery query(db);
        query.prepare("INSERT INTO Player (userName, password, highScore) "
                      "VALUES (:userName, :password, :highScore)");
        query.bindValue(":userName", userName);
        query.bindValue(":password", password);
        query.bindValue(":highScore", score);
        if (query.exec())
        {
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
    else
    {
        qDebug("Failed adding user");
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
QList<QString> Queries::selectBestServer()
{
    QString serverAddress;
    int serverID = 0;
    qDebug("Getting Best Server");
    QSqlQuery query(db);
    query.prepare("SELECT serverID, serverAddress FROM Server WHERE numGames = (SELECT MIN(numGames) FROM Server WHERE numGames < maxGames)");
    if(query.exec())
    {
        while(query.next())
        {
            serverID = query.value(0).toInt();
            serverAddress = query.value(1).toString();
            qDebug("Found Best Server");
        }
    }
    if(serverAddress == "")
    {
        qDebug() << "No server avalible";
    }
    query.prepare("SELECT MAX(gameID) FROM Game");
    int gameIdCheck = 0;
    if(query.exec())
    {
        while(query.next())
        {
            gameIdCheck = query.value(0).toInt();
            qDebug("Successfully found Game ID");
        }
    }
    else
    {
        qDebug("Error setting game ID");
        qDebug() << query.lastError();
    }
    query.finish();
    gameIdCheck += 1;
    qDebug() << "Server Address: " << serverAddress;
    query.prepare("INSERT INTO Game VALUES (:gameID, :serverID, :roomCode, NULL, NULL)");
    query.bindValue(":gameID", gameIdCheck);
    query.bindValue(":serverID", serverID);
    query.bindValue(":roomCode", gameIdCheck);
    if(query.exec())
    {
        qDebug("got um coach");
    }
    else
    {
     qDebug("aaron");
    }
    query.finish();
    QList<QString> tempList;
    tempList.append(serverAddress);
    tempList.append(QString::number(gameIdCheck));
    tempList.append(QString::number(gameIdCheck));
    return tempList;
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
    QSqlQuery query(db);
    query.prepare("INSERT INTO something VALUES (:serverData)");
    query.bindValue(":serverData", serverData);
    if(query.exec())
    {
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
    query.finish();
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
    QSqlQuery query(db);
    int dormant = 1;
    query.prepare("DELETE FROM Server WHERE Dormant = (:dormant)");
    query.bindValue(":dormant", dormant);
    if(query.exec())
    {
        query.finish();
    }
    else {
        qDebug("Failed Deleting Servers");
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
 *  ONLY USED IF GROUP GETS SAVES WORKING
 *
 * Contributors:
 *  John
*/
void Queries::setSeat(QString userName, int gameID)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO Seat (userName, gameID) VALUES (:userName, :gameID)");
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    if(query.exec())
    {
        query.finish();
    }
    else
    {
        qDebug("Failed Setting Seats");
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
    QSqlQuery query(db);
    query.prepare("UPDATE Game SET numPlayers = numPlayers + 1 WHERE gameID = (:gameID)");
    query.bindValue(":gameID", gameID);
    if (query.exec())
    {
        query.finish();
    }
    else {
        query.finish();
        qDebug("Failed Updating Seats");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Updates the current seats in the database that the user is currently in using variable(s) userName and gameID
 *  ONLY USED IF GROUP GETS SAVES WORKING
 *
 * Contributors:
 *  John, Katie
*/
void Queries::updateSeat(QString userName, int gameID){
    QSqlQuery query(db);
    query.prepare("UPDATE Seat SET gameID = (:gameID) WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    if (query.exec())
    {
        query.finish();
    }
    else
    {
        query.finish();
        qDebug("Failed Updating Seats");
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
        query.finish();
    }
    else
    {
        query.finish();
        qDebug("Failed Updating Game");
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
    QSqlQuery query(db);
    query.prepare("UPDATE Player SET highScore = (:score) + highScore WHERE userName = (:userName)");
    query.bindValue(":score",score);
    query.bindValue(":userName", userName);
    if (query.exec())
    {
        query.finish();
    }
    else
    {
        query.finish();
        qDebug("Failed Updating Player Score");
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
        query.finish();
    }
    else
    {
        query.finish();
        qDebug("Failed Creating Game");
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
    QSqlQuery query(db);
    query.prepare("UPDATE Server SET Dormant = 1 WHERE serverID = (:serverID)");
    query.bindValue(":serverID", serverID);
    if(query.exec())
    {
        query.finish();
    }
    else
    {
        query.finish();
        qDebug("Failed Setting Dormants");
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
        query.finish();
        return roomCode;
 }

/*
 * Description:
 *  Creates a server in the database with variable(s) serverID, serverAddress, serverPort, numGames, maxGames
 *
 * Contributors:
 *  John
*/
void Queries::createServer(int serverID, QString serverAddress, int serverPort, int numGames, int maxGames)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO Server (serverID, serverAddress, serverPort, numGames, maxGames)"
                  "VALUES (:serverID, :serverAddress, :serverPort, :numGames, :maxGames)");
    query.bindValue(":serverID", serverID);
    query.bindValue(":serverAddress", serverAddress);
    query.bindValue(":serverPort", serverPort);
    query.bindValue(":numGames", numGames);
    query.bindValue(":maxGames", maxGames);
    if(query.exec())
    {
        query.finish();
    }
    else
    {
        query.finish();
        qDebug("Failed Creating Server");
        qDebug() << query.lastError();
    }
}

/*
 * Description:
 *  Creates and sets a new gameID to the database with all other columns as null
 *
 * Contributors:
 *  John
*/
void Queries::createGameID()
{;
    QSqlQuery query(db);
    int value = 0;
    query.prepare("SELECT MAX(gameID) FROM Game");
    if(query.exec())
    {
        while(query.next())
        {
            value = query.value(0).toInt();
        }
    }
    else
    {
        qDebug("Error setting game ID");
        qDebug() << query.lastError();
    }
    query.finish();
    value += 1;
    query.prepare("INSERT INTO Game VALUES (:value, NULL, NULL, NULL, NULL)");
    query.bindValue(":value", value);
    if(query.exec())
    {
       query.finish();

    }
    else
    {
        query.finish();
        qDebug("Error adding game ID");
        qDebug() << query.lastError();
    }
    query.finish();
}

/*
 * Description:
 *  Returns the server address and port from the database using variable(s) gameID
 *
 * Contributors:
 *  John
*/
QString Queries::getAddressPort(int gameID)
{
    QSqlQuery query(db);
    int check = 0;
    QString address;
    query.prepare("SELECT serverID FROM Game WHERE gameID = (:gameID)");
    query.bindValue(":gameID", gameID);
    if(query.exec())
    {
        while(query.next())
        {
            check = query.value(0).toInt();
        }
    }
    else
    {
            qDebug("Error getting serverID");
            qDebug() << query.lastError();
    }
    query.finish();
    query.prepare("SELECT serverAddress, serverPort FROM Server WHERE "
                  "serverID = (:check)");
    query.bindValue(":check", check);
    if(query.exec())
    {
        while(query.next())
        {
            address = query.value(0).toString();
            address.append(":");
            address.append(query.value(1).toString());
        }
    }
    else
    {
        qDebug("Error getting server and port");
        qDebug() << query.lastError();
    }
    query.finish();
    return address;
}

/*
 * Description:
 *  Updates the game port using variable(s) gameID and gamePort
 *
 * Contributors:
 *  John
*/
void Queries::updateGamePort(int gameID, QString gamePort)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO GAME gamePort VALUES (:gamePort) WHERE gameID = :gameID");
    query.bindValue(":gamePort", gamePort);
    query.bindValue(":gameID", gameID);
    if(query.exec())
    {
        query.finish();
    }
    else
    {
        query.finish();
        qDebug("Failed");
    }
}
