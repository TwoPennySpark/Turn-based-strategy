#include "networkhost.h"
#include <iostream>

NetworkHost::NetworkHost(QString name, quint16 hostPort, int numOfPlayers): port(hostPort)
{
    qDebug() << "nm2";
    isHost = true;
    playerNum = numOfPlayers;
    playerNum = 3;
    names.reserve(playerNum);
    playersSockets.reserve(playerNum);

    // server player will go 1st so we push his name immediately
    // the other's player's names will be pushed in the order
    // in which they are connected to the server
    names.push_back(name);

    // push nullptr cause server itself has no ingame sock
    playersSockets.push_back(nullptr);

    parse_func = &NetworkHost::parse_ingame_msg;
}

NetworkHost::~NetworkHost()
{
    qDebug() << "dnh";
    delete listenSock;
}

void NetworkHost::create_serv()
{
    QEventLoop loop;

    listenSock = new QTcpServer;
    if (!listenSock->listen(QHostAddress::Any, 5555))
    {
        qDebug() << "[-]Attempt failed\n";
        return;
    }

    emit network_manager_success(isHost);
    emit new_player_connected_sig(names[0]);
    qDebug() << "NETWORK THREAD: " << QThread::currentThreadId() << "\n";

    while (names.size() != playerNum)
    {
        loop.processEvents();
        if (!listenSock->waitForNewConnection(3000))
            continue;

        QTcpSocket* sock = listenSock->nextPendingConnection();
        loop.connect(sock, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                     this, QOverload<QAbstractSocket::SocketError>::of(&NetworkHost::handle_pregame_disconnect));
        if (!sock->waitForReadyRead(5000))
            continue;

        handle_new_player(sock);
    }

    qDebug() << "START\n";

    listenSock->close();
    for (int i = 1; i < playersSockets.size(); i++)
    {
        loop.connect(playersSockets[i], &QTcpSocket::readyRead, this, &NetworkHost::readyRead);
        loop.disconnect(playersSockets[i], QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                        this, QOverload<QAbstractSocket::SocketError>::of(&NetworkHost::handle_pregame_disconnect));
        loop.connect(playersSockets[i], QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                     this, QOverload<QAbstractSocket::SocketError>::of(&NetworkHost::handle_ingame_disconnect));
    }

    emit this_player_turn_start();
    broadcast_start_message();
    emit start_multiplayer_game();

    loop.exec();
}

void NetworkHost::handle_new_player(QTcpSocket *newPlayerSock)
{
    QByteArray newName;

    newName = newPlayerSock->read(64);
    if (names.contains(newName) || static_cast<uint>(newName.size()) < MIN_NAME_LENGTH)
    {
        preGameCmd cmd;
        QByteArray payloadArr;

        cmd.set_type(PREGAME_NW_CMD_PLAYER_REQ_CHANGE_NAME);

        payloadArr.resize(cmd.ByteSize());
        cmd.SerializeToArray(payloadArr.data(), payloadArr.size());

        prepend_length_prefix(payloadArr);

        newPlayerSock->write(payloadArr, payloadArr.size());
        newPlayerSock->close();
    }
    else
    {
        names.push_back(newName);
        send_list_of_names_to_new_player(newPlayerSock);
        broadcast_player_connect(names[names.size()-1]);
        playersSockets.push_back(newPlayerSock);

        emit new_player_connected_sig(newName);
    }
}

void NetworkHost::send_list_of_names_to_new_player(QTcpSocket *newPlayerSock)
{// length-prefix framing
    preGameCmd msg;
    QByteArray payloadArr;

    msg.set_type(PREGAME_NW_CMD_PLAYER_CONNECTED);

    for (auto name: names)
    {
        msg.set_name(name.toUtf8());

        payloadArr.resize(msg.ByteSize());
        msg.SerializeToArray(payloadArr.data(), payloadArr.size());

        prepend_length_prefix(payloadArr);

        newPlayerSock->write(payloadArr, payloadArr.size());

        payloadArr.clear();
        msg.clear_name();
    }
}

template<class T>
void NetworkHost::broadcast_cmd(const T& cmd) const
{// serialize cmd, perform length-prefix framing and send
    QByteArray payloadArr;
    payloadArr.resize(cmd.ByteSize());
    cmd.SerializeToArray(payloadArr.data(), payloadArr.size());

    broadcast_byte_array(payloadArr);
}

void NetworkHost::broadcast_byte_array(QByteArray &payloadArray) const
{
    prepend_length_prefix(payloadArray);

    for (int i = 1; i < playersSockets.size(); i++)
        playersSockets[i]->write(payloadArray, payloadArray.size());
}

void NetworkHost::prepend_length_prefix(QByteArray &payloadArr) const
{
    QByteArray payloadSizeArr;
    int32_t packetSize = 0;
    QDataStream packetSizeStream(&payloadSizeArr, QIODevice::WriteOnly);

    packetSize = payloadArr.size();
    packetSizeStream << packetSize;

    payloadArr.prepend(payloadSizeArr);
}

void NetworkHost::readyRead()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(QObject::sender());
    if (sock != playersSockets[curPlayerIndex])
        return;

    do
    {
        isPrefixRead ? read_and_parse_frame() : read_frame_size_prefix();
    } while (sock->bytesAvailable());
}

void NetworkHost::read_frame_size_prefix()
{
    data.append(playersSockets[curPlayerIndex]->read(LENGTH_PREFIX_SIZE - static_cast<uint>(data.size())));

    if (static_cast<uint>(data.size()) == LENGTH_PREFIX_SIZE)
    {
        isPrefixRead = true;
        frameSize = byteArrayToInt(data);

        read_and_parse_frame();
    }
}

void NetworkHost::read_and_parse_frame()
{
    data.append(playersSockets[curPlayerIndex]->read(frameSize + LENGTH_PREFIX_SIZE - data.size()));

    if (frameSize + LENGTH_PREFIX_SIZE == data.size())
    {
        isPrefixRead = false;

        forward_incoming_cmd_to_other_players(data);

        data = data.mid(LENGTH_PREFIX_SIZE);
        (this->*parse_func)();

        data.clear();
    }
}

void NetworkHost::forward_incoming_cmd_to_other_players(const QByteArray &payloadArray) const
{
    for (int i = 1; i < playersSockets.size(); i++)
        if (!(playersSockets[i] == playersSockets[curPlayerIndex]))
            playersSockets[i]->write(payloadArray, payloadArray.size());
}

int NetworkHost::parse_ingame_msg()
{
    inGameCmd msg;
    QVector<uint> args;

    if (!msg.ParseFromArray(data, data.size()))
    {
        qDebug() << "[-]Failed to parse data:" << data << "\n";
        return 1;
    }

    for (int i = 0; i < msg.args_size(); i++)
        args.push_back(msg.args(i));
    emit recv_ingame_cmd_for_execution(msg.type(), args);

    if (msg.type() == INGAME_NW_CMD_NEXT_TURN)
        next_turn();

    return 0;
}

void NetworkHost::send_this_player_ingame_cmd(const uint type, const QVector<uint> args)
{
    inGameCmd cmd;

    qDebug() << "NETWORK THREAD: " << QThread::currentThreadId() << "\n";

    cmd.set_type(type);
    for (int i = 0; i < args.size(); i++)
        cmd.add_args(args[i]);

    if (cmd.type() == INGAME_NW_CMD_NEXT_TURN)
        next_turn();

    broadcast_cmd<inGameCmd>(cmd);
}

void NetworkHost::broadcast_player_connect(const QString& name)
{
    // when new player connects we need to broadcast it's name
    preGameCmd msg;
    msg.set_type(PREGAME_NW_CMD_PLAYER_CONNECTED);
    msg.set_name(name.toStdString());

    qDebug() << "Broadcast connect:" << name;

    broadcast_cmd<preGameCmd>(msg);
}

void NetworkHost::broadcast_player_disconnect(const unsigned int index)
{
    // when player disconnects we only need it's index num
    // to delete note about it from all other players
    preGameCmd msg;
    msg.set_type(PREGAME_NW_CMD_PLAYER_DISCONNECTED);
    msg.set_indexnum(index);

    broadcast_cmd<preGameCmd>(msg);
}

void NetworkHost::broadcast_start_message()
{
    preGameCmd msg;
    msg.set_type(PREGAME_NW_CMD_START_GAME);

    broadcast_cmd<preGameCmd>(msg);
}

void NetworkHost::next_turn()
{
    curPlayerIndex = (curPlayerIndex + 1) % playerNum;
    while (lostPlayerIndexes.contains(curPlayerIndex))
        curPlayerIndex = (curPlayerIndex + 1) % playerNum;
    if (curPlayerIndex == 0)
        emit this_player_turn_start();
}

void NetworkHost::handle_pregame_disconnect(QAbstractSocket::SocketError sockError)
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(QObject::sender());

    int index = playersSockets.indexOf(sock);
    qDebug() << "(" << QThread::currentThreadId() << "):[-]Error:" << sock->errorString() << "\nPlayer " << names[index] << " disconnected";

    emit player_disconnected_sig(names[index]);

    playersSockets.remove(index);
    names.remove(index);

    this->broadcast_player_disconnect(static_cast<uint>(index));

    sock->close();
}

void NetworkHost::handle_ingame_disconnect(QAbstractSocket::SocketError sockError)
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(QObject::sender());

    int index = playersSockets.indexOf(sock);
    qDebug() << "(" << QThread::currentThreadId() << "):[-]Error:" << sock->errorString() << "\nPlayer " << names[index] << " disconnected";

    lostPlayerIndexes.push_back(index);
    if (curPlayerIndex == index)
        next_turn();

    // notify everyone else that player disconnected
    inGameCmd cmd;
    QByteArray payloadArr;
    cmd.set_type(INGAME_NW_CMD_PLAYER_DISCONNECTED);
    cmd.add_args(static_cast<uint>(index));
    broadcast_cmd<inGameCmd>(cmd);

    // send cmd to game thread
    QVector<uint>args;
    args.push_back(static_cast<uint>(index));
    emit recv_ingame_cmd_for_execution(INGAME_NW_CMD_PLAYER_DISCONNECTED, args);

    sock->close();
}

void NetworkHost::initial_setup()
{
    create_serv();
}

void NetworkHost::this_player_disconnected()
{
    listenSock->close();

    std::for_each(playersSockets.begin()+1, playersSockets.end(), [](QTcpSocket* sock){sock->close();});
}

void NetworkHost::handle_player_loss(int loserIndex)
{
    lostPlayerIndexes.push_back(loserIndex);
}
