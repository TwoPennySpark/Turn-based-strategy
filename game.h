#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollBar>
#include <QApplication>
#include "gamefield.h"

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
    void start();

    void next_turn();

    player_color get_cur_player_color() const;
    int get_cur_player_money() const;
    int get_cur_player_income() const;
    void get_cur_player_name(QString &retName) const;

    void set_state(cur_player_state newState);
    cur_player_state get_state() const;

    int get_player_num() const;

    void change_player_income(player_color player, int change);
    void change_cur_player_money_amount(int change);

    bool is_player_losing(player_color player);
    void set_player_countdown(player_color player, bool status);
    void decrement_countdown(player_color player);
    int get_turns_left(player_color player);
    void delete_player(player_color player);

    void show_player_lost_msg_box(const QString &playerName);
    void show_player_won_msg_box(const QString &playerName);

    GameField *gameField;

private:
    const int max_player_num = 4;
    const int max_turns_before_losing = 1;
    int playerNum;
    int playersLeft;

    int curPlayerIndex;
//    player *curPlayer;
    QVector<player*> players;
    void create_players();
int *a;
    cur_player_state state;

    QGraphicsView *view;
};

#endif // GAME_H
