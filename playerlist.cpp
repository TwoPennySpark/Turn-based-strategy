#include "playerlist.h"

PlayerList::PlayerList(QVector<QString>& playerNames)
{
    playerNum = playerNames.size();
    for (int i = 0; i < playerNum; i++)
    {
        players.push_back(new player);
        players[i]->color = static_cast<player_color>(i);
        players[i]->money = 100000;
        players[i]->income = 0;
        players[i]->isLosing = false;
        players[i]->turnsBeforeLosing = max_turns_before_losing;
        players[i]->name = playerNames[i];
    }

    curPlayerIndex = 0;
}

PlayerList::~PlayerList()
{
    for (auto player: players)
        delete player;
}

void PlayerList::next_turn()
{
    curPlayerIndex = (curPlayerIndex + 1) % playerNum;
    while (lostPlayerIndexes.contains(curPlayerIndex))
        curPlayerIndex = (curPlayerIndex + 1) % playerNum;
}

player *PlayerList::find_player_by_color(const player_color playerColor) const
{
    for (auto player: players)
        if (player->color == playerColor)
            return player;

    return nullptr;
}

player_color PlayerList::get_cur_player_color() const
{
    return players[curPlayerIndex]->color;
}

int PlayerList::get_cur_player_money() const
{
    return players[curPlayerIndex]->money;
}

int PlayerList::get_cur_player_income() const
{
    return players[curPlayerIndex]->income;
}

void PlayerList::get_cur_player_name(QString &retName) const
{
    retName = players[curPlayerIndex]->name;
}

void PlayerList::change_player_income(const player_color playerColor, const int change)
{
    find_player_by_color(playerColor)->income += change;
}

void PlayerList::change_cur_player_money_amount(const int change)
{
    players[curPlayerIndex]->money += change;
}

bool PlayerList::is_player_losing(const player_color playerColor) const
{
    return find_player_by_color(playerColor)->isLosing;
}

void PlayerList::set_player_countdown(const player_color playerColor, const bool status)
{
    find_player_by_color(playerColor)->isLosing = status;

    if (!status)
        find_player_by_color(playerColor)->turnsBeforeLosing = max_turns_before_losing;
}

int PlayerList::decrement_countdown(const player_color playerColor)
{
    player* p = find_player_by_color(playerColor);
    if (p->isLosing)
        if (--p->turnsBeforeLosing < 0)
            return 1;

    return 0;
}

int PlayerList::get_turns_left(const player_color playerColor) const
{
    return find_player_by_color(playerColor)->turnsBeforeLosing;
}

void PlayerList::set_turns_left(const player_color playerColor, const int turnsLeft)
{
    find_player_by_color(playerColor)->turnsBeforeLosing = turnsLeft;
}

void PlayerList::delete_player(const player_color playerColor)
{
    show_player_lost_msg_box(find_player_by_color(playerColor)->name);

    uint loserIndex = static_cast<uint>(players.indexOf(find_player_by_color(playerColor)));
    lostPlayerIndexes.push_back(loserIndex);

    if (playerColor == get_cur_player_color())
        next_turn();

    emit player_lost(playerColor);
    if (lostPlayerIndexes.size() == playerNum - 1)
        show_player_won_msg_box(players[curPlayerIndex]->name);
}

void PlayerList::show_player_lost_msg_box(const QString& playerName) const
{
    QMessageBox msgBox;
    msgBox.setText(QString("Player \"%1\" lost").arg(playerName));
    msgBox.addButton(QMessageBox::Close);
    msgBox.exec();
}

void PlayerList::show_player_won_msg_box(const QString &playerName) const
{
    QMessageBox gameOverBox;
    gameOverBox.setText(QString("Player \"%1\" won!").arg(playerName));
    gameOverBox.addButton(QMessageBox::Close);
    gameOverBox.exec();

    emit game_over();
}
