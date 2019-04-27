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
            createAccount();
            break;
        }
    case Msg::USERLOGIN:
        {
            userLogin();
            break;
        }
    case Msg::CREATEGAME:
        {
            //createGame();
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
    QString userName = userPass[0];
    QString password = userPass[1];
    // passes the Username and password

}

void ConnectionThread::userLogin(QList<QString> login){
    QString userName = login[0];
    QString password = login[1];

    if (!queries->checkUser(userName)){
        if(queries->addUser(userName, password)){
            qDebug("Sent username and password");
        }
    } else {
        qDebug("Username was found in database");
    }

}

void ConnectionThread::createGame(QList<QString> createGame){
    int gameId = createGame[0].toInt(); //needs to be converted to int for database
    int serverId = createGame[1].toInt(); //needs to be converted to int for database
    int roomNum = createGame[2].toInt(); //needs to be converted to int for database
    int numPlayers = createGame[3].toInt(); //needs to be converted to int for database
    int turns = createGame[4].toInt(); //needs to be converted to int for database


}

void ConnectionThread::joinGame(QList<QString> join){

    QString username = join[0];
    int gId = join[1].toInt();
    int addPlayer = 1;

    //Calls the query for updating the Seat Table
    //Calls the query for updating the number of players in the Game

}

void ConnectionThread::userData(QList<QString>){
    //Updates Player data
}
