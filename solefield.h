#ifndef SOLEFIELD_H
#define SOLEFIELD_H

#include <QGraphicsPixmapItem>

typedef enum real_estate_income
{
    INCOME_CASTLE = 100
}real_estate_income;

typedef enum terrain_type
{
    TERRAIN_TYPE_NONE,
    TERRAIN_TYPE_FOREST,
    TERRAIN_TYPE_GRASSFIELD,
    TERRAIN_TYPE_CASTLE,
    TERRAIN_TYPE_ROAD,
    TERRAIN_TYPE_HILL,
    TERRAIN_TYPE_VILLAGE,
    TERRAIN_TYPE_WATER
}terrain_type;

class Unit;

const int SOLE_SQUARE_FIELD_SIZE = 100;

#define FROM_POS_TO_GAMEFIELD_COORD(coord) static_cast<int>((coord) / SOLE_SQUARE_FIELD_SIZE)
#define FROM_GAMEFIELD_TO_POS_COORD(coord) static_cast<qreal>((coord) * SOLE_SQUARE_FIELD_SIZE)

class SoleField: public QGraphicsPixmapItem
{
public:
    SoleField();
    ~SoleField();

    Unit* get_unit() const;
    void set_unit(Unit *newUnit);
    void del_unit();

    terrain_type get_terrain_type() const;
    int get_speed_modificator() const;
    int get_defense_bonus() const;
    void get_terrain_name(QString& retName) const;

    void setImg(const terrain_type randNum);
private:
    Unit* unit;
    QPixmap* img;

    terrain_type terrainType;
    int speedModificator;
    int defenseBonus;
};

#endif // SOLEFIELD_H
