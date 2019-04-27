#include "queries.h"

Queries::Queries()
{

}

Queries::~Queries()
{

}

bool Queries::checkUser(QString userName)
{
    qDebug() << "Checking User Names For: " << userName;
    //querey database with username
    QSqlQuery query;
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
    QSqlQuery query;
    QString passCheck;
    query.prepare("SELECT password FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
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
        qDebug() << "Passwords For User: " << userName << " Are Not The Same";        return false;
    }


}

bool Queries::addUser(QString userName, QString password)
{
    int score = 0;
    qDebug("Adding New User");
    //prepare to add userName, password, and highScore to database for table Player
    QSqlQuery query;
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
    QSqlQuery query;
    query.prepare("SELECT serverID, serverAddress FROM Server WHERE numGames = (SELECT MIN(numGames) FROM Server WHERE numGames < maxGames)");
    //checks query to find correct serverID
    if(query.exec())
    {
        while(query.next())
        {
            // iterate through and find the serverID with lowest games
            bestServer.append(query.value(0).toString());
            bestServer.append(":");
            bestServer.append(query.value(1).toString());
            qDebug("Found Best Server");
        }
    }
    if(bestServer == "")
    {
        qDebug() << "No server avalible";
    }
    qDebug() << "Server Id : Server Address: " << bestServer;
    query.finish();
    return bestServer;
}

void Queries::updateServerInfo(QString serverData)
{
    //need more data
    qDebug("Updating Server Infomation");
    QSqlQuery query;
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

/*
QList<QString> Queries::getServerData(int gameID)
{
    qDebug("Getting Server Data");
    QList<QString> serverList;
    QSqlQuery query;
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
    QSqlQuery query;
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
void Queries::expiredDormantServers()
{
    qDebug("Removing Dormant Servers");
    QSqlQuery query;
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
*/
bool updateSeat(QString userName, int gameId){
    //Used to add players to game and update the Seat
    qDebug ("Update Seat Data");

    QSqlQuery query;
    query.prepare("INSERT INTO Seat (userName, gameID)"
                  "VALUES(:userName, :gameID)");
    query.bindValue(":userName", userName);
    query.bindValue(":gameID", gameId);
    if (query.exec()){
        qDebug("Successfull");
    } else {
        qDebug("Failed");
        qDebug() << query.lastError();
    }
}

bool updateNumPlayer(QString userName, int gameId, bool action){
    //Updates the number of players
    // if action is true add a player and if action is false remove a player
    qDebug ("Update number of players in Game");

    QSqlQuery query;
    int num;

    int ori;
    query.prepare("SELECT numPlayers FROM Game WHERE gameId = (:gameId)");
    query.bindValue(":gameId", gameId);
    ori = query.value(0).toInt();

    if (action){
        //adds a player
        query.prepare("SELECT numPlayers FROM Game WHERE gameId = (:gameId)");
        query.bindValue(":gameId", gameId);
        num = query.value(0).toInt();
        num = 1 + num;
        //not done yet

    } else {
        //subtracts a player
        query.prepare("SELECT numPlayers FROM Game WHERE gameId = (:gameId)");
        query.bindValue(":gameId", gameId);
        num = query.value(0).toInt();
        num = num - 1;
        //not done yet
    }
}
bool createGame(int gameId, int serverId, int roomNum, int numPlayers, int turns){
    return false;
}

