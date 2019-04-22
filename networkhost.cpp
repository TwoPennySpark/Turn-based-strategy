#include "networkhost.h"
#include <iostream>

NetworkHost::NetworkHost(QString name, quint16 hostPort, int numOfPlayers): port(hostPort)
{
    isHost = true;
    playerNum = numOfPlayers;
    names.reserve(playerNum);
    socket.reserve(playerNum);

    // server player will go 1st so we push his name immediately
    // the other's player's names will be pushed in the order
    // in which they are connected to the server
    names.push_back(name);
}

void NetworkHost::create_serv()
{
    QEventLoop loop;

    server = new QTcpServer;
    if (!server->listen(QHostAddress::Any, 5555))
    {
        qDebug() << "[-]Attempt failed\n";
        return;
    }

    emit network_manager_success(isHost);
    emit new_player_connected_sig(names[0]);

    while (names.size() != playerNum)
    {
        loop.processEvents();
        if (!server->waitForNewConnection(3000))
            continue;

        QTcpSocket* sock = server->nextPendingConnection();
        loop.connect(sock, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                     this, QOverload<QAbstractSocket::SocketError>::of(&NetworkHost::handle_disconnect));
        if (!sock->waitForReadyRead(5000))
            continue;

        QByteArray name(sock->read(64));
        broadcast_player_connect(name.data());

        names.push_back(name);
        socket.push_back(sock);

        send_list_of_names_to_new_player(sock);

        emit new_player_connected_sig(name);
//        qDebug() << "["<<  names.indexOf(name) << "]:" << name << socket[names.indexOf(name)-1] << "\n";
    }
    qDebug() << "START\n";
    broadcast_start_message();

//    while (1)
//    {
//        std::string typeStr;
//        std::cin >> typeStr;

//        ingame_network_cmd_types type = static_cast<ingame_network_cmd_types>(std::stoi(typeStr));
//        inGameCmd cmd;
//        QByteArray arr;

//        cmd.set_type(type);
//        serialize_ingame_msg(cmd, arr);

//        broadcast_msg(arr);
//    }
}

void NetworkHost::send_list_of_names_to_new_player(QTcpSocket *newPlayerSock)
{// length-prefix framing
    preGameCmd msg;
    QByteArray payloadArr;
    QByteArray payloadSizeArr;
    int32_t packetSize = 0;

    msg.set_type(PREGAME_NW_CMD_PLAYER_CONNECTED);

    for (auto name: names)
    {
        msg.set_name(name.toUtf8());

        payloadArr.resize(msg.ByteSize());
        msg.SerializeToArray(payloadArr.data(), payloadArr.size());

        packetSize = payloadArr.size();
        QDataStream stream(&payloadSizeArr, QIODevice::WriteOnly);
        stream << packetSize;

        newPlayerSock->write(payloadSizeArr, payloadSizeArr.size());
        newPlayerSock->write(payloadArr, payloadArr.size());
        newPlayerSock->flush();

        payloadArr.clear();
        payloadSizeArr.clear();
        msg.clear_name();
    }
}

void NetworkHost::broadcast_msg(QByteArray &payloadArray)
{// length-prefix framing
    QByteArray payloadSizeArr;
    int32_t packetSize = 0;
    QDataStream packetSizeStream(&payloadSizeArr, QIODevice::WriteOnly);

    packetSize = payloadArray.size();
    packetSizeStream << packetSize;

    for (auto sock: socket)
    {
        sock->write(payloadSizeArr, payloadSizeArr.size());
        sock->write(payloadArray, payloadArray.size());
        sock->flush();
    }
}

void NetworkHost::serialize_pregame_msg(preGameCmd &msg, QByteArray& payloadArr)
{
    payloadArr.resize(msg.ByteSize());
    msg.SerializeToArray(payloadArr.data(), payloadArr.size());
}

void NetworkHost::serialize_ingame_msg(inGameCmd &msg, QByteArray& payloadArr)
{
    payloadArr.resize(msg.ByteSize());
    msg.SerializeToArray(payloadArr.data(), payloadArr.size());
}

void NetworkHost::read_frame_size_prefix()
{
    data.append(socket[curPlayerIndex]->read(LENGTH_PREFIX_SIZE - static_cast<uint>(data.size())));
    qDebug() <<  "ARR" << data.size() << data;
    if (static_cast<uint>(data.size()) == LENGTH_PREFIX_SIZE)
    {
        isPrefixRead = true;
        frameSize = byteArrayToInt(data);
        qDebug() << "FRAME SIZE = " << frameSize;
        data.clear();

        read_and_parse_frame();
    }
}

void NetworkHost::read_and_parse_frame()
{
    data.append(socket[curPlayerIndex]->read(frameSize - data.size()));

    if (frameSize == data.size())
    {
        isPrefixRead = false;

        broadcast_msg(data);

        (this->*parse_func)();

        data.clear();
    }
}

void NetworkHost::get_and_send_ingame_cmd(ingame_network_cmd_types type, QVector<uint> &args)
{

}

void NetworkHost::readyRead()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(QObject::sender());
    if (sock != socket[curPlayerIndex])
        return;

    do
    {
        isPrefixRead ? read_and_parse_frame() : read_frame_size_prefix();
    } while (sock->bytesAvailable());
}

void NetworkHost::initial_setup()
{
    create_serv();
}

void NetworkHost::this_player_disconnected()
{
    server->close();

    std::for_each(socket.begin(), socket.end(), [](QTcpSocket* sock){sock->close();});
}

void NetworkHost::broadcast_player_connect(char *name)
{
    // when new player connects we need to broadcast it's name
    preGameCmd msg;
    msg.set_type(PREGAME_NW_CMD_PLAYER_CONNECTED);
    msg.set_name(name);

    qDebug() << "Broadcast connect:" << name;

    QByteArray arr;
    serialize_pregame_msg(msg, arr);
    broadcast_msg(arr);
}

void NetworkHost::broadcast_player_disconnect(unsigned int index)
{
    // when player disconnects we only need it's index num
    // to delete note about it from all other players
    preGameCmd msg;
    msg.set_type(PREGAME_NW_CMD_PLAYER_DISCONNECTED);
    msg.set_indexnum(index);

    QByteArray arr;
    serialize_pregame_msg(msg, arr);
    broadcast_msg(arr);
}

void NetworkHost::broadcast_start_message()
{
    preGameCmd startMsg;
    startMsg.set_type(PREGAME_NW_CMD_START_GAME);

    QByteArray arr;
    serialize_pregame_msg(startMsg, arr);
    broadcast_msg(arr);
}

void NetworkHost::handle_disconnect(QAbstractSocket::SocketError sockError)
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(QObject::sender());

    int index = socket.indexOf(sock);
    qDebug() << "[-]Error:" << sock->errorString() << "\nPlayer " << names[index+1] << " disconnected";

    emit player_disconnected_sig(names[index+1]);

    socket.remove(index);
    names.remove(index+1);

    this->broadcast_player_disconnect(static_cast<uint>(index+1));

    sock->close();
}
