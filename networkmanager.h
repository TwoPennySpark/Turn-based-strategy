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

const int MAX_NAME_LENGTH = 32;

typedef enum pregame_network_commands
{
    PREGAME_NW_CMD_PLAYER_NONE = -1,
    PREGAME_NW_CMD_PLAYER_CONNECTED,
    PREGAME_NW_CMD_PLAYER_DISCONNECTED,
    PREGAME_NW_CMD_PLAYER_READY,
    PREGAME_NW_CMD_START_GAME
}pregame_network_commands;

class NetworkManager: public QObject
{
    Q_OBJECT

public:
    NetworkManager(QString name, bool createServer, int playerNum);

private:
    const int maxNumOfPlayers;
    int numOfConnectedPlayers;

    bool isHost;

    QString thisPlayerName;
    QTcpServer* server;
    QVector<QTcpSocket*> socket;
    QVector<QString> playerNames;

    void wait_for_players_connections();
    void parse_first_server_message(QByteArray &data);
    void broadcast_player_connected(char* name);
//    void broadcast_player_disconnected();

public slots:
    void create_server(unsigned short port);
    void connect_to_server(QString host, unsigned short port);
    void player_disconnected_from_this_host(void);
    void player_disconnected_from_this_host(QAbstractSocket::SocketError sockError);
    void this_player_disconnected();

signals:
    void network_manager_success(bool isHost);
    void new_player_connected_sig(char *name);
    void player_disconnected_sig(QString name);
};

#endif // NETWORKMANAGER_H
