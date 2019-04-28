#include "connectionthread.h"

ConnectionThread::ConnectionThread(int threadID, qintptr socketID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketID;
    this->threadID = threadID;
}

void ConnectionThread::setup()
{
    queries = new Queries();
    QString hostName = QString::number(socketDescriptor);
    queries->connectDB(hostName);

    //BEGIN TEST

    qDebug() << queries->checkUser("Nick");

    //END TEST


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
            //createAccount();
            break;
        }
    case Msg::USERLOGIN:
        {
            //userLogin();
            break;
        }
    case Msg::CREATEGAME:
        {
            createGame(tokens);
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

}

void ConnectionThread::createGame(QList<QString> createGame){
    QString gameId = createGame[0]; //needs to be converted to int for database
    QString serverId = createGame[1]; //needs to be converted to int for database
    QString roomNum = createGame[2]; //needs to be converted to int for database
    QString numPlayers = createGame[3]; //needs to be converted to int for database
    QString turn = createGame[4]; //needs to be converted to int for database
    /*
     *
     *
     *              COME BACK TO THIS
     *                  ISAAC
     *
     *
     *
     */
    //socket->connectToHost()


}

void ConnectionThread::joinGame(QList<QString>){
    //Calls Seat

}

void ConnectionThread::userData(QList<QString>){
    //Updates Player data
}
