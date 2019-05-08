#include <QApplication>
#include "game.h"

Game* game;

//    template <typename... Args>
//    void print(Args... args){(std::cout << ... << args);}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "GUI THREAD: " << QThread::currentThreadId() << "\n";

    game = new Game();
    QObject::connect(game, &Game::finished, &a, &QApplication::quit, Qt::DirectConnection);
    game->show_main_menu();

    return a.exec();
}
