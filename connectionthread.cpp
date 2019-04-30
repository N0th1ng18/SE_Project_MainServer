#include "connectionthread.h"
/*
 *  Description:
 *      Constructor for ConnectionThread passes in threadID from server for tracking connection threads
 *
 *  Author:
 *      Isaac, Nick
 */
ConnectionThread::ConnectionThread(int threadID, qintptr socketID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketID;
    this->threadID = threadID;
}

/*
 *  Description:
 *      Initially run method,
 *      creates queries object for database connection,
 *      creates socket for client connection
 *
 *  Author:
 *      Isaac, Nick
 */
void ConnectionThread::setup()
{
    queries = new Queries();
    QString hostName = QString::number(socketDescriptor);
    queries->connectDB(hostName);


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

/*
 *  Description:
 *      Is called any time the socket recieves a message via a signal,
 *      reads entire buffer and breaks into message,
 *      has for loop to pass each message to the message processor
 *
 *  Author:
 *      Isaac
 */
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

/*
 *  Description:
 *      is called when the client disconnects
 *      set the socket to be deleted
 *
 *  Author:
 *      Isaac
 */
void ConnectionThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    exit(0);
}

/*
 *  Description:
 *      Processes messages by splitting them into tokens,
 *      chooses which method to call via the message header,
 *      passes list of tokens to the method call for the message
 *
 *  Author:
 *      Isaac, Katie, John
 */
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
              tempMessage.append(QString::number(info));
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
        createGame();
        break;
    }
    case Msg::JOINGAME:
    {
        //joinGame();
        if (tokens.count() == 3){
            if (joinGame(tokens)){
                //send true
                qDebug() << "Sent Game Address, Port to Client";
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
                tempMessage.append(rlist[0]);
                sendMessage(tempMessage);

    //Default Drops message
        }  // end of if
      }    // end of if
    }       //end of case
  }         //end of switch
}           //end of function

/*
 *  Description:
 *      Checks if client's chosen username is in the database already,
 *      Creates that user if it isn't in the database
 *
 *
 *  Author:
 *      Isaac, John
 */
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

/*
 *  Description:
 *      Checks if given username, password matches that of the database
 *      Logs in the client if it matches, doesn't if it doesn't match the database exactly
 *
 *  Author:
 *      Isaac, John
 */
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

/*
 *  Description:
 *      Chooses best server for hosting a game,
 *      reserves gameID and roomCode in the database,
 *      Contacts that server and tells it to start the game,
 *      Waits for response from chosen game server,
 *      game server sends in address and port of game thread socket,
 *      this method then sends that info to the client to connect,
 *      Tells the client if it fails so they dont wait forever
 *
 *  Author:
 *      Isaac, John
 */
bool ConnectionThread::createGame(){

    QList<QString> serverData = queries->selectBestServer();
    //Swap above with query when database is populated
    qDebug() << "in ConnectionThread::createGame";

    tempGameSocket = new QTcpSocket();  //Connection to GameServer for creation of game Thread
    tempGameSocket->connectToHost(serverData.at(0), 5556);
    qDebug() << tempGameSocket->waitForConnected();
    QString createGameMessage = "0|" + serverData.at(2) + "||";
    QByteArray tempMessage;
    tempMessage.append(createGameMessage);
    tempGameSocket->write(tempMessage);
    tempGameSocket->flush();

    tempGameSocket->waitForReadyRead();
    QString response = tempGameSocket->readAll();
    qDebug() << response;

    QStringList tokens = response.split("|", QString::SkipEmptyParts);

    int gameCreated = tokens.at(1).toInt();
    QString port = tokens.at(2);
    int gameID = serverData.at(1).toInt();
    queries->updateGamePort(gameID, port);
    QList<QString> message;
    message.append("3");
    message.append(QString::number(gameCreated));
    if (gameCreated)
    {

        message.append(serverData.at(0));
        message.append(port);
        message.append(serverData.at(2));
        sendMessage(message);
        return true;
    }
    else
    {
        qDebug() << "Game Server responded with faled game creation";
        message.append("0");
        sendMessage(message);
        return false;
    }


}

/*
 *  Description:
 *      searches database for game with given room code,
 *      returns connection info to be passed to client,
 *      passes failed to client if roomCode doesn't match a game in the database
 *
 *  Author:
 *      Isaac, John
 */
bool ConnectionThread::joinGame(QList<QString> join){

    QString userName = join[1];
    int roomCode = join[2].toInt();

    queries->updateSeat(userName, roomCode);
    queries->updateNumPlayer(roomCode);
    QList<QString> serverInfo = queries->getAddressPort(roomCode);
    QList<QString> message;
    message.append("4");
    if(!serverInfo.isEmpty())
    {
        message.append("1");
        message.append(serverInfo);
        qDebug() << message;
        return true;
    }
    else
    {
        message.append("0");
        sendMessage(message);
        return false;
    }
}

/*
 *  Description:
 *      returns all the games that a player is in upon login for quick access if disconnected;
 *      is also called when the client starts for home page game list
 *
 *  Author:
 *      Isaac, John
 */
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

/*
 *  Description:
 *      a general send message function for communicating with the client
 *      takes in a QList<QString>,
 *      converts it to a QByteArray with proper token seperation and end message flag
 *
 *  Author:
 *      Isaac
 */
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
