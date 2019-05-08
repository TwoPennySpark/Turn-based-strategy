#include "game.h"
#include <QLineEdit>
#include <QStackedWidget>
#include <QLabel>

Game::Game()
{
    mainWidget = new QWidget;

    thisPlayerTurn = false;
    isMultiplayerGame = false;

    playerNum = 0;

    state = STATE_NONE;
}

Game::~Game()
{
    delete gameField;

    delete view;

    delete mainWidget;

    delete playerList;
}

PlayerList* Game::get_player_list() const
{
    return playerList;
}

void Game::create_players()
{
    playerNum = 4;
    if (playerNum > PLAYER_MAX)
        playerNum = PLAYER_MAX;
    playerNames.reserve(playerNum);

    QString temp;
    for (int i = 0; i < playerNum; i++)
    {
        switch (i)
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

void Game::show_main_menu()
{
    QVBoxLayout *mainMenuLayout = new QVBoxLayout;

    mainWidget->setGeometry(700, 250, 500, 500);

    QPushButton* singlePlayerButton = new QPushButton("Hot-seat");
    mainMenuLayout->addWidget(singlePlayerButton);
    connect(singlePlayerButton, &QPushButton::clicked, this, &Game::start_hot_seat);

    QPushButton* multiPlayerButton = new QPushButton("Multiplayer");
    mainMenuLayout->addWidget(multiPlayerButton);
    connect(multiPlayerButton, &QPushButton::clicked, this, &Game::show_multiplayer_settings_menu);

    QPushButton* exitButton = new QPushButton("Exit");
    mainMenuLayout->addWidget(exitButton);
    connect(exitButton, &QPushButton::clicked, [&](){this->clear_main_window(); this->deleteLater(); emit finished();});

    mainWidget->setLayout(mainMenuLayout);

    mainWidget->show();
}

void Game::start_hot_seat()
{
    clear_main_window();
    mainWidget->hide();

    view = new QGraphicsView();
//    view->showFullScreen();
    view->setFixedSize(1900, 900);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    create_players();
    thisPlayerTurn = true;
    state = STATE_BASIC;
    gameField = new GameField(view);
}

void Game::show_multiplayer_settings_menu()
{
    clear_main_window();

    QVBoxLayout *enterNameLayout = new QVBoxLayout;
    QLabel* nameLabel = new QLabel("Enter your name - it must be more than 3 characters long and should not exceed 64 characters");
    QLineEdit *nameEdit = new QLineEdit();
    nameEdit->setAlignment(Qt::AlignCenter);
    nameEdit->setText("Player");

    QPushButton* connectButton = new QPushButton("Connect");
    connect(connectButton, &QPushButton::clicked, [nameEdit, this]()
    {
        QString name = nameEdit->text();
        if (name.size() >= 4 && name.size() <= 64)
            this->show_connect_menu(name);
    });

    QPushButton* createServButton = new QPushButton("Create a server");
    connect(createServButton, &QPushButton::clicked, [nameEdit, this]()
    {
        QString name = nameEdit->text();
        if (name.size() >= 4 && name.size() <= 64)
            this->show_create_serv_menu(name);
    });

    enterNameLayout->addWidget(nameLabel);
    enterNameLayout->addWidget(nameEdit);
    enterNameLayout->addWidget(connectButton);
    enterNameLayout->addWidget(createServButton);
    mainWidget->setLayout(enterNameLayout);
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
            [this, name, ipEdit, portEdit](){this->create_network_thread(name, false, ipEdit->text(), static_cast<quint16>(portEdit->text().toInt()));});

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
            [this, portEdit, name](){this->create_network_thread(name, true, "127.0.0.1", static_cast<quint16>(portEdit->text().toInt()));});

    createServLayout->addWidget(portLabel);
    createServLayout->addWidget(portEdit);
    createServLayout->addWidget(createServButton);

    mainWidget->setLayout(createServLayout);
}

void Game::create_network_thread(QString name, bool createServer, QString host, quint16 port)
{
    createServer ? (netMng = new NetworkHost(name, port, PLAYER_MAX)) : (netMng = new NetworkClient(name, host, port));
    connect(this, &Game::network_initial_setup, netMng, &NetworkManager::initial_setup);
    connect(this, &Game::send_ingame_cmd, netMng, &NetworkManager::send_this_player_ingame_cmd);
    connect(netMng, &NetworkManager::network_manager_success, this, &Game::show_waiting_for_players_screen, Qt::BlockingQueuedConnection);
    connect(netMng, &NetworkManager::start_multiplayer_game, this, &Game::start_multiplayer);
    connect(netMng, &NetworkManager::this_player_turn_start, this, &Game::set_this_player_turn_true);
    connect(netMng, &NetworkManager::recv_ingame_cmd_for_execution, this, &Game::execute_multiplayer_ingame_cmd);
    connect(netMng, &NetworkManager::network_error, this, &Game::show_network_error_msg);

    networkThread = new QThread;
    netMng->moveToThread(networkThread);
    networkThread->start();

    emit network_initial_setup();
}

void Game::show_waiting_for_players_screen(bool isHost)
{
    clear_main_window();

    isMultiplayerGame = true;

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
        QString playerList; for (auto name: playerNames) playerList += name +"\n"; playersListLabel->setText(playerList);
    });

    QPushButton* startButton = new QPushButton(isHost ? "Start" : "Ready");

    QPushButton* disconnectButton = new QPushButton("Disconnect");
    connect(disconnectButton, &QPushButton::clicked, netMng, &NetworkManager::this_player_disconnected);
    connect(disconnectButton, &QPushButton::clicked, [this](){this->clear_main_window(); this->roll_back_to_main_menu();});

    layout->addWidget(playersReadyLabel);
    layout->addWidget(playersListLabel);
    layout->addWidget(startButton);
    layout->addWidget(disconnectButton);

    mainWidget->setLayout(layout);
}

void Game::start_multiplayer()
{
//    clear_main_window();
    mainWidget->hide();

    view = new QGraphicsView();
//    view->showFullScreen();
    view->setFixedSize(1900, 900);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    playerList = new PlayerList(playerNames);
    connect(this->playerList, &PlayerList::player_lost, netMng, &NetworkManager::handle_player_loss);
    state = STATE_BASIC;
    gameField = new GameField(view);
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
        else if (list[0] == "del")
        {
            player_color pc = playerList->get_cur_player_color();
            if (static_cast<player_color>(list[1].toInt()) == pc)
            {
                playerList->set_player_countdown(pc, true);
                playerList->set_turns_left(pc, -1);
                gameField->next_turn();
            }
            else
            {
                gameField->delete_players_items(static_cast<player_color>(list[1].toInt()));
                playerList->delete_player(static_cast<player_color>(list[1].toInt()));
            }
        }
    });

    layout->addWidget(commandLine);
    layout->addWidget(execButton);
    mainWidget->setLayout(layout);

    mainWidget->show();
}

int Game::multiplayer_ingame_cmd_validation_check(const uint type, const QVector<uint>& args) const
{
    bool errFlag = false;

    if (type <= INGAME_NW_CMD_NONE || type >= INGAME_NW_CMD_MAX)
        return 1;

    const uint gameFieldWidth  = static_cast<uint>(gameField->get_width());
    const uint gameFieldHeight = static_cast<uint>(gameField->get_height());
    switch (type)
    {
        case INGAME_NW_CMD_PLAYER_RECONNECTED:
            break;
        case INGAME_NW_CMD_PLAYER_DISCONNECTED:
            if (playerList->find_player_by_color(static_cast<player_color>(args[0])) == nullptr)
                errFlag = true;
            break;
        case INGAME_NW_CMD_NEXT_TURN:
            break;
        case INGAME_NW_CMD_MOVE_UNIT:
            if (!(args[0] < gameFieldWidth &&
                  args[1] < gameFieldHeight &&
                  args[2] < gameFieldWidth  &&
                  args[3] < gameFieldHeight))
                errFlag = true;
            break;
        case INGAME_NW_CMD_ATTACK_UNIT:
            if (!(args[0] < gameFieldWidth &&
                  args[1] < gameFieldHeight &&
                  args[2] < gameFieldWidth &&
                  args[3] < gameFieldHeight))
                errFlag = true;
            break;
        case INGAME_NW_CMD_PLACE_UNIT:
            if (!(args[0] < gameFieldWidth  &&
                  args[1] < gameFieldHeight &&
                  args[2] < UNIT_TYPE_MAX))
                errFlag = true;
            break;
        case INGAME_NW_CMD_REMOVE_UNIT:
            if (!(args[0] < gameFieldWidth &&
                  args[1] < gameFieldHeight))
                errFlag = true;
            break;
        default:
            break;
    }

    return errFlag ? 1 : 0;
}

void Game::execute_multiplayer_ingame_cmd(const uint type, const QVector<uint> args)
{
    if (multiplayer_ingame_cmd_validation_check(type, args))
        return;

    switch (type)
    {
        case INGAME_NW_CMD_PLAYER_RECONNECTED:
            qDebug() << "INGAME_NW_CMD_PLAYER_RECONNECTED";
            break;
        case INGAME_NW_CMD_PLAYER_DISCONNECTED:
        {
            qDebug() << "INGAME_NW_CMD_PLAYER_DISCONNECTED";
            player_color pc = playerList->get_cur_player_color();
            if (static_cast<player_color>(args[0]) == pc)
            {
                playerList->set_player_countdown(pc, true);
                playerList->set_turns_left(pc, -1);
                gameField->next_turn();
            }
            else
            {
                gameField->delete_players_items(static_cast<player_color>(args[0]));
                playerList->delete_player(static_cast<player_color>(args[0]));
            }
            break;
        }
        case INGAME_NW_CMD_NEXT_TURN:
            qDebug() << "INGAME_NW_CMD_NEXT_TURN";
            gameField->next_turn();
            break;
        case INGAME_NW_CMD_MOVE_UNIT:
            qDebug() << "INGAME_NW_CMD_MOVE_UNIT";
            gameField->move_unit_to_another_field(&gameField->fields[args[0]][args[1]],
                                                  &gameField->fields[args[2]][args[3]], args[4]);
            break;
        case INGAME_NW_CMD_ATTACK_UNIT:
            qDebug() << "INGAME_NW_CMD_ATTACK_UNIT";
            gameField->one_unit_attack_another(&gameField->fields[args[0]][args[1]],
                                               &gameField->fields[args[2]][args[3]], args[4]);
            break;
        case INGAME_NW_CMD_PLACE_UNIT:
            qDebug() << "INGAME_NW_CMD_PLACE_UNIT";
            gameField->place_new_unit_on_gamefield(args[0], args[1], static_cast<unit_type>(args[2]));
            break;
        case INGAME_NW_CMD_REMOVE_UNIT:
            qDebug() << "INGAME_NW_CMD_REMOVE_UNIT";
            gameField->remove_unit_from_gamefield(&gameField->fields[args[0]][args[1]]);
            break;
        default:
            qDebug() << "[-]Unknown type:" << type;
            break;
    }
}

void Game::show_network_error_msg(const int code)
{
    QMessageBox msgBox;

    switch (code)
    {
        case NETWORK_ERROR_SERVER_SHUTDOWN:
            msgBox.setText(QString("Server shutdown"));
            break;
        case NETWORK_ERROR_THIS_PLAYER_DISCONNECT:
            msgBox.setText(QString("Connection lost"));
            break;
        case NETWORK_ERROR_NAME_ALREADY_TAKEN:
            msgBox.setText(QString("This name is already taken"));
            break;
        case NETWORK_ERROR_CANT_ESTABLISH_CONNECTION_WITH_SERVER:
            msgBox.setText(QString("Can't establish connection with the server"));
            break;
        default:
            msgBox.setText(QString("Unknown error"));
            break;
    }
    msgBox.addButton(QMessageBox::Close);
    msgBox.exec();

    roll_back_to_main_menu();
}

void Game::roll_back_to_main_menu()
{
    clear_main_window();

    isMultiplayerGame = false;
    thisPlayerTurn = false;
    playerNum = 0;
    playerNames.clear();

    netMng->deleteLater();
    networkThread->quit();
    networkThread->wait();
    networkThread->deleteLater();

    if (state != STATE_NONE)
    {
        delete gameField;

        delete view;

        delete playerList;
    }
    show_main_menu();
}

bool Game::is_this_player_turn() const
{
    return thisPlayerTurn;
}

void Game::set_this_player_turn_true()
{
    thisPlayerTurn = true;
}

void Game::set_this_player_turn_false()
{
    thisPlayerTurn = false;
}

bool Game::is_multiplayer_game() const
{
    return isMultiplayerGame;
}

void Game::clear_main_window()
{
    QLayoutItem* item;

    if (mainWidget->layout() == nullptr)
        return;

    while ((item = mainWidget->layout()->takeAt(0)))
    {
        delete item->widget();
        delete item;
    }
    delete mainWidget->layout();
}
