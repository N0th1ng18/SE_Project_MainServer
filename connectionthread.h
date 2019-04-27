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


    int createAccount(QList<QString>);
    int userLogin(QList<QString>);
    bool createGame(QList<QString>);
    bool joinGame(QList<QString>);
    QList<QString> userData(QList<QString>);


};

#endif // CONNECTIONTHREADCLIENT_H
