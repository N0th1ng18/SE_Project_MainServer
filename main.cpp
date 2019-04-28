#include <QCoreApplication>
#include <QTcpSocket>

#include "server.h"
#include "authentication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Authentication* auth = new Authentication();

    //Client Connection Socket
    quint16 clientPort = 5555;
    Server* server = new Server();
    server->startServer(clientPort);

    //BEGIN TEST - REMOVE BEFORE RELEASE

    QTcpSocket *testConnection = new QTcpSocket();
    testConnection->connectToHost("192.168.1.109", 5557);
    testConnection->waitForConnected();

    testConnection->write("0");
    testConnection->flush();
    if(testConnection->waitForReadyRead())
        qDebug() << testConnection->readAll();

    //END TEST - REMOVE BEFORE RELEASE



    return a.exec();
}
