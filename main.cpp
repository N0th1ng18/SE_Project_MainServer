#include <QCoreApplication>
#include <QTcpSocket>
#include "queries.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Authentication* auth = new Authentication();

    //Client Connection Socket
    quint16 clientPort = 5555;
    Server* server = new Server();
    server->startServer(clientPort);

    return a.exec();
}
