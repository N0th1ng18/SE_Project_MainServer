#include "server.h"

/*
 *  Description:
 *      empty constructor method
 *
 *  Author:
 *      Isaac
 */
Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

/*
 *  Description:
 *      starts the TcpServer for client connection to main server
 *
 *  Author:
 *      Isaac
 */
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

/*
 *  Description:
 *      creates thread for each incomming connection for simultaneous processing
 *
 *  Author:
 *      Isaac
 */
void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";

    ConnectionThread *thread = new ConnectionThread(threadID++, socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(started()), thread, SLOT(setup()));
    thread->start();

}
