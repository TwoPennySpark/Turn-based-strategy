#include "game.h"
#include <QLineEdit>
#include <QStackedWidget>
#include <QLabel>

//Game::Game(QThread* mainThread)
Game::Game()
{/*
    view = new QGraphicsView();
    view->setFixedSize(1000, 800);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);*/

//    connect(mainThread, &QThread::started, this, &Game::start);
//    this->moveToThread(mainThread);
    mainWidget = new QWidget;
}

Game::~Game()
{
    delete view;

    delete gameField;

    for (auto player: players)
        delete player;

    delete mainWidget;
}

void Game::start_hot_seat()
{
//    qDebug() << "MAIN THREAD: " << QThread::currentThreadId() << "\n";

    view = new QGraphicsView();
//    view->showFullScreen();
    view->setFixedSize(1900, 900);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    create_players();
    state = STATE_BASIC;
    gameField = new GameField(view);
}

void Game::start_multiplayer()
{
    clear_main_window();

    QVBoxLayout *enterNameLayout = new QVBoxLayout;
    QLabel* nameLabel = new QLabel("Enter your name");
    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setAlignment(Qt::AlignCenter);

    QPushButton* connectButton = new QPushButton("Connect");
    connect(connectButton, &QPushButton::clicked, this, &Game::show_connect_menu);
    QPushButton* createServButton = new QPushButton("Create a server");
    connect(createServButton, &QPushButton::clicked, this, &Game::show_create_serv_menu);

    enterNameLayout->addWidget(nameLabel);
    enterNameLayout->addWidget(nameEdit);
    enterNameLayout->addWidget(connectButton);
    enterNameLayout->addWidget(createServButton);
    mainWidget->setLayout(enterNameLayout);
}

void Game::next_turn()
{
    curPlayerIndex = (curPlayerIndex+1) % (playerNum);
    while (players[curPlayerIndex]->turnsBeforeLosing < 0)
        curPlayerIndex = (curPlayerIndex+1) % (playerNum);
}

void Game::create_players()
{
    mainWidget->hide();

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
        players[i]->money = 1000;
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

void Game::call_create_network_thread(QString host, unsigned short port)
{
    create_network_thread(false, host, port);
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

void Game::set_state(const cur_player_state newState)
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

void Game::change_player_income(const player_color player, const int change)
{
    players[static_cast<int>(player)]->income += change;
}

void Game::change_cur_player_money_amount(const int change)
{
    players[curPlayerIndex]->money += change;
}

bool Game::is_player_losing(const player_color player) const
{
    return players[static_cast<int>(player)]->isLosing;
}

void Game::set_player_countdown(const player_color player, bool status)
{
    players[static_cast<int>(player)]->isLosing = status;

    if (!status)
        players[static_cast<int>(player)]->turnsBeforeLosing = max_turns_before_losing;
}

void Game::decrement_countdown(const player_color player)
{
    if (players[static_cast<int>(player)]->isLosing)
        if (--players[static_cast<int>(player)]->turnsBeforeLosing < 0)
            delete_player(players[static_cast<int>(player)]->color);
}

int Game::get_turns_left(const player_color player) const
{
    return players[static_cast<int>(player)]->turnsBeforeLosing;
}

void Game::delete_player(const player_color player)
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
    QStackedWidget *stackWidget = new QStackedWidget;
    QVBoxLayout *mainMenuLayout = new QVBoxLayout;

    mainWidget->setGeometry(700, 250, 500, 500);

    QPushButton* singlePlayerButton = new QPushButton("Hot-seat");
    mainMenuLayout->addWidget(singlePlayerButton);
    connect(singlePlayerButton, &QPushButton::clicked, this, &Game::start_hot_seat);

    QPushButton* multiPlayerButton = new QPushButton("Multiplayer");
    mainMenuLayout->addWidget(multiPlayerButton);
    connect(multiPlayerButton, &QPushButton::clicked, this, &Game::start_multiplayer);

    QPushButton* exitButton = new QPushButton("Exit");
    mainMenuLayout->addWidget(exitButton);
    connect(exitButton, &QPushButton::clicked, [&](){this->deleteLater(); emit finished();});

    mainWidget->setLayout(mainMenuLayout);

//    QWidget *widget2 = new QWidget;
//    QVBoxLayout *enterNameLayout = new QVBoxLayout;
//    QLineEdit *lineEdit = new QLineEdit();
//    lineEdit->setGeometry(100, 100, 1000, 200);
//    lineEdit->setAlignment(Qt::AlignCenter);
//    QPushButton* nextButton = new QPushButton("Next");

//    enterNameLayout->addWidget(lineEdit);
//    enterNameLayout->addWidget(nextButton);
//    widget2->setLayout(enterNameLayout);

//    stackWidget->addWidget(mainWidget);
//    stackWidget->addWidget(widget2);
//    stackWidget->setCurrentIndex(0);
//    connect(nextButton, &QPushButton::clicked, [&](){stackWidget->setCurrentIndex(1);});
//    stackWidget->show();
    mainWidget->show();
}

void Game::show_connect_menu()
{
    clear_main_window();

    QVBoxLayout* connectMenuLayout = new QVBoxLayout();

    QLabel* ipLabel = new QLabel("Enter IP");
    QLineEdit *ipEdit = new QLineEdit("127.0.0.1");
    ipEdit->setAlignment(Qt::AlignCenter);

    QLabel* portLabel = new QLabel("Enter port");
    QLineEdit *portEdit = new QLineEdit("5555");
    portEdit->setAlignment(Qt::AlignCenter);

    QPushButton *connectButton = new QPushButton("Connect");
    connect(connectButton, &QPushButton::clicked,
            [this, ipEdit, portEdit](){this->create_network_thread(false, ipEdit->text(), portEdit->text().toInt());});

    connectMenuLayout->addWidget(ipLabel);
    connectMenuLayout->addWidget(ipEdit);
    connectMenuLayout->addWidget(portLabel);
    connectMenuLayout->addWidget(portEdit);
    connectMenuLayout->addWidget(connectButton);
    mainWidget->setLayout(connectMenuLayout);
}

void Game::show_create_serv_menu()
{
    clear_main_window();

    QVBoxLayout* createServLayout = new QVBoxLayout();
    QLabel* portLabel = new QLabel("Enter port");
    QLineEdit *portEdit = new QLineEdit("5555");
    portEdit->setAlignment(Qt::AlignCenter);

    QPushButton *createServButton = new QPushButton("Create a server");
    connect(createServButton, &QPushButton::clicked,
            [this, portEdit](){this->create_network_thread(true, "127.0.0.1", portEdit->text().toInt());});

    createServLayout->addWidget(portLabel);
    createServLayout->addWidget(portEdit);
    createServLayout->addWidget(createServButton);

    mainWidget->setLayout(createServLayout);
}

void Game::create_network_thread(bool createServer, QString host, int port)
{
    netMng = new NetworkManager(createServer, max_player_num);
    connect(this, &Game::create_serv_sig, netMng, &NetworkManager::create_server);
    connect(this, &Game::connect_to_serv_sig, netMng, &NetworkManager::connect_to_server);
    connect(netMng, &NetworkManager::network_manager_success, this, &Game::show_waiting_for_players_screen);

    networkThread = new QThread;
    networkThread->start();
    netMng->moveToThread(networkThread);

    emit createServer ? create_serv_sig(port) : connect_to_serv_sig(host, port);
}

void Game::show_waiting_for_players_screen(bool isHost)
{
    clear_main_window();

    QVBoxLayout* layout = new QVBoxLayout;
    QLabel* playersReadyLabel = new QLabel("Players ready:1/4");

    QPushButton* startButton;
    if (isHost)
        startButton = new QPushButton("Start");
    else
        startButton = new QPushButton("Ready");

    QPushButton* disconnectButton = new QPushButton("Disconnect");
    connect(disconnectButton, &QPushButton::clicked, netMng, &NetworkManager::this_player_disconnected);

    layout->addWidget(playersReadyLabel);
    layout->addWidget(startButton);
    layout->addWidget(disconnectButton);
    mainWidget->setLayout(layout);
}

void Game::clear_main_window()
{
    QLayoutItem* item;
    while ((item = mainWidget->layout()->takeAt(0)))
    {
        delete item->widget();
        delete item;
    }
    delete mainWidget->layout();
}
