
#include "networkclient.h"

NetworkClient::NetworkClient(QString name, QString ip, quint16 port): hostIP(ip), hostPort(port), thisPlayerName(name)
{
    isHost = false;
    thisPlayerIndex = -1;

    isPrefixRead = false;
    frameSize = 0;
    parse_func = &NetworkClient::parse_pregame_msg;

    curPlayerIndex = 0;
}

void NetworkClient::connect_to_server()
{
    QEventLoop loop;

    qDebug() << "Attempt connect to " << hostIP << "::" << hostPort << "\n";

    // connect to server
    servSock = new QTcpSocket;
    loop.connect(servSock, &QTcpSocket::readyRead, this, &NetworkClient::readyRead);
    loop.connect(servSock, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &NetworkClient::server_shutdown);
    servSock->connectToHost(hostIP, hostPort);
    if (servSock->waitForConnected(1000))
        qDebug() << "[!]Connected\n";

    // send player's name
    servSock->write(QByteArray(thisPlayerName.toUtf8()));
    servSock->flush();

    emit network_manager_success(isHost);

    loop.exec();
}

void NetworkClient::read_frame_size_prefix()
{
    data.append(servSock->read(LENGTH_PREFIX_SIZE - static_cast<uint>(data.size())));
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

void NetworkClient::read_and_parse_frame()
{
    data.append(servSock->read(frameSize - data.size()));

    if (frameSize == data.size())
    {
        isPrefixRead = false;

        (this->*parse_func)();

        data.clear();
    }
}

void NetworkClient::readyRead()
{
    do
    {
        isPrefixRead ? read_and_parse_frame() : read_frame_size_prefix();
    } while (servSock->bytesAvailable());
}

int NetworkClient::parse_pregame_msg()
{
    preGameCmd msg;
    // load data from array and parse it
    if (!msg.ParseFromArray(data, data.size()))
    {
        qDebug() << "[-]Failed data:" << data << "\n";
        return 1;
    }

    switch (msg.type())
    {
        case PREGAME_NW_CMD_PLAYER_CONNECTED:
        {
            qDebug() << "New player connected: " << QString::fromStdString(msg.name()) << "\n";
            QString newName = QString::fromStdString(msg.name());

            names.push_back(newName);
            if (newName == thisPlayerName)
                thisPlayerIndex = names.size()-1;

            emit new_player_connected_sig(newName);
            break;
        }
        case PREGAME_NW_CMD_PLAYER_DISCONNECTED:
            qDebug() << "Player disconnected: " << names[static_cast<int>(msg.indexnum())] << "\n";
            emit player_disconnected_sig(names[static_cast<int>(msg.indexnum())]);
            names.remove(static_cast<int>(msg.indexnum()));
            break;
        case PREGAME_NW_CMD_START_GAME:
            qDebug() << "START GAME\n";
            parse_func = &NetworkClient::parse_ingame_msg;
            playerNum = names.size();
            emit start_multiplayer_game();
            break;
        default:
            qDebug() << "Other:" << msg.type() << "\n";
            break;
    }
    return 0;
}

#define CHECK_ARGS(type, argsNum) \
    case (type): \
        if (cmd.args_size() != argsNum) \
            errFlag = true; \
            break;

int NetworkClient::validation_check_ingame_cmd(inGameCmd &cmd) const
{
    bool errFlag = false;
    uint type = cmd.type();
    if (type <= INGAME_NW_CMD_NONE || type >= INGAME_NW_CMD_MAX)
        return 1;

    switch (type)
    {
        CHECK_ARGS(INGAME_NW_CMD_PLAYER_RECONNECTED, 0)
        CHECK_ARGS(INGAME_NW_CMD_PLAYER_DISCONNECTED, 1)
        CHECK_ARGS(INGAME_NW_CMD_NEXT_TURN, 0)
        CHECK_ARGS(INGAME_NW_CMD_MOVE_UNIT, 5)
        CHECK_ARGS(INGAME_NW_CMD_ATTACK_UNIT, 5)
        CHECK_ARGS(INGAME_NW_CMD_PLACE_UNIT, 3)
        CHECK_ARGS(INGAME_NW_CMD_REMOVE_UNIT, 2)
    }

    return errFlag ? 1 : 0;
}

#undef CHECK_ARGS

int NetworkClient::parse_ingame_msg()
{
    inGameCmd msg;
    QVector<uint> args;

    if (!msg.ParseFromArray(data, data.size()))
    {
        qDebug() << "[-]Failed to parse data:" << data << "\n";
        return 1;
    }

    if (validation_check_ingame_cmd(msg))
    {
        qDebug() << "[-]Validation check fail";
        return 1;
    }

    for (int i = 0; i < msg.args_size(); i++)
        args.push_back(msg.args(i));
    emit recv_ingame_cmd_for_execution(msg.type(), args);

    if (msg.type() == INGAME_NW_CMD_NEXT_TURN)
        next_turn();
    else if (msg.type() == INGAME_NW_CMD_PLAYER_DISCONNECTED)
    {
        int removeIndex = static_cast<int>(msg.args(0));
        lostPlayerIndexes.push_back(removeIndex);
        if (removeIndex == curPlayerIndex)
            next_turn();
    }

    return 0;
}

template<class T>
void NetworkClient::send_cmd_to_server(T &cmd)
{
    QByteArray payloadArray;
    QByteArray payloadSizeArr;
    int32_t packetSize = 0;
    QDataStream packetSizeStream(&payloadSizeArr, QIODevice::WriteOnly);

    payloadArray.resize(cmd.ByteSize());
    cmd.SerializeToArray(payloadArray.data(), payloadArray.size());

    packetSize = payloadArray.size();
    packetSizeStream << packetSize;

    servSock->write(payloadSizeArr, payloadSizeArr.size());
    servSock->write(payloadArray, payloadArray.size());
    servSock->flush();
}

#define SET_ARGS(type, argsNum) \
    case type: \
        Q_ASSERT(args.size() == argsNum); \
        for (int i = 0; i < argsNum; i++) \
            cmd.add_args(args[i]); \
        break;

void NetworkClient::send_this_player_ingame_cmd(const uint type, const QVector<uint> args)
{
    inGameCmd cmd;
    QByteArray payloadArr;

    cmd.set_type(type);
    switch (type)
    {
        SET_ARGS(INGAME_NW_CMD_PLAYER_RECONNECTED, 0)
        SET_ARGS(INGAME_NW_CMD_PLAYER_DISCONNECTED, 1)
        SET_ARGS(INGAME_NW_CMD_NEXT_TURN, 0)
        SET_ARGS(INGAME_NW_CMD_MOVE_UNIT, 5)
        SET_ARGS(INGAME_NW_CMD_ATTACK_UNIT, 5)
        SET_ARGS(INGAME_NW_CMD_PLACE_UNIT, 3)
        SET_ARGS(INGAME_NW_CMD_REMOVE_UNIT, 2)
        default:
            qDebug() << "[-]Unknown type:" << type;
            break;
    }

    send_cmd_to_server<inGameCmd>(cmd);

    if (type == INGAME_NW_CMD_NEXT_TURN)
        next_turn();
}

#undef SET_ARGS

void NetworkClient::next_turn()
{
    curPlayerIndex = (curPlayerIndex + 1) % playerNum;
    while (lostPlayerIndexes.contains(curPlayerIndex))
        curPlayerIndex = (curPlayerIndex + 1) % playerNum;
    if (curPlayerIndex == thisPlayerIndex)
        emit this_player_turn_start();
}

void NetworkClient::handle_player_loss(int loserIndex)
{
    lostPlayerIndexes.push_back(loserIndex);
}

void NetworkClient::initial_setup()
{
    connect_to_server();
}

void NetworkClient::this_player_disconnected()
{
    servSock->close();
}

void NetworkClient::server_shutdown(QAbstractSocket::SocketError sockError)
{
    servSock->close();
    emit network_error(NETWORK_ERROR_SERVER_SHUTDOWN);
}
