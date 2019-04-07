#include "networkmanager.h"

NetworkManager::NetworkManager(QString name, bool createServer, int playerNum): thisPlayerName(name), isHost(createServer), maxNumOfPlayers(playerNum), numOfConnectedPlayers(0)
{
    socket.resize( isHost ? maxNumOfPlayers : 1);
    qDebug() << "THIS PLAYER NAME:" << thisPlayerName;
}

void NetworkManager::connect_to_server(QString host, unsigned short int port)
{
    qDebug() << "<" << QThread::currentThreadId() << ">Attempt connect to " << host << "::" << port << "\n";

    // connect to server
    QTcpSocket* sock = new QTcpSocket;
    sock->connectToHost(host, port);
    if (sock->waitForConnected(1000))
        qDebug() << "[!]Connected\n";

    // send player's name
    sock->write(QByteArray(thisPlayerName.toUtf8()));
    sock->flush();

    // receive list of players names
    sock->waitForReadyRead(-1);
    QByteArray data(sock->read(64));

    emit network_manager_success(isHost);

    parse_first_server_message(data);

    socket.push_back(sock);
}

void NetworkManager::parse_first_server_message(QByteArray& data)
{
    std::stringstream ss(data.toStdString());
    std::string name;

    while (std::getline(ss, name, '\n'))
    {
        playerNames.push_back(QString::fromStdString(name));
        QByteArray arr(name.data());
        emit new_player_connected_sig(arr.data());
    }
    for (auto str: playerNames)
        qDebug() << str;
}

void NetworkManager::broadcast_player_connected(char *name)
{
    for (auto sock: socket)
        sock->write(name);
}

void NetworkManager::create_server(unsigned short int port)
{
    qDebug() << "<" << QThread::currentThreadId() << "> Attempt to create server on port: " << port << "\n";
    server = new QTcpServer;

    if (!server->listen(QHostAddress::Any, port))
    {
        qDebug() << "[-]Attempt failed\n";
        return;
    }
    emit network_manager_success(isHost);

    playerNames.insert(numOfConnectedPlayers, thisPlayerName);
    numOfConnectedPlayers++;

    wait_for_players_connections();
}

void NetworkManager::wait_for_players_connections()
{
    QEventLoop loop;

    while (numOfConnectedPlayers != maxNumOfPlayers)
    {
        loop.processEvents();

        if (!server->waitForNewConnection(3000))
            continue;
        qDebug() << "[!]New connection\n";

        QTcpSocket* sock = server->nextPendingConnection();        
//        loop.connect(sock, &QTcpSocket::disconnected,
//                     this, QOverload<>::of(&NetworkManager::player_disconnected_from_this_host));
        loop.connect(sock, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                     this, QOverload<QAbstractSocket::SocketError>::of(&NetworkManager::player_disconnected_from_this_host));
        if (!sock->waitForReadyRead(5000))
            continue;

        QByteArray name(sock->read(MAX_NAME_LENGTH));

        playerNames.insert(numOfConnectedPlayers, name);
        socket.insert(numOfConnectedPlayers, sock);
        numOfConnectedPlayers++;

        QString response;
        for (auto name: playerNames)
            response += (name + "\n");
        sock->write(response.toUtf8());
        sock->flush();

        qDebug() << socket[numOfConnectedPlayers-1] << ":" << playerNames[numOfConnectedPlayers-1] << ":" << numOfConnectedPlayers;
        emit new_player_connected_sig(name.data());
    }
}

void NetworkManager::player_disconnected_from_this_host()
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(QObject::sender());
    qDebug() << "<" << QThread::currentThreadId() << "> [!]Player disconnected";
    socket.removeOne(sock);
    sock->close();
    numOfConnectedPlayers--;
}

void NetworkManager::player_disconnected_from_this_host(QAbstractSocket::SocketError sockError)
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(QObject::sender());
    qDebug() << "<" << QThread::currentThreadId() << "> [-]Error:" << sock->errorString();

    int index = socket.indexOf(sock);
    socket.remove(index);
    emit player_disconnected_sig(playerNames[index]);
    playerNames.remove(index);

    sock->close();
    numOfConnectedPlayers--;
}

void NetworkManager::this_player_disconnected()
{
    if (isHost)
        server->close();

    qDebug() << "<" << QThread::currentThreadId() << "> [!]Host disconnect\n";
    std::for_each(socket.begin(), socket.end(), [](QTcpSocket* sock){sock->close();});
}
