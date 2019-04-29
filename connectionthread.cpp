#include "connectionthread.h"

ConnectionThread::ConnectionThread(int threadID, qintptr socketID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketID;
    this->threadID = threadID;
}

void ConnectionThread::setup()
{
    queries = new Queries();
    QString hostName = QString::number(socketDescriptor);
    queries->connectDB(hostName);

    //BEGIN TEST

    qDebug() << queries->checkUser("Nick");

    //END TEST


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
    QList<QString> messages = data.split("||", QString::SkipEmptyParts);

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
    QList<QString> tokens = message.split("|", QString::SkipEmptyParts);



    switch(tokens[0].toInt())
    {
    case Msg::CREATEACCOUNT:
    {
        int info;
        if (tokens.count() == 3){
          info = createAccount(tokens);
          if (info == 1){
              //Login Successful
              QList<QString> tempMessage;
              tempMessage.append(QString::number(tokens[0].toInt()));
              tempMessage.append("|"+ QString::number(info));
              sendMessage(tempMessage);
          }
        }
        break;
    }

    case Msg::USERLOGIN:
        {
            int info;
            if (tokens.count() == 3){
               info = userLogin(tokens);
               QList<QString> tempMessage;
               tempMessage.append(QString::number(tokens[0].toInt()));
               tempMessage.append(QString::number(info));
               QList<QString> gameList = queries->getUserGameData(tokens[1]);
               tempMessage.append(QString::number(gameList.size()));
               for(QString game: gameList)
               {
                    tempMessage.append(game);
               }
               sendMessage(tempMessage);
               qDebug() << tempMessage;
            }
            break;
        }
    case Msg::CREATEGAME:
    {
        createGame(tokens);
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
                QList<QString> tempMessage;
                tempMessage.append(QString::number(tokens[0].toInt()));
                tempMessage.append("|"+ rlist[0]);
                sendMessage(tempMessage);

    //Default Drops message
        }  // end of if
      }    // end of if
    }       //end of case
  }         //end of switch
}           //end of function

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
    //int gameId = createGame[1].toInt();
    //int numPlayers = createGame[4].toInt();

    //Below query requires stuff in the database
    //QString gameServerAddress = queries->selectBestServer();
    QString gameServerAddress = "192.168.1.2";
    //Swap above with query when database is populated
    qDebug() << "in ConnectionThread::createGame";


    tempSocket = new QTcpSocket();  //Connection to GameServer for creation of game Thread
    tempSocket->connectToHost(gameServerAddress, 5556);
    qDebug() << tempSocket->waitForConnected();
    tempSocket->write("0|1||");
    tempSocket->flush();

    tempSocket->waitForReadyRead();
    QString response = tempSocket->readAll();
    qDebug() << response;

    QStringList tokens = response.split("|", QString::SkipEmptyParts);

    int gameCreated = tokens.at(1).toInt();
    QString port = tokens.at(2);
    QByteArray message = "3|";
    message.append(QString::number(gameCreated));
    if (gameCreated)
    {

        message.append("|" + gameServerAddress + "|" + port);
        message.append("|1||");
        socket->write(message);
        socket->flush();
        return true;
    }
    else
    {
        qDebug() << "Game Server responded with faled game creation";
        socket->write(message);
        socket->flush();
        return false;
    }


}

bool ConnectionThread::joinGame(QList<QString> join){

    QString userName = join[1];
    int gID = join[2].toInt();
    bool add = true;

    queries->updateSeat(userName, gID);
    queries->updateNumPlayer(gID);

    //return room code to client


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
        error.append("0");
        return error;

    }
}

void ConnectionThread::sendMessage(QList<QString> tokens){

     QByteArray array;

     for(QString each: tokens)
     {
         array.append("|");
         array.append(each);
     }
     array.append("||");

     socket->write(array);
     socket->flush();
}
