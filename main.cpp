#include <QCoreApplication>

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




    return a.exec();
}
