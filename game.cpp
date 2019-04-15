#include "game.h"
#include <QLineEdit>
#include <QStackedWidget>
#include <QLabel>

Game::Game()
{
    mainWidget = new QWidget;

    playerNum = 0;
}

Game::~Game()
{
    delete view;

    delete gameField;

    delete mainWidget;

    delete playerList;
}

PlayerList* Game::get_player_list() const
{
    return playerList;
}

void Game::next_turn()
{
    playerList->next_turn();
}

void Game::create_players()
{
    mainWidget->hide();

    playerNum = 4;
    if (playerNum > PLAYER_MAX)
        playerNum = PLAYER_MAX;
    playerNames.reserve(playerNum);

    QString temp;
    for (int i = 0; i < playerNum; i++)
    {
        switch (static_cast<player_color>(i))
        {
            case PLAYER_RED:
                temp = "Red";
                break;
            case PLAYER_GREEN:
                temp = "Green";
                break;
            case PLAYER_BLUE:
                temp = "Blue";
                break;
            case PLAYER_BLACK:
                temp = "Black";
                break;
            default:
                break;
        }
        playerNames.push_back(temp);
    }
    playerList = new PlayerList(playerNames);
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

void Game::start_hot_seat()
{
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
    nameEdit->setText("Player");

    QPushButton* connectButton = new QPushButton("Connect");
    connect(connectButton, &QPushButton::clicked, [nameEdit, this](){this->show_connect_menu(nameEdit->text());});
    QPushButton* createServButton = new QPushButton("Create a server");
    connect(createServButton, &QPushButton::clicked, [nameEdit, this](){this->show_create_serv_menu(nameEdit->text());});

    enterNameLayout->addWidget(nameLabel);
    enterNameLayout->addWidget(nameEdit);
    enterNameLayout->addWidget(connectButton);
    enterNameLayout->addWidget(createServButton);
    mainWidget->setLayout(enterNameLayout);
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

void Game::show_connect_menu(QString name)
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
            [this, name, ipEdit, portEdit](){this->create_network_thread(name, false, ipEdit->text(), portEdit->text().toInt());});

    connectMenuLayout->addWidget(ipLabel);
    connectMenuLayout->addWidget(ipEdit);
    connectMenuLayout->addWidget(portLabel);
    connectMenuLayout->addWidget(portEdit);
    connectMenuLayout->addWidget(connectButton);
    mainWidget->setLayout(connectMenuLayout);
}

void Game::show_create_serv_menu(QString name)
{
    clear_main_window();

    QVBoxLayout* createServLayout = new QVBoxLayout();
    QLabel* portLabel = new QLabel("Enter port");
    QLineEdit *portEdit = new QLineEdit("5555");
    portEdit->setAlignment(Qt::AlignCenter);

    QPushButton *createServButton = new QPushButton("Create a server");
    connect(createServButton, &QPushButton::clicked,
            [this, portEdit, name](){this->create_network_thread(name, true, "127.0.0.1", portEdit->text().toInt());});

    createServLayout->addWidget(portLabel);
    createServLayout->addWidget(portEdit);
    createServLayout->addWidget(createServButton);

    mainWidget->setLayout(createServLayout);
}

void Game::create_network_thread(QString name, bool createServer, QString host, quint16 port)
{
    createServer ? (netMng = new NetworkHost(name, port, PLAYER_MAX)) : (netMng = new NetworkClient(name, host, port));
    connect(this, &Game::network_initial_setup, netMng, &NetworkManager::initial_setup);
    connect(netMng, &NetworkManager::network_manager_success, this, &Game::show_waiting_for_players_screen, Qt::BlockingQueuedConnection);

    networkThread = new QThread;
    networkThread->start();
    netMng->moveToThread(networkThread);

    emit network_initial_setup();
}

void Game::show_waiting_for_players_screen(bool isHost)
{
    clear_main_window();

//    playerNum++;

    QVBoxLayout* layout = new QVBoxLayout;
    QLabel* playersReadyLabel = new QLabel("Players ready:1/4");
    QLabel* playersListLabel = new QLabel("");

    connect(netMng, &NetworkManager::new_player_connected_sig, [playersReadyLabel, playersListLabel, this](QString newName)
    {
        playerNum++; playerNames.push_back(newName); playersListLabel->text().clear();
        playersReadyLabel->setText(QString("Players ready:%1/%2").arg(QString::number(playerNum), QString::number(PLAYER_MAX)));
        QString playerList; for (auto name: playerNames) playerList += name +"\n"; playersListLabel->setText(playerList);
    });

    connect(netMng, &NetworkManager::player_disconnected_sig, [playersReadyLabel, playersListLabel, this](QString newName)
    {
        playerNum--; playerNames.removeOne(newName); playersListLabel->text().clear();
        playersReadyLabel->setText(QString("Players ready:%1/%2").arg(QString::number(playerNum), QString::number(PLAYER_MAX)));
        QString playerList; for (auto name: playerNames) {qDebug() << name ;playerList += name +"\n";} playersListLabel->setText(playerList);
    });

    QPushButton* startButton = new QPushButton(isHost ? "Start" : "Ready");

    QPushButton* disconnectButton = new QPushButton("Disconnect");
    connect(disconnectButton, &QPushButton::clicked, netMng, &NetworkManager::this_player_disconnected);
    connect(disconnectButton, &QPushButton::clicked, [this](){this->clear_main_window(); this->show_main_menu();});

    layout->addWidget(playersReadyLabel);
    layout->addWidget(playersListLabel);
    layout->addWidget(startButton);
    layout->addWidget(disconnectButton);

    mainWidget->setLayout(layout);
}

void Game::show_console()
{
    clear_main_window();

    QVBoxLayout* layout = new QVBoxLayout;
    QLineEdit* commandLine = new QLineEdit();
    commandLine->setAlignment(Qt::AlignCenter);
    QPushButton *execButton = new QPushButton("Exec");

    connect(execButton, &QPushButton::clicked, [commandLine, this](){
        QString commandLineText = commandLine->text();
        QStringList list = commandLineText.split(QRegExp("\\s+"));

        for (auto token: list)
            qDebug() << token;
        commandLine->clear();

        if (list[0] == "next")
            gameField->next_turn();
        else if (list[0] == "move")
            gameField->move_unit_to_another_field(&gameField->fields[list[1].toInt()][list[2].toInt()],
                                                  &gameField->fields[list[3].toInt()][list[4].toInt()], list[5].toInt());
        else if (list[0] == "attack")
            gameField->one_unit_attack_another(&gameField->fields[list[1].toInt()][list[2].toInt()],
                                               &gameField->fields[list[3].toInt()][list[4].toInt()], list[5].toInt());
        else if (list[0] == "place")
            gameField->place_new_unit_on_gamefield(list[1].toInt(), list[2].toInt(), UNIT_TYPE_DRAGON);
        else if (list[0] == "remove")
            gameField->remove_unit_from_gamefield(&gameField->fields[list[1].toInt()][list[2].toInt()]);
        else if (commandLineText.contains("del"))
        {
            player_color pc = playerList->get_cur_player_color();
            gameField->delete_players_items(static_cast<player_color>(list[1].toInt()));
            playerList->delete_player(static_cast<player_color>(list[1].toInt()));
            if (static_cast<player_color>(list[1].toInt()) == pc)
                gameField->next_turn();
        }
    });

    layout->addWidget(commandLine);
    layout->addWidget(execButton);
    mainWidget->setLayout(layout);

    mainWidget->show();
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
