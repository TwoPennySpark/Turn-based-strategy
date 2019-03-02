#ifndef GAMEFIELD_H
#define GAMEFIELD_H
#include "solefield.h"
#include "markfield.h"
#include "unit.h"
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
#include <iostream>

const qreal gameFieldStart_X = 0;
const qreal gameFieldStart_Y = 0;

const int FIELD_NUM_X = 20;
const int FIELD_NUM_Y = 15;

typedef struct field_info
{
    int minElapsedSpeed;
    unit_possible_move_type moveType;
   // QVector<QPair<int, int> > way;
}field_info;

typedef struct real_estate
{
    player_color fraction;
    QGraphicsRectItem *fractionRect;
}real_estate;

class GameField: public QGraphicsScene
{

public:    
    GameField(QGraphicsView *view, QWidget* parent = nullptr);
    ~GameField() override;

    void keyPressEvent(QKeyEvent* event) override;

    void calculate_and_show_possible_movements(QHash<QPair<int, int>, field_info> &possibleMovements);
    void show_possible_movements(QHash<QPair<int, int>, field_info>& possibleMovements);
    void delete_possible_movements_rect();

    void place_new_unit_on_gamefield(int field_x, int field_y, unit_type type);
    void move_unit_to_another_field(SoleField* from, SoleField* to, int speedElapsed);
    void remove_unit_from_gamefield(SoleField* unitField);

    void update_info_rect();
    void update_info_rect_color();
    void update_cur_player_rect();
    void update_hud();

    void move_view();

    void create_gamefield();
    void prepare_hud();
    void prepare_info_rect();
    void prepare_cur_player_rect();
    void prepare_unit_purchase_scene();

    void show_cur_player_rect();
    void show_unit_purchase_scene();
    void show_unit_info_on_purchase_scene();

    void set_new_castle_owner(QPair<int, int> castleCoord, player_color newOwner);
    void check_if_player_has_any_castles_left(player_color player);
    void set_countdown(player_color player);

    void parse_map_file();

    void next_turn();

//    SoleField fields[FIELD_NUM_X][FIELD_NUM_Y];
    SoleField** fields;

private:
    int gameFieldWidth;
    int gameFieldHeight;

    SoleField* get_marked_field();
    Unit* get_marked_field_unit();

    MarkField mark;

    QGraphicsView *gameFieldView;

    QScrollBar *horizontalBar;
    QScrollBar *verticalBar;

    int viewPointX;
    int viewPointY;

    QGraphicsItemGroup *hud;
    QGraphicsTextItem *moneyAmountText;
    QGraphicsTextItem *turnsLeftBeforeLosingText;

    SoleField* selectedUnitField;
    QVector<QGraphicsRectItem*> posMovesRects;

    QVector<Unit*> unitsOnGamefield;
    QHash<QPair<int, int>, real_estate> castles;
    QHash<QPair<int, int>, real_estate> houses;

//    QGraphicsRectItem* infoRect;
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
