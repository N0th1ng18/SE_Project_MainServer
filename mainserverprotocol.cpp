#include "mainserverprotocol.h"
#include "connectionthread.h"

MainServerProtocol::MainServerProtocol(QObject *parent) : QTcpServer(parent)
{
}

void MainServerProtocol::startServer()
{
    quint16   portNum = 1234;


    if (!this->listen(QHostAddress::AnyIPv4, portNum))
    {
        qDebug() << "Server not Started";
    }
    else
    {
        qDebug() << "Listening at " << serverAddress() << ":" << serverPort();
    }
}

void MainServerProtocol::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " connecting to Host...";
    ConnectionThread *thread = new ConnectionThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
