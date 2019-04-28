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
    qintptr socketDescriptor;
    Queries* queries;
    int threadID;

    enum Msg
    {
        CREATEACCOUNT,
        USERLOGIN,
        CREATEGAME,
        JOINGAME,
        USERDATA
    };


    void createAccount(QList<QString>);
    void userLogin(QList<QString>);
    void createGame(QList<QString>);
    void joinGame(QList<QString>);
    void userData(QList<QString>);


};

#endif // CONNECTIONTHREADCLIENT_H
