#ifndef UNIT_H
#define UNIT_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsTextItem>
#include <QTimer>

typedef enum unit_move_direction
{
    UNIT_MOVE_DIRECTION_NONE,
    UNIT_MOVE_DIRECTION_UP,
    UNIT_MOVE_DIRECTION_DOWN,
    UNIT_MOVE_DIRECTION_LEFT,
    UNIT_MOVE_DIRECTION_RIGHT
}unit_move_direction;

typedef enum unit_possible_move_type
{
    UNIT_POSSIBLE_MOVE_TYPE_NONE,
    UNIT_POSSIBLE_MOVE_TYPE_ALLOWED,
    UNIT_POSSIBLE_MOVE_TYPE_ENEMY_IN_ATTACK_RANGE,
    UNIT_POSSIBLE_MOVE_TYPE_ENEMY_FAR_OR_ALLY,
}unit_possible_move_type;

typedef enum unit_attack_type
{
    UNIT_ATTACK_TYPE_NONE,
    UNIT_ATTACK_TYPE_MELEE,
    UNIT_ATTACK_TYPE_RANGED
}unit_attack_type;

typedef enum unit_combat_outcome
{
    UNIT_COMBAT_NONE,
    UNIT_COMBAT_ATTACKER_DESTROYED,
    UNIT_COMBAT_DEFENDER_DESTROYED
}unit_combat_outcome;

typedef enum unit_type
{
    UNIT_TYPE_NONE = -1,
    UNIT_TYPE_WARRIOR,
    UNIT_TYPE_ARCHER,
    UNIT_TYPE_KNIGHT,
    UNIT_TYPE_MAGE,
    UNIT_TYPE_WATER_ELEMENTAL,
    UNIT_TYPE_CATAPULT,
    UNIT_TYPE_DRAGON,
    UNIT_TYPE_MAX
}unit_type;

typedef enum player_color
{
    PLAYER_NONE = -1,
    PLAYER_RED,
    PLAYER_GREEN,
    PLAYER_BLUE,
    PLAYER_BLACK,
    PLAYER_MAX
}player_color;

struct field_info;

typedef struct defaultUnitStats
{
    QString name;
    QString imgPath;
    int speed;
    int strength;
    unit_attack_type attackType;
    int attackRange;
    int cost;
}defaultUnitStats;


class Unit: public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    Unit(int x, int y, unit_type t, player_color fraction_color);
    ~Unit();
    int get_coord_x();
    int get_coord_y();

    void calculate_possible_movements(QHash<QPair<int, int>, field_info> &possibleMovements);

    void depth_search_for_possible_moves(QHash<QPair<int, int>, field_info> &possibleMovements, int x, int y, int fieldsPassed, QVector<QPair<int, int> > &path);
    void width_search_for_enemies_in_attack_range(QHash<QPair<int, int>, field_info>& possibleMovements);

    void move_unit(qreal toPosCoord_x, qreal toPosCoord_y, int numOfPassedFields);
    void pave_the_way(int toCoord_x, int toCoord_y, int elapsedSpeed);
    int depth_search_for_the_shortest_path(int x, int y, int toCoord_x, int toCoord_y, int& elapsedSpeed);
    unit_move_direction determine_direction(int to_x, int to_y);

    void add_fraction_rect();
    player_color get_fraction();
    void get_fraction_name(QString &retName);

    unit_combat_outcome attack(Unit* enemy, int enemyFieldDefenseBonus); // TODO: change two args on SoleField* ?

    void reset_speed();
    int get_speed_left() const;

    bool is_active();
    void set_active();
    void set_inactive();

    unit_attack_type get_attack_type() const;
    void get_name(QString& retName) const;

    void update_displayed_health();
    void start_damage_received_animation(int damageReceived);

    static const defaultUnitStats *get_record_from_default_stats_table(unit_type type);

public slots:
    void move();
    void show_damage_received();

private:
    const qreal max_health = 100.0;

    QString name;

    bool active;

    int coord_x; // coord on a gamefield array (not position on scene coord)
    int coord_y;

    int Vx;
    int Vy;
    int curMoveDestIndex;
    unit_move_direction direction;
    QTimer moveTimer;
    QVector<QPair<int, int> > way;

    QTimer damageReceivedTimer;
    QGraphicsTextItem damageReceivedText;

    QGraphicsPixmapItem img;
    QGraphicsRectItem fractionRect;
    QGraphicsTextItem healthText;

    player_color fraction;
    void set_fraction_color();

    unit_type type;
    unit_attack_type attackType;
    int attackRange;

    int speed;
    int speedLeft;
    int health;
    qreal strength;
    qreal defense;
    int cost;
    int capacity;
};

#endif // UNIT_H
