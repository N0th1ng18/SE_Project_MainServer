#include <QCoreApplication>
#include <QTcpSocket>
#include "queries.h"
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
    Queries * qy = new Queries();
    qy->connectDB("bob");
    qy->createGameID();

    /*BEGIN TEST - REMOVE BEFORE RELEASE

    QTcpSocket *testConnection = new QTcpSocket();
    testConnection->connectToHost("192.168.1.109", 5557);
    testConnection->waitForConnected();

    for(int i=0; i<6; i++){
    testConnection->write("0");
    testConnection->flush();
    if(testConnection->waitForReadyRead())
        qDebug() << testConnection->readAll();


    }
    END TEST - REMOVE BEFORE RELEASE*/



    return a.exec();
}
