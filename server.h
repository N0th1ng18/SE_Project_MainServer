#ifndef SERVER_H
#define SERVER_H


#include <QTcpServer>
#include "connectionthread.h"
#include "authentication.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer(quint16 port);
signals:

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);

    int threadID = 0;

};

#endif // SERVER_H
