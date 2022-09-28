#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "solefield.h"
#include "markfield.h"
#include "unit.h"
#include "networkmanager.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QGraphicsView>
#include <QDebug>
#include <QBrush>
#include <QPushButton>
#include <QMessageBox>
#include <fstream>

typedef struct field_info
{
    int minElapsedSpeed;
    unit_possible_move_type moveType;
   // QVector<QPair<int, int> > way;
}field_info;

typedef struct real_estate
{
    player_color faction;
    QGraphicsRectItem *factionRect;
}real_estate;

class GameField: public QGraphicsScene
{

public:    
    GameField(QGraphicsView *view, QWidget* parent = nullptr);
    ~GameField() override;

    void keyPressEvent(QKeyEvent* event) override;

    void calculate_and_show_possible_movements(QHash<QPair<int, int>, field_info> &possibleMovements);
    void show_possible_movements(QHash<QPair<int, int>, field_info>& possibleMovements);
    void delete_possible_movements_rects();

    void place_new_unit_on_gamefield(int field_x, int field_y, unit_type type);
    void move_unit_to_another_field(SoleField &from, SoleField &to, int speedElapsed);
    void remove_unit_from_gamefield(SoleField &unitField);
    void one_unit_attack_another(SoleField &attackerField, SoleField &defenderField, int minElapsedSpeed);

    void move_view();

    void create_gamefield();
    void prepare_hud();
    void prepare_info_rect();
    void prepare_cur_player_rect();
    void prepare_unit_purchase_scene();

    void update_info_rect();
    void update_info_rect_color();
    void update_hud();

    void show_cur_player_rect();
    void show_unit_purchase_scene();
    void show_unit_info_on_purchase_scene();

    void set_new_castle_owner(const QPair<int, int> &castleCoord, const player_color newOwner);
    void check_if_player_has_any_castles_left(const player_color player);
    void set_countdown(player_color player);
    void delete_players_items(player_color playerColor);

    void send_ingame_cmd(ingame_network_cmd_types type, std::initializer_list<int> list);

    int parse_map_file();

    void next_turn();

    int get_width() const;
    int get_height() const;

    SoleField** fields;

private:
    int gameFieldWidth;
    int gameFieldHeight;

    SoleField* get_marked_field() const;
    Unit* get_marked_field_unit() const;

    MarkField mark;

    QGraphicsView *gameFieldView;

    QScrollBar *horizontalBar;
    QScrollBar *verticalBar;

    int viewPointX;
    int viewPointY;

    QGraphicsItemGroup *hud;
    QGraphicsTextItem *moneyAmountText;

    SoleField* selectedUnitField;
    QVector<QGraphicsRectItem*> posMovesRects;

    QVector<Unit*> unitsOnGamefield;
    QMap<QPair<int, int>, real_estate> castles;
    QMap<QPair<int, int>, real_estate> houses;

    QGraphicsItemGroup* infoRectGroup;
    QGraphicsTextItem *infoRectText;

    QGraphicsItemGroup* curPlayerRectGroup;
    QGraphicsTextItem *curPlayerRectText;

    QGraphicsItemGroup* unitPurchaseSceneGroup;
    int unitPurchaseSceneMarkBorder_X1;
    int unitPurchaseSceneMarkBorder_X2;
    int unitPurchaseSceneMarkBorder_Y;

    int castleSpawnCoord_X;
    int castleSpawnCoord_Y;
    unit_type selectedUnit;

    QGraphicsTextItem* nameString;
    QGraphicsTextItem* costString;
    QGraphicsTextItem* strengthString;
    QGraphicsTextItem* speedString;
    QGraphicsTextItem* attackTypeString;
    QGraphicsTextItem* attackRangeString;
    QGraphicsTextItem* descriptionString;
};

#endif // GAMEFIELD_H
