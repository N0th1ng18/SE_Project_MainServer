#ifndef MAINSERVERPROTOCOL_H
#define MAINSERVERPROTOCOL_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MainServerProtocol : public QTcpServer
{
    Q_OBJECT
public:
    explicit MainServerProtocol(QObject *parent = nullptr);
    void startServer();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnect();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;

protected:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // MAINSERVERPROTOCOL_H
