#ifndef CONNECTIONTHREAD_H
#define CONNECTIONTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class ConnectionThread : public QThread
{
    Q_OBJECT
public:
    explicit ConnectionThread(qintptr ID, QObject *parent = nullptr);
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
};

#endif // CONNECTIONTHREAD_H
