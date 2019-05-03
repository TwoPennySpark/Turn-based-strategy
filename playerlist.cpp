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

    curPlayerIt = players.begin();
}

PlayerList::~PlayerList()
{
    for (auto player: players)
        delete player;
}

void PlayerList::next_turn()
{
    curPlayerIt++;
    if (curPlayerIt == players.end())
        curPlayerIt = players.begin();
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
    return (*curPlayerIt)->color;
}

int PlayerList::get_cur_player_money() const
{
    return (*curPlayerIt)->money;
}

int PlayerList::get_cur_player_income() const
{
    return (*curPlayerIt)->income;
}

void PlayerList::get_cur_player_name(QString &retName) const
{
    retName = (*curPlayerIt)->name;
}

void PlayerList::change_player_income(const player_color playerColor, const int change)
{
    find_player_by_color(playerColor)->income += change;
}

void PlayerList::change_cur_player_money_amount(const int change)
{
    (*curPlayerIt)->money += change;
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

    if (playerColor == get_cur_player_color())
    {
        curPlayerIt = players.erase(curPlayerIt);
        if (curPlayerIt == players.end())
            curPlayerIt = players.begin();
    }
    else
    {
        player_color curPlayerColor = (*curPlayerIt)->color;
        players.removeOne(find_player_by_color(playerColor));
        curPlayerIt = &players[players.indexOf(find_player_by_color(curPlayerColor))];
    }

    if (players.size() == 1)
        show_player_won_msg_box((*curPlayerIt)->name);
    player_lost(playerColor);
}

void PlayerList::show_player_lost_msg_box(const QString& playerName) const
{
    QMessageBox msgBox;
    msgBox.setText(QString("Player \"%1\" lost").arg(playerName));
    msgBox.addButton(QMessageBox::Close);
//    msgBox.move(view->width()/2  - msgBox.width()/2,
//                view->height()/2 - msgBox.height()/2);
    msgBox.exec();
}

void PlayerList::show_player_won_msg_box(const QString &playerName) const
{
    QMessageBox gameOverBox;
    gameOverBox.setText(QString("Player \"%1\" won!").arg(playerName));
    gameOverBox.addButton(QMessageBox::Close);
//    gameOverBox.move(view->width()/2  - gameOverBox.width()/2,
//                     view->height()/2 - gameOverBox.height()/2);
    gameOverBox.exec();

//    this->deleteLater();
}
