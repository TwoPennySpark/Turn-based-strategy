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
    void connect_to_server();

    void readyRead();
    void read_frame_size_prefix();
    void read_and_parse_frame();
    int parse_pregame_msg();
    int parse_ingame_msg();

    void initial_setup();
    void this_player_disconnected();

//public slots:

private:
    QString hostIP;
    quint16 hostPort;
    QTcpSocket* servSock;

//    int frameSize;
//    bool isPrefixRead;
//    QByteArray data;

    QString thisPlayerName;
    QVector<QString> names;

    typedef int (NetworkClient::*parse_arr_func)();
    parse_arr_func parse_func;
};

#endif // NETWORKCLIENT_H
