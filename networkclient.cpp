#include "networkclient.h"

NetworkClient::NetworkClient(QString name, QString ip, quint16 port): thisPlayerName(name), hostIP(ip), hostPort(port)
{
    isHost = false;
    thisPlayerIndex = -1;

    isPrefixRead = false;
    frameSize = 0;
    parse_func = &NetworkClient::parse_pregame_msg;
}

void NetworkClient::connect_to_server()
{
    QEventLoop loop;

    qDebug() << "Attempt connect to " << hostIP << "::" << hostPort << "\n";

    // connect to server
    servSock = new QTcpSocket;
    loop.connect(servSock, &QTcpSocket::readyRead, this, &NetworkClient::readyRead);
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
                thisPlayerIndex = names.size();
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
            break;
        default:
            qDebug() << "Other:" << msg.type() << "\n";
            break;
    }
    return 0;
}

int NetworkClient::parse_ingame_msg()
{
    inGameCmd msg;

    if (!msg.ParseFromArray(data, data.size()))
    {
        qDebug() << "[-]Failed to parse data:" << data << "\n";
        return 1;
    }

    switch (msg.type())
    {
        case INGAME_NW_CMD_PLAYER_RECONNECTED:
            qDebug() << "INGAME_NW_CMD_PLAYER_RECONNECTED";
            break;
        case INGAME_NW_CMD_PLAYER_DISCONNECTED:
            qDebug() << "INGAME_NW_CMD_PLAYER_DISCONNECTED";
            playerNum--;
            break;
        case INGAME_NW_CMD_NEXT_TURN:
            qDebug() << "INGAME_NW_CMD_NEXT_TURN";
            curPlayerIndex = (curPlayerIndex+1) % playerNum;
            if (curPlayerIndex == thisPlayerIndex)
                emit this_player_turn();
            break;
        case INGAME_NW_CMD_MOVE_UNIT:
            qDebug() << "INGAME_NW_CMD_MOVE_UNIT";
            break;
        case INGAME_NW_CMD_ATTACK_UNIT:
            qDebug() << "INGAME_NW_CMD_ATTACK_UNIT";
            break;
        case INGAME_NW_CMD_PLACE_UNIT:
            qDebug() << "INGAME_NW_CMD_PLACE_UNIT";
            break;
        case INGAME_NW_CMD_REMOVE_UNIT:
            qDebug() << "INGAME_NW_CMD_REMOVE_UNIT";
            break;      
        default:
            qDebug() << "[-]Unknown type:" << msg.type();
            break;
    }
    return 0;
}

#define SET_ARGS(type, argsNum) \
    case type: \
        Q_ASSERT(args.size() == argsNum); \
        for (int i = 0; i < argsNum; i++) \
            cmd.set_args(i, args[i]); \
        break;

void NetworkClient::create_and_send_ingame_cmd(ingame_network_cmd_types type, QVector<uint> args)
{
    inGameCmd cmd;
    QByteArray cmdArray;

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

    serialize_ingame_cmd(cmd, cmdArray);
    send_cmd_to_server(cmdArray);
}
#undef SET_ARGS

void NetworkClient::serialize_ingame_cmd(inGameCmd &msg, QByteArray& payloadArr)
{
    payloadArr.resize(msg.ByteSize());
    msg.SerializeToArray(payloadArr.data(), payloadArr.size());
}

void NetworkClient::send_cmd_to_server(QByteArray &payloadArray)
{
    QByteArray payloadSizeArr;
    int32_t packetSize = 0;
    QDataStream packetSizeStream(&payloadSizeArr, QIODevice::WriteOnly);

    packetSize = payloadArray.size();
    packetSizeStream << packetSize;

    servSock->write(payloadSizeArr, payloadSizeArr.size());
    servSock->write(payloadArray, payloadArray.size());
    servSock->flush();
}

void NetworkClient::initial_setup()
{
    connect_to_server();
}

void NetworkClient::this_player_disconnected()
{
    servSock->close();
}

void NetworkClient::get_and_send_ingame_cmd(ingame_network_cmd_types type, QVector<uint> &args)
{
    inGameCmd cmd;
    QByteArray payloadArr;

    cmd.set_type(type);
    for (int i = 0; i < args.size(); i++)
        cmd.add_args(args[i]);

    serialize_ingame_cmd(cmd, payloadArr);
    send_cmd_to_server(payloadArr);
}
