#include "networkmanager.h"
#include <pthread.h>

NetworkManager::NetworkManager()
{

}

void NetworkManager::create_server(int port)
{
    qDebug() << "<" << pthread_self() << ">" << "Serv created: " << port << "\n";
}

void NetworkManager::connect_to_server(QString host, int port)
{
    qDebug() << "<" << pthread_self() << ">" << "Connect to: " << host << "::" << port << "\n";
}
