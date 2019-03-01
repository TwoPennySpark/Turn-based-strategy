#include <QApplication>
#include "game.h"
#include <pthread.h>
#include <QThread>

Game* game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QThread* mainThread = new QThread;

    qDebug() << "GUI THREAD: " << pthread_self() << "\n";
    game = new Game();

//    game = new Game(mainThread);

//    game->moveToThread(mainThread);
//    mainThread->start();

    game->start();

    return a.exec();
}
