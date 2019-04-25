#ifndef SERVER_H
#define SERVER_H


#include <QTcpServer>
#include "connectionthreadclient.h"
#include "connectionthreadserver.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(int serverType = 0, QObject *parent = nullptr);
    void startServer(quint16 port);
signals:

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);
    int serverType;

    enum SType{
        ClientServer,
        GameServer
    };

};

#endif // SERVER_H
