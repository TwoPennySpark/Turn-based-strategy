#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QString>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QEventLoop>
#include <QVector>
#include <QThread>
#include <sstream>
#include <QDataStream>
#include "ntwrkCmd.pb.h"

const uint MAX_NAME_LENGTH = 32;
const uint LENGTH_PREFIX_SIZE = sizeof(int32_t);

typedef enum pregame_network_cmd_types
{
    PREGAME_NW_CMD_NONE,
    PREGAME_NW_CMD_PLAYER_CONNECTED,
    PREGAME_NW_CMD_PLAYER_DISCONNECTED,
    PREGAME_NW_CMD_PLAYER_READY,
    PREGAME_NW_CMD_START_GAME,
    PREGAME_NW_CMD_MAX
}pregame_network_cmd_types;

typedef enum ingame_network_cmd_types
{
    INGAME_NW_CMD_NONE,
    INGAME_NW_CMD_PLAYER_RECONNECTED,
    INGAME_NW_CMD_PLAYER_DISCONNECTED,
    INGAME_NW_CMD_NEXT_TURN,
    INGAME_NW_CMD_MOVE_UNIT,
    INGAME_NW_CMD_ATTACK_UNIT,
    INGAME_NW_CMD_PLACE_UNIT,
    INGAME_NW_CMD_REMOVE_UNIT,
    INGAME_NW_CMD_MAX
}ingame_network_cmd_types;

typedef enum network_error
{
    NETWORK_ERROR_UNKNOWN,
    NETWORK_ERROR_THIS_PLAYER_DISCONNECT,
    NETWORK_ERROR_SERVER_SHUTDOWN
}network_error;

int byteArrayToInt(QByteArray& arr);

class NetworkManager: public QObject
{
    Q_OBJECT

protected:
    NetworkManager();

    bool isHost;
    int playerNum;

    int curPlayerIndex;
    bool isPrefixRead;
    int frameSize;

    QByteArray data;

    QVector<QString> playerNames;

//    virtual void read_frame_size_prefix() = 0;
//    virtual void read_and_parse_frame() = 0;

public slots:
    virtual void initial_setup() = 0;
//    virtual void readyRead() = 0;
    virtual void this_player_disconnected() = 0;
    virtual void send_this_player_ingame_cmd(const uint type, const QVector<uint> args) = 0;
signals:
    void this_player_turn_start();
    void network_manager_success(bool isHost);
    void new_player_connected_sig(QString name);
    void player_disconnected_sig(QString name);
    void start_multiplayer_game();
    void recv_ingame_cmd_for_execution(uint type, QVector<uint> args);
    void network_error(int errorCode);
};

#endif // NETWORKMANAGER_H
