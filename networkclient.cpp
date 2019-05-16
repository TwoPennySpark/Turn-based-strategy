#include "networkclient.h"

NetworkClient::NetworkClient(QString name, QString ip, quint16 port): hostIP(ip), hostPort(port), thisPlayerName(name)
{
    thisPlayerIndex = 0;

    isPrefixRead = false;
    frameSize = 0;
    parse_func = &NetworkClient::parse_pregame_msg;
}

NetworkClient::~NetworkClient()
{
    delete servSock;
}

void NetworkClient::connect_to_server()
{
    qDebug() << "Attempt connect to " << hostIP << "::" << hostPort << "\n";

    // connect to server
    servSock = new QTcpSocket;
    connect(servSock, &QTcpSocket::readyRead, this, &NetworkClient::readyRead);
    servSock->connectToHost(hostIP, hostPort);
    if (!servSock->waitForConnected(3000))
    {
        qDebug() << "[-]Attempt failed";
        emit network_error(NETWORK_ERROR_CANT_ESTABLISH_CONNECTION_WITH_SERVER);
        return;
    }
    connect(servSock, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &NetworkClient::server_shutdown);

    // send player's name
    servSock->write(QByteArray(thisPlayerName.toUtf8()));

    emit network_manager_success(isHost);
}

void NetworkClient::read_frame_size_prefix()
{
    data.append(servSock->read(LENGTH_PREFIX_SIZE - static_cast<uint>(data.size())));

    if (static_cast<uint>(data.size()) == LENGTH_PREFIX_SIZE)
    {
        isPrefixRead = true;
        frameSize = byteArrayToInt(data);
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

int NetworkClient::validation_check_pregame_cmd(preGameCmd &cmd) const
{
    bool errFlag = false;
    uint type = cmd.type();
    if (type <= PREGAME_NW_CMD_NONE || type >= PREGAME_NW_CMD_MAX)
        return 1;

    switch (type)
    {
        case PREGAME_NW_CMD_PLAYER_CONNECTED:
            if (cmd.name().empty())
                errFlag = true;
            break;
        case PREGAME_NW_CMD_PLAYER_DISCONNECTED:
            if (cmd.indexnum() >= static_cast<uint>(names.size()))
                errFlag = true;
            break;
        default:
            break;
    }

    return errFlag ? 1 : 0;
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

    if (validation_check_pregame_cmd(msg))
    {
        qDebug() << "[-]Validation check fail";
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
                thisPlayerIndex = static_cast<uint>(names.size()-1);

            emit new_player_connected(newName);
            break;
        }
        case PREGAME_NW_CMD_PLAYER_DISCONNECTED:
            qDebug() << "Player disconnected: " << names[static_cast<int>(msg.indexnum())] << "\n";
            emit player_disconnected(names[static_cast<int>(msg.indexnum())]);
            names.remove(static_cast<int>(msg.indexnum()));
            break;
        case PREGAME_NW_CMD_PLAYER_REQ_CHANGE_NAME:
            qDebug() << "PREGAME_NW_CMD_PLAYER_REQ_CHANGE_NAME\n";
            emit network_error(NETWORK_ERROR_NAME_ALREADY_TAKEN);
            break;
        case PREGAME_NW_CMD_START_GAME:
            qDebug() << "START GAME\n";
            parse_func = &NetworkClient::parse_ingame_msg;
            playerNum = static_cast<uint>(names.size());
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
        uint removeIndex = msg.args(0);
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

void NetworkClient::handle_player_loss(uint loserIndex)
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
