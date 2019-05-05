#include "networkmanager.h"

NetworkManager::NetworkManager()
{
    qDebug() << "nm";
    frameSize = 0;
    isPrefixRead = false;

    curPlayerIndex = 0;
}

NetworkManager::~NetworkManager()
{
    qDebug() << "dnm";
}

int byteArrayToInt(QByteArray& arr)
{
    int ret = 0;

    QDataStream stream(&arr, QIODevice::ReadWrite);
    stream >> ret;

    return ret;
}

