#include <QCoreApplication>
#include "database.h"

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Database* db = new Database();
    //db->connect();


    //MainServerProtocol server;
    //server.startServer();

    quint16 port = 5555;
    Server* server = new Server();
    server->startServer(port);

    return a.exec();
}
