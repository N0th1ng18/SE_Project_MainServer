#include "queries.h"



Queries::Queries()
{

}

Queries::~Queries()
{

}

void Queries::connect(int threadID)
{
    QString s = QString::number(threadID);
    s.append("Connection");
    db = QSqlDatabase::addDatabase("QSQLITE", s);
    db.setDatabaseName("C:/Database/db.sqlite");

    if(db.open())
    {
        qDebug("Connected to Database!");

        QSqlQuery query(db);
        query.prepare("SELECT * FROM Player");
        if(query.exec()){
            while(query.next()){
                qDebug() << query.value(0);
                qDebug() << query.value(1);
                qDebug() << query.value(2);
            }
        }

    }else{

        qDebug("Could not connect to database!");
        qDebug() << db.lastError();
    }
}
void Queries::disconnect()
{

}

bool Queries::checkUser(QString userName)
{
    qDebug("Checking username");
    //querey database with username
    QSqlQuery query;
    query.prepare("SELECT userName FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);

    if(query.exec()) {
        if (query.next()) {
            qDebug() << "User " + userName + " Found";
            return true;
        }
        else {
            qDebug() << "User " + userName + " Not Found";
            return false;
        }
    } return false; // will never reach here if command is valid
}


bool Queries::checkPassword(QString userName, QString password)
{
    qDebug("Checking Passwords");
    //querey data with username to get password
    QSqlQuery query;
    QVariant pass;
    query.prepare("SELECT password FROM Player WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    if(query.exec()){
        while(query.next()){
            pass = query.value(0);
           // qDebug() << query.value(0);
           // qDebug() << pass;

        }
    }
    if (password == pass) {
        qDebug("Password the same");
        return true;
    }
    else {
        qDebug("Passwords dont match");
        return false;
    }

}

bool Queries::addUser(QString userName, QString password)
{
    int score = 0;
    qDebug("Adding New User");
    QSqlQuery query;
    query.prepare("INSERT INTO Player (userName, password, highScore) "
                  "VALUES (:userName, :password, :highScore)");
    query.bindValue(":userName", userName);
    query.bindValue(":password", password);
    query.bindValue(":highScore", score);
    if (query.exec())
    {
        qDebug("Successful Add");
        return true;
    }
    else {
        qDebug("Failed");
        qDebug() << query.lastError();
        return false;
    }
}

QString Queries::selectBestServer()
{
    QString bestServer = "";
    qDebug("Getting Best Server");
    QSqlQuery query(db);
    query.prepare("SELECT serverAddress, serverPort FROM server WHERE numGames < maxGames ORDER BY numGames;");

    if(query.exec())
    {  
        while(query.next()){
            bestServer.append(query.value(0).toString());
            bestServer.append("|");
            bestServer.append(query.value(1).toString());
            break;
        }
    }
    if(bestServer == "")
    {
        qDebug()<<"No Server Available";
        return nullptr;
    }
    qDebug() << bestServer;
    return bestServer;

}

void Queries::updateServerInfo(QString serverData)
{

}

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
           QString st = query.value(0).toString();

           serverList.append(st);
        }
    }
    return serverList;
    //execute
    //while(query.next())
    //serverlist.append(query.value[0];
    //outside while, return server list
}

QList<QString> Queries::getUserGameData(QString userName)
{
    //not right
    qDebug("Getting User Game Data");
    QList<QString> gameData;
    QSqlQuery query;
    query.prepare("SELECT userName FROM Seat WHERE userName = (:userName)");
    query.bindValue(":userName", userName);
    if(query.exec())
    {
        while(query.next())
        {
           QString st = query.value(0).toString();

           gameData.append(st);
        }
    }
    return gameData;
}

void Queries::expiredDormantServers()
{
//find server running over MAX_DORMANT_TIME and then remove all existence of the game from the database, players and server.
}

void updateSeat(QString userName, int gameId){
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

void updateNumPlayer(QString userName, int gameId, bool action){
    //Updates the number of players
    // if action is true add a player and if action is false remove a player
    qDebug ("Update number of players in Game");

    QSqlQuery query;
    int num;

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


