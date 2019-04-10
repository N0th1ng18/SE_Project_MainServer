#include "connectionthread.h"

ConnectionThread::ConnectionThread(qintptr ID, QObject *parent):QThread(parent)
{
    this->socketDescriptor = ID;
}

void ConnectionThread::run()
{
    qDebug() << socketDescriptor << " thread started";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << "Connection for thread: " << socketDescriptor << " connected.";

    exec();
}

void ConnectionThread::readyRead()
{
    QByteArray Data = socket->readAll();
    qDebug() << "Data from " << socketDescriptor << ": " << Data;
    socket->write(Data);
}

void ConnectionThread::disconnected()
{
    qDebug() << socketDescriptor << " disconnected.";
    socket->deleteLater();
    exit(0);
}
