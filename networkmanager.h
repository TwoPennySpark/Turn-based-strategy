#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QString>
#include <QtNetwork/QTcpServer>
#include <QDebug>

class NetworkManager: public QObject
{
    Q_OBJECT
public:
    NetworkManager();

public slots:
    void create_server(int port);
    void connect_to_server(QString host, int port);
};

#endif // NETWORKMANAGER_H
