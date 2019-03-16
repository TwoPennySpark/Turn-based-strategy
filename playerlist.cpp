#include "playerlist.h"


PlayerList::PlayerList(QVector<QString>& playerNames)
{
    playerNum = playersLeft = playerNames.size();
    for (int i = 0; i < playersLeft; i++)
    {
        players.push_back(new player);
        players[i]->color = static_cast<player_color>(i);
        players[i]->money = 1000;
        players[i]->income = 0;
        players[i]->isLosing = false;
        players[i]->turnsBeforeLosing = max_turns_before_losing;
        players[i]->name = playerNames[i];
    }

    curPlayerIndex = 0;
}

void PlayerList::next_turn()
{
   curPlayerIndex = (curPlayerIndex+1) % playersLeft;
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

void PlayerList::set_player_countdown(const player_color playerColor, bool status)
{
    find_player_by_color(playerColor)->isLosing = status;

    if (!status)
        find_player_by_color(playerColor)->turnsBeforeLosing = max_turns_before_losing;
}

void PlayerList::decrement_countdown(const player_color playerColor)
{
    player* p = find_player_by_color(playerColor);
    if (p->isLosing)
        if (--p->turnsBeforeLosing < 0)
            delete_player(p->color);
}

int PlayerList::get_turns_left(const player_color playerColor) const
{
    return find_player_by_color(playerColor)->turnsBeforeLosing;
}

void PlayerList::delete_player(const player_color playerColor)
{
    show_player_lost_msg_box(find_player_by_color(playerColor)->name);

    players.removeOne(find_player_by_color(playerColor));
    curPlayerIndex--;

    if (--playersLeft == 1)
    {
        next_turn();
        show_player_won_msg_box(players[curPlayerIndex]->name);
    }
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

void PlayerList::show_player_won_msg_box(const QString &playerName)
{
    QMessageBox gameOverBox;
    gameOverBox.setText(QString("Player \"%1\" won!").arg(playerName));
    gameOverBox.addButton(QMessageBox::Close);
//    gameOverBox.move(view->width()/2  - gameOverBox.width()/2,
//                     view->height()/2 - gameOverBox.height()/2);
    gameOverBox.exec();

//    this->deleteLater();
}
