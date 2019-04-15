#include "networkmanager.h"

NetworkManager::NetworkManager()
{

}

int byteArrayToInt(QByteArray& arr)
{
    int ret = 0;

    QDataStream stream(&arr, QIODevice::ReadWrite);
    stream >> ret;

    return ret;
}

