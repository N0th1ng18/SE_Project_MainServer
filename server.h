#ifndef SERVER_H
#define SERVER_H


#include <QTcpServer>
#include "connectionthread.h"

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

};

#endif // SERVER_H
