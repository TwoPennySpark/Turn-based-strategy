#include "solefield.h"
#include "unit.h"

SoleField::SoleField()
{
    unit = nullptr;
}

SoleField::~SoleField()
{
    delete img;
    if (unit != nullptr)
        delete unit;
}

Unit *SoleField::get_unit() const
{
    return unit;
}

void SoleField::set_unit(Unit* newUnit)
{
    unit = newUnit;
}

void SoleField::del_unit()
{
    if (unit != nullptr)
    {
        delete unit;
        unit = nullptr;
    }
}

terrain_type SoleField::get_terrain_type() const
{
    return terrainType;
}

int SoleField::get_speed_modificator() const
{
    return speedModificator;
}

int SoleField::get_defense_bonus() const
{
    return defenseBonus;
}

void SoleField::get_terrain_name(QString &retName) const
{
    switch (terrainType)
    {
        case TERRAIN_TYPE_FOREST:
            retName = "Forest";
            break;
        case TERRAIN_TYPE_GRASSFIELD:
            retName = "Grassfield";
            break;
        case TERRAIN_TYPE_CASTLE:
            retName = "Castle";
            break;
        default:
            retName = "default";
            break;
    }
}

void SoleField::setImg(const terrain_type fieldType)
{
    switch (fieldType)
    {
        case TERRAIN_TYPE_FOREST:
            img = new QPixmap(":/terrain/img/forest1.png");
            terrainType = TERRAIN_TYPE_FOREST;
            speedModificator = 2;
            defenseBonus = 20;
            break;
        case TERRAIN_TYPE_GRASSFIELD:
            img = new QPixmap(":/terrain/img/grass00.png");
            terrainType = TERRAIN_TYPE_GRASSFIELD;
            speedModificator = 1;
            defenseBonus = 0;
            break;
        case TERRAIN_TYPE_CASTLE:
        case TERRAIN_TYPE_START_CASTLE:
            img = new QPixmap(":/terrain/img/castle2.png");
            terrainType = TERRAIN_TYPE_CASTLE;
            speedModificator = 1;
            defenseBonus = 50;
            break;
        default:
            break;
    }
    QPixmap newImg = img->scaled(QSize(SOLE_SQUARE_FIELD_SIZE, SOLE_SQUARE_FIELD_SIZE),
                                 Qt::KeepAspectRatio);
    setPixmap(newImg);
}
