#include "connectionthreadserver.h"

ConnectionThreadServer::ConnectionThreadServer(qintptr socketID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketID;
}

void ConnectionThreadServer::run()
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

    qDebug() << "Server ID: "<< socketDescriptor << " connected.";

    exec();
}

void ConnectionThreadServer::readyRead()
{
    //Read All Data in Socket buffer
    QString data = socket->readAll();

    //Break message into QList
    QList<QString> messages = data.split("||");

    for(int i=0; i < messages.size(); i++){
        processMessage(messages[i]);
    }
}

void ConnectionThreadServer::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    socket->deleteLater();
    exit(0);
}

void ConnectionThreadServer::processMessage(QString message){
    //Seperate message into tokens
    QList<QString> tokens = message.split("|");

    switch(tokens[0].toInt())
    {
    case Msg::GAMEOVER:

        break;
    //Default Drops message
    }

}
