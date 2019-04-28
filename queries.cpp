#include "queries.h"

Queries::Queries()
{

}

Queries::~Queries()
{

}

void Queries::connectDB(QString hostName) {
    qDebug("Connecting to DB");
    hostName.append("Connection");
    db = QSqlDatabase::addDatabase("QSQLITE", hostName);
    db.setDatabaseName("C:/Database/db.sqlite");

    if(db.open())
    {
        qDebug("Connected to Database");

    }
    else{
        qDebug("Could not connect to DB");
        qDebug() << db.lastError();
    }
}

void Queries::disconnectDB() {

    db.close();
    qDebug("Closed DB");
}

bool Queries::checkUser(QString userName)
{
    qDebug() << "Checking User Names For: " << userName;
    //querey database with username
    QSqlQuery query(db);
    query.prepare("SELECT userName FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    //checks query to see if userName is found
    if(query.exec()) {
        if (query.next()) {
            //if userName is found
            qDebug() << "User " + userName + " Found";
            query.finish();
            return true;
        }
        else {
            //if userName is not found
            qDebug() << "User " + userName + " Not Found";
            query.finish();
            return false;
        }

    } return false; // will never reach here if command is valid
}


bool Queries::checkPassword(QString userName, QString password)
{
    qDebug() << "Checking Passwords For: " << userName;
    //querey data with username to get password
    QSqlQuery query(db);
    QString passCheck;
    query.prepare("SELECT password FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);


    qDebug() << query.lastError();


    //checks query to get password from userName
    if(query.exec()){
        while(query.next()){
           //sets the password related to userName to variable pass
           passCheck = query.value(0).toString();
        }
    }
    query.finish();
    //if the password entered and the password found within query are the same
    if (password == passCheck) {
        qDebug() << "Passwords For User:" << userName << "Are The Same";
        return true;
    }
    //if the passwords do not match
    else {
        qDebug() << "Passwords For User: " << userName << " Are Not The Same";
        return false;
    }


}

bool Queries::addUser(QString userName, QString password)
{
    int score = 0;
    qDebug("Adding New User");
    //prepare to add userName, password, and highScore to database for table Player
    QSqlQuery query(db);
    query.prepare("INSERT INTO Player (userName, password, highScore) "
                  "VALUES (:userName, :password, :highScore)");
    query.bindValue(":userName", userName);
    query.bindValue(":password", password);
    query.bindValue(":highScore", score);
    if (query.exec())
    {
        //if values are added to database
        qDebug("Successful Add");
        query.finish();
        return true;
    }
    else {
        //if values are not added to database
        qDebug("Failed");
        qDebug() << query.lastError();
        query.finish();
        return false;
    }
}

QString Queries::selectBestServer()
{
    //update to nicks version
    QString bestServer;
    qDebug("Getting Best Server");
    // query database to find server with lowest games being played
    QSqlQuery query(db);
    query.prepare("SELECT serverAddress FROM Server WHERE numGames = (SELECT MIN(numGames) FROM Server WHERE numGames < maxGames)");
    //checks query to find correct serverID
    if(query.exec())
    {
        while(query.next())
        {
            // iterate through and find the serverID with lowest games
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
void Queries::updateServerInfo(QString serverData)
{
    //need more data
    qDebug("Updating Server Infomation");
    QSqlQuery query(db);
    query.prepare("INSERT INTO something VALUES (:serverData)");
    query.bindValue(":serverData", serverData);
    if(query.exec())
    {
        qDebug("Server Updated");
        query.finish();
    }
    else{

        qDebug("Error");
        qDebug() << query.lastError();
        query.finish();
    }
}


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
    //execute
    //while(query.next())
    //serverlist.append(query.value[0];
    //outside while, return server list
}
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


void Queries::expiredDormantServers()
{
    qDebug("Removing Dormant Servers");
    QSqlQuery query(db);
    int dormant = 1;
    // query database to find dormant servers
    query.prepare("DELETE FROM Server WHERE Dormant = (:dormant)");
    query.bindValue(":dormant", dormant);
    //delete from Server table if server is found dormant
    //better to delete or update to null? UPDATE TABLE SET columnName = null WHERE YourCondition
    if(query.exec())
    {
        qDebug("All Dormant Servers Removed");

    }
    else {
        qDebug("Error Occured");
        qDebug() << query.lastError();
    }
}
void Queries::setSeat(QString userName, int gameID){
    qDebug("Setting Seats");
    QSqlQuery query(db);
    query.prepare("INSERT INTO Seat (userName, gameID) VALUES (:userName, :gameID)");
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    if(query.exec())
    {
        qDebug("Seat Settings Done");

    }
    else {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

void Queries::updateSeat(QString userName, int gameID){
    //Used to add players to game and update the Seat
    qDebug ("Updating Seats");
    QSqlQuery query(db);
    query.prepare("UPDATE Seat SET gameID = (:gameID) WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    if (query.exec()){
        qDebug("Successfully Updated Seats");
    } else {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

void Queries::updateNumPlayer(int gameID){
    //Updates the number of players
    // if action is true add a player and if action is false remove a player
    qDebug ("Updating number of players in Game");
    QSqlQuery query(db);
    query.prepare("UPDATE Game SET numPlayers = numPlayer + 1 WHERE gameID = (:gameID)");
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

void Queries::updatePlayerScore(QString userName, int score) {

    qDebug("Updating Player Score");
    QSqlQuery query(db);
    query.prepare("UPDATE Player SET highScore = (:score) + highScore WHERE userName = (:userName)");
    query.bindValue(":score",score);
    query.bindValue(":userName", userName);
    if (query.exec()){
        qDebug("Successfull Score Update");
    } else {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

void Queries::createGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn) {
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
    else {
        qDebug("Error");
        qDebug() << query.lastError();
    }


}

void Queries::updateGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn) {
    //need to update correctly, on which variable is static
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
    else {
        qDebug("Error");
        qDebug() << query.lastError();
    }


}
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
    else{
        qDebug("Error");
        qDebug() << query.lastError();
    }
}
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






