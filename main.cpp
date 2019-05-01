#include <QApplication>
#include "game.h"

Game* game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QVector<int>b{0,1,2,3,4};
//    QVector<int>::iterator curIt = b.begin();

//    curIt += 2;
//    qDebug() << *curIt;
//    b.remove(2);
//    if (curIt == b.end())
//        qDebug() << "END";
//    else
//        qDebug() << *curIt;
//    return 0;

    qDebug() << "GUI THREAD: " << QThread::currentThreadId() << "\n";

    game = new Game();
    QObject::connect(game, &Game::finished, &a, &QApplication::quit, Qt::DirectConnection);
    game->show_main_menu();

    return a.exec();
}
