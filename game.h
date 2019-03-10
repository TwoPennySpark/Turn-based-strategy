#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollBar>
#include <QApplication>
#include <QVBoxLayout>
#include "gamefield.h"
#include "networkmanager.h"

#include <QThread>
#include <QObject>

typedef enum cur_player_state
{
    STATE_NONE,
    STATE_BASIC,
    STATE_UNIT_SELECTED,
    STATE_UNIT_PURCHASE,
}cur_player_state;

typedef struct player
{
    player_color color;
    QString name;
    int income;
    int money;
    int limit;

    bool isLosing;
    int turnsBeforeLosing;
}player;

class Game: public QObject
{
    Q_OBJECT

public:
    Game();
//    Game(QThread *mainThread);
    ~Game();
    void start_hot_seat();
    void start_multiplayer();

    void next_turn();

    player_color get_cur_player_color() const;
    int get_cur_player_money() const;
    int get_cur_player_income() const;
    void get_cur_player_name(QString &retName) const;

    void set_state(const cur_player_state newState);
    cur_player_state get_state() const;

    int get_player_num() const;

    void change_player_income(const player_color player, const int change);
    void change_cur_player_money_amount(const int change);

    bool is_player_losing(const player_color player) const;
    void set_player_countdown(const player_color player, bool status);
    void decrement_countdown(const player_color player);
    int get_turns_left(const player_color player) const;
    void delete_player(const player_color player);

    void show_player_lost_msg_box(const QString &playerName) const;
    void show_player_won_msg_box(const QString &playerName);

    void show_main_menu();
    void show_connect_menu();
    void show_create_serv_menu();
    void clear_main_window();

    void create_network_thread(bool createServer, QString host, int port);

    GameField *gameField;

private:
    const int max_player_num = 4;
    const int max_turns_before_losing = 1;
    int playerNum;
    int playersLeft;

    int curPlayerIndex;
    QVector<player*> players;
    void create_players();

    cur_player_state state;

    QGraphicsView *view;

    QWidget *mainWidget;

    NetworkManager* netMng;
    QThread* networkThread;

signals:
    void finished();

    void create_serv_sig(int port);
    void connect_to_serv_sig(QString host, int port);
};

#endif // GAME_H
