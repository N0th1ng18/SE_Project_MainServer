#include <QCoreApplication>
#include "database.h"

#include "server.h"

enum SType{
    ClientServer,
    GameServer
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Database* db = new Database();
    db->connect();
    //Database connection happends in each thread


    //MainServerProtocol server;
    //server.startServer();

    quint16 clientPort = 5555;
    Server* clientServer = new Server(SType::ClientServer);
    clientServer->startServer(clientPort);


    //THIS WILL BE PUT IN CONNECTIONTHREADCLIENT
    quint16 serverPort = 5556;
    Server* gameServer = new Server(SType::GameServer);
    gameServer->startServer(serverPort);


    return a.exec();
}
