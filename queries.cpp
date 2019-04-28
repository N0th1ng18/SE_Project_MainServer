#include "queries.h"

//Constructor
Queries::Queries()
{

}

//Destructor
Queries::~Queries()
{

}

//Conenct to Database/Recieve new incoming connections
void Queries::connectDB(QString hostName)
{
    qDebug("Connecting to DB");
    hostName.append("Connection");
    //creates instance from database for reading/writing
    db = QSqlDatabase::addDatabase("QSQLITE", hostName);
    db.setDatabaseName("C:/Database/db.sqlite");
    //checks if database is correctly opened
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

//disconnects from database
void Queries::disconnectDB()
{
    //disconnect
    db.close();
    qDebug("Closed DB");
}

//Checks if User Name is already in database
bool Queries::checkUser(QString userName)
{
    qDebug() << "Checking User Names For: " << userName;
    //querey database with username
    QSqlQuery query(db);
    query.prepare("SELECT userName FROM Player WHERE userName = (:userName)");
    //binds value(s) passed into function with query to database
    query.bindValue(":userName", userName);
    //executes query
    if(query.exec())
    {
        if (query.next())
        {
            //if userName is found
            qDebug() << "User " + userName + " Found";
            query.finish();
            return true;
        }
        else
        {
            //if userName is not found
            qDebug() << "User " + userName + " Not Found";
            query.finish();
            return false;
        }
    } return false; // will never reach here if command is valid
}

//Checks for correct password for user from userName
bool Queries::checkPassword(QString userName, QString password)
{
    qDebug() << "Checking Passwords For: " << userName;
    //querey data with username to get password
    QSqlQuery query(db);
    QString passCheck;
    //gets password from database
    query.prepare("SELECT password FROM Player WHERE userName = (:userName)");
    //binds value(s) passed into function with query to database
    query.bindValue(":userName", userName);
    //executes query
    if(query.exec())
    {
        while(query.next())
        {
           //sets the password related to userName to variable pass for checking
           passCheck = query.value(0).toString();
        }
    }
    query.finish();
    //if the password entered and the password found within query are the same
    if (password == passCheck)
    {
        qDebug() << "Passwords For User:" << userName << "Are The Same";
        return true;
    }
    //if the passwords do not match
    else
    {
        qDebug() << "Passwords For User: " << userName << " Are Not The Same";
        return false;
    }


}

//adds user to database
bool Queries::addUser(QString userName, QString password)
{
    //all new players score will start at 0
    int score = 0;
    qDebug("Adding New User");
    //prepare to add userName, password, and highScore to database for table Player
    QSqlQuery query(db);
    query.prepare("INSERT INTO Player (userName, password, highScore) "
                  "VALUES (:userName, :password, :highScore)");
    //binds value(s) passed into function with query to database
    query.bindValue(":userName", userName);
    query.bindValue(":password", password);
    query.bindValue(":highScore", score);
    //executes query
    //if values are added to database
    if (query.exec())
    {
        qDebug("Successful Add");
        query.finish();
        return true;
    }
    //if values are not added to database
    else
    {
        qDebug("Failed");
        qDebug() << query.lastError();
        query.finish();
        return false;
    }
}

//Selects the best server based on lowest games
QString Queries::selectBestServer()
{
    //variable used to return best server
    QString bestServer;
    qDebug("Getting Best Server");
    // query database to find server with lowest games being played
    QSqlQuery query(db);
    query.prepare("SELECT serverAddress FROM Server WHERE numGames = (SELECT MIN(numGames) FROM Server WHERE numGames < maxGames)");
    //executes query
    //checks database to find correct serverID
    if(query.exec())
    {
        // iterate through and find the serverID with lowest games
        while(query.next())
        {
            bestServer.append(query.value(0).toString());
            qDebug("Found Best Server");
        }
    }
    //if no best server is found
    if(bestServer == "")
    {
        qDebug() << "No server avalible";
    }
    qDebug() << "Server Address: " << bestServer;
    query.finish();
    return bestServer;
}

//ONLY USED IF GROUP GETS SAVES WORKING
void Queries::updateServerInfo(QString serverData)
{
    //need more data
    qDebug("Updating Server Infomation");
    QSqlQuery query(db);
    query.prepare("INSERT INTO something VALUES (:serverData)");
    //binds value(s) passed into function with query to database
    query.bindValue(":serverData", serverData);
    //executes query
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

//ONLY USED IF GROUP GETS SAVES WORKING
QList<QString> Queries::getServerData(int gameID)
{
    qDebug("Getting Server Data");
    QList<QString> serverList;
    QSqlQuery query(db);
    query.prepare("SELECT gameID FROM Seat WHERE gameID = (:gameID)");
    //binds value(s) passed into function with query to database
    query.bindValue(":gameID", gameID);
    //executes query
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

//gets a list of games that the user is currently in
QList<QString> Queries::getUserGameData(QString userName)
{
    qDebug("Getting User Games List Data");
    //variable used to return the list of games user is in
    QList<QString> gameList;
    QSqlQuery query(db);
    query.prepare("SELECT gameID FROM Seat WHERE userName = (:userName)");
    //binds value(s) passed into function with query to database
    query.bindValue(":userName", userName);
    //execute query
    if(query.exec())
    {
        while(query.next())
        {
            //iterate through database and append gameID for each game the user is in
            QString gamesIn = query.value(0).toString();
            gameList.append(gamesIn);
        }
    }
    //return the list
    return gameList;
}

//ONLY USED IF GROUP GETS SAVES WORKING
void Queries::expiredDormantServers()
{
    qDebug("Removing Dormant Servers");
    QSqlQuery query(db);
    int dormant = 1;
    // query database to find dormant servers
    query.prepare("DELETE FROM Server WHERE Dormant = (:dormant)");
    //binds value(s) passed into function with query to database
    query.bindValue(":dormant", dormant);
    //executes query
    if(query.exec())
    {
        qDebug("All Dormant Servers Removed");
    }
    else {
        qDebug("Error Occured");
        qDebug() << query.lastError();
    }
}

//sets the seat for the user in a game
void Queries::setSeat(QString userName, int gameID)
{
    qDebug("Setting Seats");
    QSqlQuery query(db);
    query.prepare("INSERT INTO Seat (userName, gameID) VALUES (:userName, :gameID)");
    //binds value(s) passed into function with query to database
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    //executes query
    //set the seats of the user with the gameID
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
//Updates the current seats of the user
void Queries::updateSeat(QString userName, int gameID){
    //Used to add players to game and update the Seat
    qDebug ("Updating Seats");
    QSqlQuery query(db);
    query.prepare("UPDATE Seat SET gameID = (:gameID) WHERE userName = (:userName)");
    //binds value(s) passed into function with query to database
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameID);
    //executes query
    //updates seats
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

//Updates the number of players in a certain game
void Queries::updateNumPlayer(int gameID)
{
    //Updates the number of players
    // if action is true add a player and if action is false remove a player
    qDebug ("Updating number of players in Game");
    QSqlQuery query(db);
    query.prepare("UPDATE Game SET numPlayers = numPlayers + 1 WHERE gameID = (:gameID)");
    //binds value(s) passed into function with query to database
    query.bindValue(":gameID", gameID);
    //executes query
    if (query.exec())
    {
        qDebug("Successfully Updated Player Count");
    }
    else {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

//Updates the players score
void Queries::updatePlayerScore(QString userName, int score)
{
    qDebug("Updating Player Score");
    QSqlQuery query(db);
    query.prepare("UPDATE Player SET highScore = (:score) + highScore WHERE userName = (:userName)");
    //binds value(s) passed into function with query to database
    query.bindValue(":score",score);
    query.bindValue(":userName", userName);
    //executes query
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

//Creates a game in the database with information given
void Queries::createGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn)
{
    qDebug("Creating Game Table");
    QSqlQuery query(db);
    query.prepare("INSERT INTO Game (gameID, serverID, roomCode, numPlayers, currentTurn)"
                  "VALUES (:gameID, :serverID, :roomCode, :numPlayers, :currentTurn)");
    //binds value(s) passed into function with query to database
    query.bindValue(":gameID", gameID);
    query.bindValue(":serverID", serverID);
    query.bindValue(":roomCode", roomCode);
    query.bindValue(":numPlayers", numPlayers);
    query.bindValue(":currentTurn", currentTurn);
    //executes query
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

//Updates game with information given
void Queries::updateGame(int gameID, int serverID, QString roomCode, int numPlayers, int currentTurn)
{
    //need to update correctly, on which variable is static
    qDebug("Updating Game Table");
    QSqlQuery query(db);
    query.prepare("UPDATE Game SET serverID = (:serverID), roomCode = (:roomCode), "
                  "numPlayers = (:numPlayers), currentTurn = (:currentTurn) WHERE gameID = (:gameID)");
    //binds value(s) passed into function with query to database
    query.bindValue(":gameID", gameID);
    query.bindValue(":serverID", serverID);
    query.bindValue(":roomCode", roomCode);
    query.bindValue(":numPlayers", numPlayers);
    query.bindValue(":currentTurn", currentTurn);
    //executes query
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

//ONLY USED IF GROUP GETS SAVES WORKING
void Queries::setDormant(int serverID)
{
    qDebug("Setting server to dormant");
    QSqlQuery query(db);
    query.prepare("UPDATE Server SET Dormant = 1 WHERE serverID = (:serverID)");
    //binds value(s) passed into function with query to database
    query.bindValue(":serverID", serverID);
    //executes query
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

//Gets room code from game
QString Queries::getRoomCode(int gameID)
{
    qDebug("Getting Room Code");
    QSqlQuery query(db);
    //Variable used to return room code
    QString roomCode;
    query.prepare("SELECT roomCode FROM Game WHERE gameID = (:gameID)");
    //binds value(s) passed into function with query to database
    query.bindValue(":gameID",gameID);
    //executes query
    if(query.exec())
    {
        while(query.next())
        {
            //sets the room code from database to variable roomCode
            roomCode = query.value(0).toString();
        }
    }
        qDebug("Suecessfully Got Room Code");
        return roomCode;
 }
