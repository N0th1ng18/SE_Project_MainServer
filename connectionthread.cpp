#include "connectionthread.h"
#include <iostream>
#include <sstream>

ConnectionThread::ConnectionThread(int threadID, qintptr socketID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketID;
    this->threadID = threadID;
}

void ConnectionThread::run()
{
    queries = new Queries();
    queries->connect(threadID);

    //create Socket
    socket = new QTcpSocket();

    // set socket to socket Descriptor passed into socketID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }


    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << "Client ID: "<< socketDescriptor << " connected.";

    exec();
}

void ConnectionThread::readyRead()
{
    //If message doesnt have Active code it is sent to USERLOGIN only

    //Read All Data in Socket buffer
    QString data = socket->readAll();

    //Break message into QList
    QList<QString> messages = data.split("||");

    for(int i=0; i < messages.size(); i++){
        processMessage(messages[i]);
    }
}

void ConnectionThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    socket->deleteLater();
    exit(0);
}

void ConnectionThread::processMessage(QString message)
{
    qDebug() << message;

    //Seperate message into tokens
    QList<QString> tokens = message.split("|");



    switch(tokens[0].toInt())
    {
    case Msg::CREATEACCOUNT:
        {
            if (tokens.count() == 3){
                createAccount(tokens);
            } else {
                break;
            }
            break;
        }
    case Msg::USERLOGIN:
        {
            if (tokens.count() == 3){
                userLogin(tokens);
            }else {
                break;
            }
            break;
        }
    case Msg::CREATEGAME:
        {
            if (tokens.count() == 6){
                createGame(tokens);
            } else {
                break;

            }
            break;
        }
    case Msg::JOINGAME:
        {
            //joinGame();
            break;
        }
    case Msg::USERDATA:
        {
            //userData();
            break;
        }
    //Default Drops message
    }

}

void ConnectionThread::createAccount(QList<QString> userPass){
    QString userName = userPass[1];
    QString password = userPass[2];
    // passes the Username and password
    if (!queries->checkUser(userName)){
        if(queries->addUser(userName, password)){
            qDebug("Sent username and password");
        }
    } else {
        qDebug("User needs a different username and return info to client");
    }

}

void ConnectionThread::userLogin(QList<QString> login){
    QString userName = login[1];
    QString password = login[2];

    if (queries->checkUser(userName)){
        qDebug("User found");
        if (queries->checkPassword(userName, password)){
            qDebug("Username and password sent");
        } else {
            qDebug("Username and password not sent and ");
        }
    } else {

        qDebug("User not found");
    }



}

void ConnectionThread::createGame(QList<QString> createGame){
    int gameId = createGame[1].toInt();
    int serverId = createGame[2].toInt();
    int roomNum = createGame[3].toInt();
    int numPlayers = createGame[4].toInt();
    int turns = createGame[5].toInt();

    queries->createGame(gameId, serverId, roomNum, numPlayers, turns);

}

void ConnectionThread::joinGame(QList<QString> join){

    QString username = join[0];
    int gId = join[1].toInt();
    bool add = true;

    //Calls the query for updating the Seat Table
    queries ->updateSeat(username, gId);
    //Calls the query for updating the number of players in the Game
    queries ->updateNumPlayer(username, gId, add);

}

void ConnectionThread::userData(QList<QString>){
    //Updates Player data
}
