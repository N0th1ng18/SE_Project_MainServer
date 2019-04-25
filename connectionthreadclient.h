#ifndef CONNECTIONTHREADCLIENT_H
#define CONNECTIONTHREADCLIENT_H


#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTextCodec>

class ConnectionThreadClient : public QThread
{
    Q_OBJECT
public:
    explicit ConnectionThreadClient(qintptr socketID, QObject *parent = nullptr);

    void run();


signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    void processMessage(QString message);
    QTcpSocket *socket;
    qintptr socketDescriptor;

    enum Msg
    {
        CREATEACCOUNT,
        USERLOGIN,
        CREATEGAME,
        JOINGAME,
        USERDATA
    };

};

#endif // CONNECTIONTHREADCLIENT_H
