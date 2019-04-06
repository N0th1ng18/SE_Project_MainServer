#include "mainserverprotocol.h"

MainServerProtocol::MainServerProtocol(QObject *parent) : QTcpServer(parent)
{
}

void MainServerProtocol::startServer(){
    quint16 numport = 5555;
    qDebug() << ":5555";

    if(!this->listen(QHostAddress::Any,numport)){
        qDebug() << "Could not start server";
    }
    else{
        qDebug() << "Listen to port: " << numport << "...";
    }
}

void MainServerProtocol::incomingConnection(qintptr socketDescriptor){
    qDebug() << socketDescriptor << "Connecting...";
    socket = new QTcpSocket();
    //connect and get message
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    //disconnect
    //connect(socket, SIGNAL(disconnect()), this, SLOT(disconnect()));
    qDebug() << socketDescriptor << " Connected";
}

void MainServerProtocol::readyRead(){
    QByteArray Data = socket->readAll();
    qDebug() << socketDescriptor << "Data in: " << Data;
}

void MainServerProtocol::disconnect(){
    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
}
