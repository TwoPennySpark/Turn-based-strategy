#include "networkmanager.h"

NetworkManager::NetworkManager(bool createServer, int playerNum): isHost(createServer), maxNumOfPlayers(playerNum), numOfConnectedPlayers(0)
{
    socket.reserve( isHost ? maxNumOfPlayers : 1);
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

    wait_for_players_connections();
}

void NetworkManager::connect_to_server(QString host, unsigned short int port)
{
    qDebug() << "<" << QThread::currentThreadId() << ">Attempt connect to " << host << "::" << port << "\n";

    QTcpSocket* sock = new QTcpSocket;
    sock->connectToHost(host, port);
    if (sock->waitForConnected(1000))
        qDebug() << "[!]Connected\n";

    sock->write("Player", MAX_NAME_LENGTH);
    sock->flush();

    sock->waitForReadyRead(-1);
    char data[64] = "\0";
    sock->read(data, 64);
    qDebug() << "Host name:" << data;

    socket.push_back(sock);
    emit network_manager_success(isHost);
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
        loop.connect(sock, &QTcpSocket::disconnected,
                     this, QOverload<>::of(&NetworkManager::player_disconnected_from_this_host));
        loop.connect(sock, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                     this, QOverload<QAbstractSocket::SocketError>::of(&NetworkManager::player_disconnected_from_this_host));
        if (!sock->waitForReadyRead(5000))
            continue;

        QByteArray name(sock->read(MAX_NAME_LENGTH));

        sock->write("Hello " + name);
        sock->flush();

        socket.push_back(sock);
        numOfConnectedPlayers++;

        qDebug() << sock << ":" << numOfConnectedPlayers;
        new_player_connected_sig(name.data());
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
    socket.removeOne(sock);
    sock->close();
    numOfConnectedPlayers--;
}

void NetworkManager::this_player_disconnected()
{
    if (isHost)
        server->close();

    qDebug() << "<" << QThread::currentThreadId() << "> [!]Disconnect\n";
    std::for_each(socket.begin(), socket.end(), [](QTcpSocket* sock){sock->close();});
}
