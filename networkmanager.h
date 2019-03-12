#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QString>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QEventLoop>
#include <QVector>
#include <QThread>

const int MAX_NAME_LENGTH = 32;

class NetworkManager: public QObject
{
    Q_OBJECT

public:
    NetworkManager(bool createServer, int playerNum);

private:
    const int maxNumOfPlayers;
    int numOfConnectedPlayers;

    bool isHost;

    QTcpServer* server;
    QVector<QTcpSocket*>socket;

    void wait_for_players_connections();

public slots:
    void create_server(unsigned short port);
    void connect_to_server(QString host, unsigned short port);
    void player_disconnected_from_this_host(void);
    void player_disconnected_from_this_host(QAbstractSocket::SocketError sockError);
    void this_player_disconnected();

signals:
    void network_manager_success(bool isHost);
};

#endif // NETWORKMANAGER_H
