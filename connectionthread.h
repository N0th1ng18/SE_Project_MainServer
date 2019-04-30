#ifndef CONNECTIONTHREADCLIENT_H
#define CONNECTIONTHREADCLIENT_H
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "queries.h"

class ConnectionThread : public QThread
{
    Q_OBJECT

public:
    explicit ConnectionThread(int threadID, qintptr socketID, QObject *parent = nullptr);



signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();
    void setup();


private:
    void processMessage(QString message);
    QTcpSocket *socket;
    QTcpSocket *tempGameSocket;
    qintptr socketDescriptor;
    Queries* queries;
    int threadID;

    enum Msg
    {
        CREATEACCOUNT=1,
        USERLOGIN,
        CREATEGAME,
        JOINGAME,
        USERDATA
    };


    int createAccount(QList<QString> tokens);
    int userLogin(QList<QString> tokens);
    bool createGame();
    bool joinGame(QList<QString> tokens);
    QList<QString> userData(QList<QString> tokens);
    void sendMessage(QList<QString> tokens);

};

#endif // CONNECTIONTHREADCLIENT_H
