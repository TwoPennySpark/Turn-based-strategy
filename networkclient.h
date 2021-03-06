#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QEventLoop>
#include <QVector>
#include "networkmanager.h"

class NetworkClient: public NetworkManager
{
    Q_OBJECT

public:
    NetworkClient(QString name, QString ip, quint16 port);
    ~NetworkClient() override;

    enum {isHost = 0};
    void initial_setup() override;
    void connect_to_server();

    void readyRead();
    void read_frame_size_prefix();
    void read_and_parse_frame();
    int parse_pregame_msg();
    int parse_ingame_msg();

    template<class T>
    void send_cmd_to_server(T &cmd);

    int validation_check_pregame_cmd(preGameCmd &cmd) const;
    int validation_check_ingame_cmd(inGameCmd &cmd) const;

    void next_turn();

    void this_player_disconnected() override;
    void send_this_player_ingame_cmd(const uint type, const QVector<uint> args) override;
    void handle_player_loss(uint loserIndex) override;

private:
    QString hostIP;
    quint16 hostPort;
    QTcpSocket* servSock;

    uint thisPlayerIndex;
    QString thisPlayerName;

    using parse_arr_func = int (NetworkClient::*)();
    parse_arr_func parse_func;

public slots:
    void server_shutdown(QAbstractSocket::SocketError sockError);
};

#endif // NETWORKCLIENT_H
