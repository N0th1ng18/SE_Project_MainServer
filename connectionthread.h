#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H


#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTextCodec>

class ConnectionThread : public QThread
{
    Q_OBJECT
public:
    explicit ConnectionThread(qintptr socketID, QObject *parent = nullptr);

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
        CreateAccount,
        UserLogin,
        CreateGame,
        JoinGame,
        UserData
    };

};

#endif // CONNECTIONTHREAD_H
