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

//    QObject::connect(game, &Game::finished, func, Qt::DirectConnection);
    QObject::connect(game, &Game::finished, &a, &QApplication::quit, Qt::DirectConnection);
//    game = new Game(mainThread);

//    game->moveToThread(mainThread);
//    mainThread->start();

   // game->start();
    game->show_main_menu();

    return a.exec();
}
