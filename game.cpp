#include "game.h"

//Game::Game(QThread* mainThread)
Game::Game()
{/*
    view = new QGraphicsView();
    view->setFixedSize(1000, 800);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);*/

//    connect(mainThread, &QThread::started, this, &Game::start);
//    this->moveToThread(mainThread);
}

Game::~Game()
{
    delete view;

    delete gameField;

    for (auto player: players)
        delete player;

    delete mainMenu;
    delete mainMenuLayout;
}

void Game::start()
{
    qDebug() << "MAIN THREAD: " << pthread_self() << "\n";

    view = new QGraphicsView();
    view->setFixedSize(1800, 900);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    create_players();
    state = STATE_BASIC;
    gameField = new GameField(view);
}

void Game::next_turn()
{
    curPlayerIndex = (curPlayerIndex+1) % (playerNum);
    while (players[curPlayerIndex]->turnsBeforeLosing < 0)
        curPlayerIndex = (curPlayerIndex+1) % (playerNum);
}

void Game::create_players()
{
    mainMenu->hide();

    playerNum = 4;
    if (playerNum > max_player_num)
        playerNum = max_player_num;
    players.reserve(playerNum);

    playersLeft = playerNum;

    for (int i = 0; i < playerNum; i++)
    {
//        players[i] = new player;
        players.push_back(new player);
        players[i]->color = static_cast<player_color>(i);
        players[i]->money = 10000;
        players[i]->income = 0;
        players[i]->isLosing = false;
        players[i]->turnsBeforeLosing = max_turns_before_losing;

        switch (players[i]->color)
        {
            case PLAYER_RED:
                players[i]->name = "Red";
                break;
            case PLAYER_GREEN:
                players[i]->name = "Green";
                break;
            case PLAYER_BLUE:
                players[i]->name = "Blue";
                break;
            case PLAYER_BLACK:
                players[i]->name = "Black";
                break;
            default:
                break;
        }
    }
    curPlayerIndex = 0;
}

player_color Game::get_cur_player_color() const
{
    return players[curPlayerIndex]->color;
}

int Game::get_cur_player_money() const
{
    return players[curPlayerIndex]->money;
}

int Game::get_cur_player_income() const
{
    return players[curPlayerIndex]->income;
}

void Game::get_cur_player_name(QString &retName) const
{
    retName = players[curPlayerIndex]->name;
}

void Game::set_state(cur_player_state newState)
{
    state = newState;
}

cur_player_state Game::get_state() const
{
    return state;
}

int Game::get_player_num() const
{
    return playerNum;
}

void Game::change_player_income(player_color player, int change)
{
    players[static_cast<int>(player)]->income += change;
}

void Game::change_cur_player_money_amount(int change)
{
    players[curPlayerIndex]->money += change;
}

bool Game::is_player_losing(player_color player) const
{
    return players[static_cast<int>(player)]->isLosing;
}

void Game::set_player_countdown(player_color player, bool status)
{
    players[static_cast<int>(player)]->isLosing = status;

    if (!status)
        players[static_cast<int>(player)]->turnsBeforeLosing = max_turns_before_losing;
}

void Game::decrement_countdown(player_color player)
{
    if (players[static_cast<int>(player)]->isLosing)
        if (--players[static_cast<int>(player)]->turnsBeforeLosing < 0)
            delete_player(players[static_cast<int>(player)]->color);
}

int Game::get_turns_left(player_color player) const
{
    return players[static_cast<int>(player)]->turnsBeforeLosing;
}

void Game::delete_player(player_color player)
{
    show_player_lost_msg_box(players[static_cast<int>(player)]->name);

    playersLeft--;
    if (playersLeft == 1)
    {
        next_turn();
        show_player_won_msg_box(players[curPlayerIndex]->name);
    }
}

void Game::show_player_lost_msg_box(const QString& playerName) const
{
    QMessageBox msgBox;
    msgBox.setText(QString("Player \"%1\" lost").arg(playerName));
    msgBox.addButton(QMessageBox::Close);
        msgBox.setFixedSize(400, 200);
    msgBox.move(view->width()/2  - msgBox.width()/2,
                view->height()/2 - msgBox.height()/2);
    msgBox.exec();
}

void Game::show_player_won_msg_box(const QString &playerName)
{
    QMessageBox gameOverBox;
    gameOverBox.setText(QString("Player \"%1\" won!").arg(playerName));
    gameOverBox.addButton(QMessageBox::Close);
    gameOverBox.move(view->width()/2  - gameOverBox.width()/2,
                view->height()/2 - gameOverBox.height()/2);
    gameOverBox.exec();

    this->deleteLater();

    emit finished();
}

void Game::show_main_menu()
{
    mainMenu = new QWidget;

    singlePlayerButton = new QPushButton("Single Player");
    connect(singlePlayerButton, &QPushButton::clicked, this, &Game::start);
    multiPlayerButton = new QPushButton("Multiplayer");

    mainMenuLayout = new QVBoxLayout;
    mainMenuLayout->addWidget(singlePlayerButton);
    mainMenuLayout->addWidget(multiPlayerButton);

    mainMenu->setGeometry(0, 0, 500, 500);
    mainMenu->setLayout(mainMenuLayout);
    mainMenu->show();
}

