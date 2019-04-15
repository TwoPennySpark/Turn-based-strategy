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

const int MAX_NAME_LENGTH = 32;

typedef enum pregame_network_cmd_types
{
    PREGAME_NW_CMD_PLAYER_NONE,
    PREGAME_NW_CMD_PLAYER_CONNECTED,
    PREGAME_NW_CMD_PLAYER_DISCONNECTED,
    PREGAME_NW_CMD_PLAYER_READY,
    PREGAME_NW_CMD_START_GAME
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
    INGAME_NW_CMD_REMOVE_UNIT
}ingame_network_cmd_types;

int byteArrayToInt(QByteArray& arr);

class NetworkManager: public QObject
{
    Q_OBJECT

protected:
    NetworkManager();
    const unsigned int LENGTH_PREFIX_SIZE = sizeof(int32_t);
//    int numOfConnectedPlayers;

    bool isHost;

    int curPlayerIndex;
    bool isPrefixRead;
    int frameSize;

    QByteArray data;

//    QString thisPlayerName;
    QVector<QString> playerNames;

//    virtual void read_frame_size_prefix() = 0;
//    virtual void read_and_parse_frame() = 0;

public slots:
    virtual void initial_setup() = 0;
//    virtual void readyRead() = 0;
    virtual void this_player_disconnected() = 0;

signals:
    void network_manager_success(bool isHost);
    void new_player_connected_sig(QString name);
    void player_disconnected_sig(QString name);
};

#endif // NETWORKMANAGER_H
