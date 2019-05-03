#ifndef NETWORKHOST_H
#define NETWORKHOST_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QEventLoop>
#include <QObject>
#include <QThread>
#include <QDataStream>
#include "networkmanager.h"

class NetworkHost: public NetworkManager
{
    Q_OBJECT

public:
    NetworkHost(QString name, quint16 hostPort, int numOfPlayers);
    ~NetworkHost();

    void initial_setup();
    void create_serv();
    void send_list_of_names_to_new_player(QTcpSocket *newPlayerSock);
    void broadcast_player_connect(const QString &name);
    void broadcast_player_disconnect(const unsigned int index);
    void broadcast_start_message();

    template <class T>
    void broadcast_cmd(const T& cmd) const;
    void broadcast_byte_array(QByteArray &payloadArray) const;
    void prepend_length_prefix(QByteArray& payloadArr) const;
    void forward_incoming_cmd_to_other_players(const QByteArray &payloadArray) const;
    void send_this_player_ingame_cmd(const uint type, const QVector<uint> args);

    void readyRead();
    void read_frame_size_prefix();
    void read_and_parse_frame();
    int parse_ingame_msg();

    void next_turn();

    void this_player_disconnected();
    void handle_player_loss(int loserIndex);

private:
    quint16 port;
    QTcpServer* listenSock;

//    QVector<QString> names;
    QVector<QTcpSocket*> playersSockets;

    typedef int (NetworkHost::*parse_arr_func)();
    parse_arr_func parse_func;

public slots:
    void handle_pregame_disconnect(QAbstractSocket::SocketError sockError);
    void handle_ingame_disconnect(QAbstractSocket::SocketError sockError);
};

#endif // NETWORKHOST_H
