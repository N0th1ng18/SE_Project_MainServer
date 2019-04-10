#ifndef MAINSERVERPROTOCOL_H
#define MAINSERVERPROTOCOL_H

#include <QTcpServer>
#include "connectionthread.h"

class MainServerProtocol : public QTcpServer
{
    Q_OBJECT
public:
    explicit MainServerProtocol(QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // MAINSERVERPROTOCOL_H
