#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollBar>
#include <QApplication>
#include <QThread>
#include <QVBoxLayout>
#include "gamefield.h"
#include "playerlist.h"
#include "networkmanager.h"
#include "networkclient.h"
#include "networkhost.h"

typedef enum cur_player_state
{
    STATE_NONE,
    STATE_BASIC,
    STATE_UNIT_SELECTED,
    STATE_UNIT_PURCHASE,
}cur_player_state;

class Game: public QObject
{
    Q_OBJECT

public:
    Game();
    ~Game();
    void start_hot_seat();
    void show_multiplayer_settings_menu();
    void start_multiplayer();

    PlayerList *get_player_list() const;

    void set_state(const cur_player_state newState);
    cur_player_state get_state() const;

    int get_player_num() const;

    void show_main_menu();
    void show_connect_menu(QString name);
    void show_create_serv_menu(QString name);
    void clear_main_window();

    void create_network_thread(QString name, bool createServer, QString host, quint16 port);

    void show_console();

    bool is_this_player_turn() const;
    bool is_multiplayer_game() const;
    void set_this_player_turn_false();

    int multiplayer_ingame_cmd_validation_check(const uint type, const QVector<uint> &args) const;

    GameField *gameField;

private:
    int playerNum;

    void create_players();
    QVector<QString> playerNames;
    PlayerList* playerList;

    cur_player_state state;

    QGraphicsView* view;

    QWidget* mainWidget;

    bool isMultiplayerGame;
    bool thisPlayerTurn;
    NetworkManager* netMng;
    QThread* networkThread;

signals:
    void finished();

    void network_initial_setup();
    void send_ingame_cmd(const uint type, const QVector<uint> args);
    void player_lost(const int playerIndex);

public slots:
    void show_waiting_for_players_screen(bool isHost);
    void set_this_player_turn_true();
    void execute_multiplayer_ingame_cmd(const uint type, const QVector<uint> args);
    void show_network_error_msg(const int code);
    void roll_back_to_main_menu();
};

#endif // GAME_H
