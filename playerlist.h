#ifndef PLAYERLIST_H
#define PLAYERLIST_H

#include <QMessageBox>
#include "unit.h"

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

class PlayerList: public QObject
{
    Q_OBJECT

public:
    PlayerList(QVector<QString>& playerNames);
    ~PlayerList();

    void next_turn();

    player* find_player_by_color(const player_color playerColor) const;

    player_color get_cur_player_color() const;
    int get_cur_player_money() const;
    int get_cur_player_income() const;
    void get_cur_player_name(QString &retName) const;

    void change_player_income(const player_color playerColor, const int change);
    void change_cur_player_money_amount(const int change);

    bool is_player_losing(const player_color playerColor) const;
    void set_player_countdown(const player_color playerColor, const bool status);
    int get_turns_left(const player_color playerColor) const;
    void set_turns_left(const player_color playerColor, const int turnsLeft);
    int decrement_countdown(const player_color playerColor);
    void delete_player(const player_color playerColor);

    void show_player_lost_msg_box(const QString &playerName) const;
    void show_player_won_msg_box(const QString &playerName) const;

private:
    const int max_turns_before_losing = 1;

    uint playerNum;
    uint curPlayerIndex = 0;
    QVector<player*> players;
    QVector<uint>lostPlayerIndexes;

signals:
    void player_lost(int playerIndex);
    void game_over() const;
};

#endif // PLAYERLIST_H
