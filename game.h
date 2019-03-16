#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollBar>
#include <QApplication>
#include <QVBoxLayout>
#include "gamefield.h"
#include "playerlist.h"
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

class Game: public QObject
{
    Q_OBJECT

public:
    Game();
    ~Game();
    void start_hot_seat();
    void start_multiplayer();

    PlayerList *get_player_list();

    void next_turn();

    void set_state(const cur_player_state newState);
    cur_player_state get_state() const;

    int get_player_num() const;

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

    QVector<QString> playerNames;
    PlayerList* playerList;

    cur_player_state state;

    QGraphicsView* view;

    QWidget* mainWidget;

    NetworkManager* netMng;
    QThread* networkThread;

signals:
    void finished();

    void create_serv_sig(int port);
    void connect_to_serv_sig(QString host, int port);

public slots:
    void show_waiting_for_players_screen(bool isHost);
    void call_create_network_thread(QString host, unsigned short int port);
};

#endif // GAME_H
