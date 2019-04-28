#include "server.h"


Server::Server(QObject *parent) :
    QTcpServer(parent)
{

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

    ConnectionThread *thread = new ConnectionThread(threadID++, socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), thread, SLOT(setup()));
    thread->start();

}
