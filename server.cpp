#include "server.h"


Server::Server(int serverType, QObject *parent) :
    QTcpServer(parent)
{
    this->serverType = serverType;
}

void Server::startServer(quint16 port)
{

    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Failed to Listen on port: " << port;
    }
    else
    {
        qDebug() << "Listening to port: " << port << "...";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";

    switch(serverType)
    {
    case SType::ClientServer:
        {

        ConnectionThreadClient *thread = new ConnectionThreadClient(socketDescriptor, this);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();

        break;
        }
    case SType::GameServer:
        {

        ConnectionThreadServer *thread = new ConnectionThreadServer(socketDescriptor, this);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();

        break;
        }
    }





}
