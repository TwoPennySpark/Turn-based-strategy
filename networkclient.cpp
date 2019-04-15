#include "networkclient.h"

NetworkClient::NetworkClient(QString name, QString ip, quint16 port): thisPlayerName(name), hostIP(ip), hostPort(port)
{
    isHost = false;
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
            qDebug() << "New player connected: " << QString::fromStdString(msg.name()) << "\n";
            names.push_back(QString::fromStdString(msg.name()));
            emit new_player_connected_sig(QString::fromStdString(msg.name()));
            break;
        case PREGAME_NW_CMD_PLAYER_DISCONNECTED:
            qDebug() << "Player disconnected: " << names[static_cast<int>(msg.indexnum())] << "\n";
            emit player_disconnected_sig(names[static_cast<int>(msg.indexnum())]);
            names.remove(static_cast<int>(msg.indexnum()));
            break;
        case PREGAME_NW_CMD_START_GAME:
            qDebug() << "START GAME\n";
            parse_func = &NetworkClient::parse_ingame_msg;
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
            break;
        case INGAME_NW_CMD_NEXT_TURN:
            qDebug() << "INGAME_NW_CMD_NEXT_TURN";
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
            qDebug() << "DEFAULT";
            break;
    }
    return 0;
}

void NetworkClient::initial_setup()
{
    connect_to_server();
}

void NetworkClient::this_player_disconnected()
{
    servSock->close();
}
