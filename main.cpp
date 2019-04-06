#include <QCoreApplication>
#include "mainserverprotocol.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainServerProtocol server;
    server.startServer();

    return a.exec();
}
