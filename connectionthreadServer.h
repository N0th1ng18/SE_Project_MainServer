#ifndef CONNECTIONTHREADSERVER_H
#define CONNECTIONTHREADSERVER_H


#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTextCodec>

class ConnectionThreadServer : public QThread
{
    Q_OBJECT
public:
    explicit ConnectionThreadServer(qintptr socketID, QObject *parent = nullptr);

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
        GAMEOVER,
    };

};

#endif // CONNECTIONTHREADSERVER_H
