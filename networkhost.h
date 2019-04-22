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
    void create_serv();
    void send_list_of_names_to_new_player(QTcpSocket* newPlayerSock);
    void broadcast_player_connect(char *name);
    void broadcast_player_disconnect(unsigned int index);
    void broadcast_start_message();
    void broadcast_msg(QByteArray& payloadArray);

    void serialize_pregame_msg(preGameCmd& msg, QByteArray &payloadArr);
    void serialize_ingame_msg(inGameCmd& msg, QByteArray &payloadArr);

    void readyRead();
    void initial_setup();
    void this_player_disconnected();

    void read_frame_size_prefix();
    void read_and_parse_frame();

    void get_and_send_ingame_cmd(ingame_network_cmd_types type, QVector<uint>& args);
private:
    quint16 port;
    QTcpServer* server;

    QVector<QString> names;
    QVector<QTcpSocket*> socket;

    typedef int (NetworkHost::*parse_arr_func)();
    parse_arr_func parse_func;

public slots:
    void handle_disconnect(QAbstractSocket::SocketError sockError);

//signals:
//    void network_manager_success();
//    void new_player_connected_sig(QString name);
//    void player_disconnected_sig(QString name);
};

#endif // NETWORKHOST_H
