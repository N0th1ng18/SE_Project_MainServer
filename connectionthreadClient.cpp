#include "connectionthreadclient.h"

ConnectionThreadClient::ConnectionThreadClient(qintptr socketID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketID;
}

void ConnectionThreadClient::run()
{
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

void ConnectionThreadClient::readyRead()
{
    //Read All Data in Socket buffer
    QString data = socket->readAll();

    //Break message into QList
    QList<QString> messages = data.split("||");

    for(int i=0; i < messages.size(); i++){
        processMessage(messages[i]);
    }
}

void ConnectionThreadClient::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    socket->deleteLater();
    exit(0);
}

void ConnectionThreadClient::processMessage(QString message){
    qDebug() << message;

    //Seperate message into tokens
    QList<QString> tokens = message.split("|");



    switch(tokens[0].toInt())
    {
    case Msg::CREATEACCOUNT:
        {

        break;
        }
    case Msg::USERLOGIN:
        {

        break;
        }
    case Msg::CREATEGAME:
        {

        break;
        }
    case Msg::JOINGAME:
        {

        break;
        }
    case Msg::USERDATA:
        {

        break;
        }
    //Default Drops message
    }

}

void ConnectionThreadClient::createAccount(QList<QString> userPass){
    QString userName = userPass[0];
    QString password = userPass[1];
    // passes the Username and password

}

void ConnectionThreadClient::userLogin(QList<QString> login){
    QString userName = login[0];
    QString password = login[1];

}

void ConnectionThreadClient::createGame(QList<QString> createGame){
    QString gameId = createGame[0]; //needs to be converted to int for database
    QString serverId = createGame[1]; //needs to be converted to int for database
    QString roomNum = createGame[2]; //needs to be converted to int for database
    QString numPlayers = createGame[3]; //needs to be converted to int for database
    QString turn = createGame[4]; //needs to be converted to int for database

}

void ConnectionThreadClient::joinGame(QList<QString>){
    //Calls Seat

}

void ConnectionThreadClient::userData(QList<QString>){
    //Updates Player data

}
