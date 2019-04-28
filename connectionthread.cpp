#include "connectionthread.h"
#include <iostream>
#include <sstream>

ConnectionThread::ConnectionThread(int threadID, qintptr socketID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketID;
    this->threadID = threadID;
}

void ConnectionThread::run()
{
    queries = new Queries();
    //queries->connect(threadID);

    //create Socket
    socket = new QTcpSocket();

    // set socket to socket Descriptor passed into socketID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }


    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    qDebug() << "Client ID: "<< socketDescriptor << " connected.";

    exec();
}

void ConnectionThread::readyRead()
{
    //If message doesnt have Active code it is sent to USERLOGIN only

    //Read All Data in Socket buffer
    QString data = socket->readAll();

    //Break message into QList
    QList<QString> messages = data.split("||");

    for(int i=0; i < messages.size(); i++){
        processMessage(messages[i]);
    }
}

void ConnectionThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    socket->deleteLater();
    exit(0);
}

void ConnectionThread::processMessage(QString message)
{
    qDebug() << message;

    //Seperate message into tokens
    QList<QString> tokens = message.split("|");



    switch(tokens[0].toInt())
    {
    case Msg::CREATEACCOUNT:
        {
            int info;
            if (tokens.count() == 3){
              info = createAccount(tokens);
              if (info == 1){
                  //Login Successful
                  constructMessage( "0|", "True||");
              } else if (info == 2){
                  //Username unavalable
                  constructMessage("0|", "False||");
              } else if (info == 3){
                 //Creation failed
                 constructMessage("0|", "False||");
              }
            } else {
                break;
            }
            break;
        }
    case Msg::USERLOGIN:
        {
            int info;
            if (tokens.count() == 3){
               info = userLogin(tokens);
               if (info == 1){
                   //Successful
                   constructMessage( "1|", "True||");
               }
               else if (info == 2){
                   //User not found
                   constructMessage( "1|", "False||");
               }
               else if (info == 3){
                    //Login Failed
                   constructMessage( "1|", "False||");
               }
            }else {
                break;
            }
            break;
        }
    case Msg::CREATEGAME:
        {
            if (tokens.count() == 6){
                if (createGame(tokens)){
                    //send back true


                }else {
                    //send back false
                }
            } else {
                break;
            }
            break;
        }
    case Msg::JOINGAME:
        {
            //joinGame();
            if (tokens.count() == 3){
                if (joinGame(tokens)){
                    //send true
                } else {
                    //send false
                }
            } else {
                break;
            }
            break;
        }
    case Msg::USERDATA:
        {
            //userData();
            QList<QString> rlist;
            if (tokens.count() == 2){
                rlist.append(userData(tokens));
                if (rlist[0] == "FALSE"){
                    constructMessage("4|","0|False||");
                    break;
                }else {
                    //Send List to Client
                    int cot = rlist.count();


                    //std::string out;
                    //std::stringstream ss;
                    //ss << cot;
                    //out = ss.str();


                    //QString srt = rlist.join("");
                    //srt = rlist.join("|");
                    //out.append(srt);

                    constructMessage("4|", );
                }
            }
            break;
        }
    //Default Drops message
    }

}

int ConnectionThread::createAccount(QList<QString> userPass){
    QString userName = userPass[1];
    QString password = userPass[2];
    // passes the Username and password
    if (!queries->checkUser(userName)){
        if(queries->addUser(userName, password)){
            qDebug("Sent username and password");
            return 1;
        } else {
            qDebug("Creation failed");
            return 3;
        }
    }else {
        qDebug("User needs a different username and return info to client");
        return 2;
    }

}

int ConnectionThread::userLogin(QList<QString> login){
    QString userName = login[1];
    QString password = login[2];

    if (queries->checkUser(userName)){
        qDebug("User found");
        if (queries->checkPassword(userName, password)){
            return 1;
            qDebug("Username and password sent");
        } else {
            return 3;
            qDebug("Username and password not sent and ");
        }
    } else {
        return 2;
        qDebug("User not found");
    }



}

bool ConnectionThread::createGame(QList<QString> createGame){
    int gameId = createGame[1].toInt();
    int serverId = createGame[2].toInt();
    int roomNum = createGame[3].toInt();
    int numPlayers = createGame[4].toInt();
    int turns = createGame[5].toInt();

    if (queries->createGame(gameId, serverId, roomNum, numPlayers, turns)){
        return true;
    }else {
        return false;
    }

}

bool ConnectionThread::joinGame(QList<QString> join){

    QString username = join[1];
    int gId = join[2].toInt();
    bool add = true;

    if (queries ->updateSeat(username, gId) && queries ->updateNumPlayer(username, gId, add))
    {
       return true;
    } else {
        return false;
    }


}

QList<QString> ConnectionThread::userData(QList<QString> data){
    //returns gameid connected to a userName
    QString username = data[1];

    QList<QString> gamelist;

    gamelist.append(data);

    gamelist = queries->getUserGameData(username);

    if (gamelist.size() != 0){
        return gamelist;
    } else {
        QList<QString> error;
        error.append("FAlSE");
        return error;

    }
}


void ConnectionThread::constructMessage(QString mes, QString info){

    QString s = mes + info;

     QByteArray array;
     array.append(s);

     socket->write(array);
     socket->flush(array);
}



