﻿#include "unit.h"
#include "game.h"

extern Game* game;

const defaultUnitStats unitStatsLookupTable[UNIT_TYPE_MAX] =
{
    [UNIT_TYPE_WARRIOR] = {"Warrior", ":/unit/img/militia.png", 4, 2, UNIT_ATTACK_TYPE_MELEE, 1, 100},
    [UNIT_TYPE_ARCHER] = {"Archer", ":/unit/img/archer.png", 4, 3, UNIT_ATTACK_TYPE_RANGED, 3, 250},
    [UNIT_TYPE_KNIGHT] = {"Knight", ":/unit/img/knight.png", 3, 6, UNIT_ATTACK_TYPE_MELEE, 1, 450},
    [UNIT_TYPE_MAGE] = {"Mage", ":/unit/img/mage.png", 4, 5, UNIT_ATTACK_TYPE_RANGED, 4, 600},
    [UNIT_TYPE_WATER_ELEMENTAL] = {"Water Elemental", ":/unit/img/elemental.png", 5, 7, UNIT_ATTACK_TYPE_MELEE, 1, 750},
    [UNIT_TYPE_CATAPULT] = {"Catapult", ":/unit/img/catapult.png", 3, 6, UNIT_ATTACK_TYPE_RANGED, 5, 800},
    [UNIT_TYPE_DRAGON] = {"Dragon", ":/unit/img/dragon2.png", 7, 10, UNIT_ATTACK_TYPE_MELEE, 1, 1000},
};

Unit::Unit(int x, int y, unit_type t, player_color faction_color): QObject(nullptr), QGraphicsItemGroup(), coord_x(x), coord_y(y), type(t), faction(faction_color)
{
    name = unitStatsLookupTable[type].name;
    QPixmap i(unitStatsLookupTable[type].imgPath);
    img.setPixmap(i.scaled(QSize(SOLE_SQUARE_FIELD_SIZE, SOLE_SQUARE_FIELD_SIZE),
                           Qt::KeepAspectRatio));
    speed = unitStatsLookupTable[type].speed;
    strength = unitStatsLookupTable[type].strength;
    attackType = unitStatsLookupTable[type].attackType;
    attackRange = unitStatsLookupTable[type].attackRange;

    active = true;
    speedLeft = speed;

    health = static_cast<int>(max_health);
    healthText.setDefaultTextColor(Qt::white);
    healthText.setScale(2);
    healthText.setPos(5, 50);

    Vx = 0;
    Vy = 0;
    curMoveDestIndex = 0;
    direction = UNIT_MOVE_DIRECTION_NONE;
    connect(&moveTimer, &QTimer::timeout, this, &Unit::move);

    damageReceivedText.setDefaultTextColor(Qt::yellow);
    damageReceivedText.setScale(2);
    connect(&damageReceivedTimer, &QTimer::timeout, this, &Unit::show_damage_received);

    addToGroup(&img);
    add_faction_rect();
    add_inactive_rect();
    addToGroup(&healthText);
    addToGroup(&damageReceivedText);

    setZValue(0.8);
    setPos(FROM_GAMEFIELD_TO_POS_COORD(coord_x), FROM_GAMEFIELD_TO_POS_COORD(coord_y));
}

Unit::~Unit()
{

}

int Unit::get_coord_x() const
{
    return coord_x;
}

int Unit::get_coord_y() const
{
    return coord_y;
}

unit_combat_outcome Unit::attack(const SoleField &defenderField)
{
    unit_combat_outcome outcome = UNIT_COMBAT_NONE;
    const qreal attackingUnitStrengthBonus = 0.1;
    const int minHealthLeftAfterCombatBothDestroyed = 10;

    Unit *enemy = defenderField.get_unit();
    int enemyFieldDefenseBonus = defenderField.get_defense_bonus();

    qreal thisUnitCombatStrength = 0.0;
    qreal thisUnitAttackBonus = 0.0;
    qreal enemyCombatStrength = 0.0;
    qreal enemyDefenseBonus = 0.0;
    qreal combatRatio = 0.0;

    int damageDealt = 0;
    int damageReceived = 0;
    int tempThisHealth = this->health;
    int tempEnemyHealth = enemy->health;

    thisUnitCombatStrength = strength * (health / max_health);
    thisUnitAttackBonus = (thisUnitCombatStrength / 100) * attackingUnitStrengthBonus;
    thisUnitCombatStrength += thisUnitAttackBonus;

    enemyCombatStrength = enemy->strength * (enemy->health / max_health);
    enemyDefenseBonus = (enemyCombatStrength / 100) * enemyFieldDefenseBonus;
    enemyCombatStrength += enemyDefenseBonus;

    combatRatio = thisUnitCombatStrength / enemyCombatStrength;

    qDebug() << "thisUnitCombatStrength:" << thisUnitCombatStrength << "\n";
    qDebug() << "enemyCombatStrength:" << enemyCombatStrength << "\n";
    qDebug() << "combatRatio:" << combatRatio << "\n";

    // y = 6.66*x*x + 3.33*x + 20
    // y - damage dealt, x - combat ratio
    damageDealt = static_cast<int>(combatRatio*combatRatio * 6.66 + combatRatio * 3.33 + 20);
    if (this->attackType == UNIT_ATTACK_TYPE_MELEE)
    {
        combatRatio = 1 / combatRatio;
        damageReceived = static_cast<int>(combatRatio*combatRatio * 6.66 + combatRatio * 3.33 + 20);
        this->health -= damageReceived;
    }
    enemy->health -= damageDealt;

    qDebug() << "DAMAGE DEALT:" << damageDealt << " RECIEVED:" << damageReceived << "\n";
    qDebug() << "HEALTH REMAINING: " << health << "\n";
    qDebug() << "ENEMY HEALTH REMAINING: " << enemy->health << "\n";

    if (enemy->health <= 0)
    {
        outcome = UNIT_COMBAT_DEFENDER_DESTROYED;
        if (this->health <= 0)
        {// if both unit got destroyed in combat, the one that dealth more damage
         // survives with 10 health or less(if it had less health before)
            if (damageDealt >= damageReceived)
            {
                if (this->health + damageReceived < minHealthLeftAfterCombatBothDestroyed)
                    this->health = this->health + damageReceived;
                else
                    this->health = minHealthLeftAfterCombatBothDestroyed;
                outcome = UNIT_COMBAT_DEFENDER_DESTROYED;
            }
            else
            {
                if (enemy->health + damageDealt < minHealthLeftAfterCombatBothDestroyed)
                    enemy->health = enemy->health + damageDealt;
                else
                    enemy->health = minHealthLeftAfterCombatBothDestroyed;
                outcome = UNIT_COMBAT_ATTACKER_DESTROYED;
            }
        }
    }
    else if (health <= 0)
        outcome = UNIT_COMBAT_ATTACKER_DESTROYED;
    else
        outcome = UNIT_COMBAT_NONE;

    set_inactive();

    this-> update_displayed_health();
    enemy->update_displayed_health();

    damageReceived = tempThisHealth  - this-> health;
    damageDealt    = tempEnemyHealth - enemy->health;

    this-> start_damage_received_animation(damageReceived);
    enemy->start_damage_received_animation(damageDealt);

    return outcome;
}

void Unit::add_faction_rect()
{
    factionRect.setRect(0, 0, 16, 16);
    set_faction_color();
    factionRect.setPos(x(), y());
    factionRect.setZValue(1);
    addToGroup(&factionRect);
}

player_color Unit::get_faction() const
{
    return faction;
}

void Unit::get_faction_name(QString& retName)
{
    switch (faction)
    {
        case PLAYER_BLUE:
            retName = "blue";
            break;
        case PLAYER_GREEN:
            retName = "green";
            break;
        case PLAYER_RED:
            retName = "red";
            break;
        case PLAYER_BLACK:
            retName = "black";
            break;
        default:
            retName = "default";
            break;
    }
}

void Unit::set_faction_color()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    switch (faction)
    {
        case PLAYER_BLUE:
            brush.setColor(Qt::blue);
            break;
        case PLAYER_RED:
            brush.setColor(Qt::red);
            break;
        case PLAYER_GREEN:
            brush.setColor(Qt::green);
            break;
        case PLAYER_BLACK:
            brush.setColor(Qt::black);
            break;
        default:
            brush.setColor(Qt::white);
            break;
    }
    factionRect.setBrush(brush);
}

int Unit::move_unit(qreal destPosCoord_x, qreal destPosCoord_y, int elapsedSpeed)
{
    int newCoord_x = FROM_POS_TO_GAMEFIELD_COORD(destPosCoord_x);
    int newCoord_y = FROM_POS_TO_GAMEFIELD_COORD(destPosCoord_y);

    if (elapsedSpeed > speedLeft)
        elapsedSpeed = speedLeft;

    if (pave_the_way(newCoord_x, newCoord_y, elapsedSpeed))
        return 1;

    speedLeft -= elapsedSpeed;

    if (speedLeft <= 0)
        set_inactive();

//    setPos(toPosCoord_x, toPosCoord_y);
    coord_x = newCoord_x;
    coord_y = newCoord_y;

    return 0;
}

int Unit::pave_the_way(int toCoord_x, int toCoord_y, int elapsedSpeed)
{
    // fill "way" vector with coord of fields unit
    // moves throught towards the distanation point
    depth_search_for_the_shortest_path(coord_x, coord_y, toCoord_x, toCoord_y, elapsedSpeed);

    // if the path is not found
    if (way.size() == 0)
        return 1;

    // if move animation has already started
    if (moveTimer.isActive())
        return 0;

    // remove current coord
    way.pop_front();

    // start move animation
    moveTimer.start(10);

    return 0;
}

void depth_seach_insert_neighbours(QVector<QPair<int, int>>& neighbours, const int x, const int y, const QVector<QPair<int, int>>& path)
{
    if (x+1 < game->gameField->get_width() && !path.contains(qMakePair(x+1, y)))
        neighbours.push_back(qMakePair(x+1, y));
    if (x-1 >= 0 && !path.contains(qMakePair(x-1, y)))
        neighbours.push_back(qMakePair(x-1, y));
    if (y+1 < game->gameField->get_height() && !path.contains(qMakePair(x, y+1)))
        neighbours.push_back(qMakePair(x, y+1));
    if (y-1 >= 0 && !path.contains(qMakePair(x, y-1)))
        neighbours.push_back(qMakePair(x, y-1));
}

int Unit::depth_search_for_the_shortest_path(int x, int y, int toCoord_x, int toCoord_y, int& elapsedSpeed)
{
    int wastedSpeed = game->gameField->fields[x][y].get_speed_modificator();
    QVector<QPair<int, int>> neighbours;
    neighbours.reserve(4);

    way.push_back(qMakePair(x, y));

    if (toCoord_x == x && toCoord_y == y)
        return 1;

    if (game->gameField->fields[x][y].get_unit() != nullptr &&
        game->gameField->fields[x][y].get_unit() != this) // TODO: get rid of this check
    {
        way.pop_back();
        return 0;
    }

    if (coord_x != x || coord_y != y)
        elapsedSpeed -= wastedSpeed;
    if (elapsedSpeed <= 0)
    {
        elapsedSpeed += wastedSpeed;
        way.pop_back();
        return 0;
    }

    depth_seach_insert_neighbours(neighbours, x, y, way);
    for (auto it = neighbours.cbegin(); it != neighbours.cend(); it++)
        if (depth_search_for_the_shortest_path(it->first, it->second, toCoord_x, toCoord_y, elapsedSpeed))
            return 1;

    elapsedSpeed += wastedSpeed;
    way.pop_back();

    return 0;
}

#define REPLACE_OR_NEW_INSERT(x, y, type) \
    do { \
    if (possibleMovements.contains(qMakePair(x, y))) { \
        if (possibleMovements.value(qMakePair(x, y)).minElapsedSpeed > fieldsPassed) \
            possibleMovements.insert(qMakePair(x, y), \
                (field_info){fieldsPassed, type}); \
    } \
    else { \
        possibleMovements.insert(qMakePair(x, y), \
            (field_info){fieldsPassed, type}); \
    } \
    } while(0)

void Unit::depth_search_for_possible_moves(QHash<QPair<int, int>, field_info>& possibleMovements, int x, int y, int fieldsPassed, QVector<QPair<int, int>>& path)
{
    QVector<QPair<int , int>> neighbours;
    neighbours.reserve(4);

    if (x != coord_x || y != coord_y)
        fieldsPassed += game->gameField->fields[x][y].get_speed_modificator();

    if (game->gameField->fields[x][y].get_unit() != nullptr
            && game->gameField->fields[x][y].get_unit() != this)
    { // check whether it's friend or foe
        if (game->gameField->fields[x][y].get_unit()->get_faction() != this->get_faction())
        { // if it's foe see if it's on one of the neighbour fields
            if (abs(x - coord_x) + abs(y - coord_y) == 1)
                REPLACE_OR_NEW_INSERT(x, y, UNIT_POSSIBLE_MOVE_TYPE_ENEMY_IN_ATTACK_RANGE);
        }
        fieldsPassed -= game->gameField->fields[x][y].get_speed_modificator();
        return;
    }
    else
        REPLACE_OR_NEW_INSERT(x, y, UNIT_POSSIBLE_MOVE_TYPE_ALLOWED);

    path.push_back(qMakePair(x, y));

    if (fieldsPassed < speedLeft)
    {
        depth_seach_insert_neighbours(neighbours, x, y, path);

        for (int i = 0; i < neighbours.size(); i++)
            depth_search_for_possible_moves(possibleMovements, neighbours[i].first, neighbours[i].second, fieldsPassed, path);
    }

    path.pop_back();
    fieldsPassed -= game->gameField->fields[x][y].get_speed_modificator();

    return;
}
#undef REPLACE_OR_NEW_INSERT

void width_search_insert_neighbours(QQueue<QPair<int, int>>& checkQueue, QHash<QPair<int, int>, int>& alreadyChecked, const int x, const int y)
{
    if (x+1 < game->gameField->get_width() && !alreadyChecked.value(qMakePair(x+1, y)))
    {
        checkQueue.enqueue(qMakePair(x+1, y));
        alreadyChecked.insert(qMakePair(x+1, y), 1);
    }
    if (x-1 >= 0 && !alreadyChecked.value(qMakePair(x-1, y)))
    {
        checkQueue.enqueue(qMakePair(x-1, y));
        alreadyChecked.insert(qMakePair(x-1, y), 1);
    }
    if (y+1 < game->gameField->get_height() && !alreadyChecked.value(qMakePair(x, y+1)))
    {
        checkQueue.enqueue(qMakePair(x, y+1));
        alreadyChecked.insert(qMakePair(x, y+1), 1);
    }
    if (y-1 >= 0 && !alreadyChecked.value(qMakePair(x, y-1)))
    {
        checkQueue.enqueue(qMakePair(x, y-1));
        alreadyChecked.insert(qMakePair(x, y-1), 1);
    }
}

void Unit::width_search_for_enemies_in_attack_range(QHash<QPair<int, int>, field_info>& possibleMovements)
{
    QQueue<QPair<int, int>> checkQueue;
    QHash<QPair<int, int>, int> alreadyChecked;

    width_search_insert_neighbours(checkQueue, alreadyChecked, coord_x, coord_y);

    while (!checkQueue.empty())
    {
        auto pair = checkQueue.dequeue();
        if (abs(coord_x - pair.first) + abs(coord_y - pair.second) <= this->attackRange)
        {
            if (game->gameField->fields[pair.first][pair.second].get_unit() != nullptr &&
                game->gameField->fields[pair.first][pair.second].get_unit()->get_faction() != this->get_faction())
                possibleMovements.insert(qMakePair(pair.first, pair.second),
                                        (field_info){0, UNIT_POSSIBLE_MOVE_TYPE_ENEMY_IN_ATTACK_RANGE});

            width_search_insert_neighbours(checkQueue, alreadyChecked, pair.first, pair.second);
        }
    }
}

void Unit::calculate_possible_movements(QHash<QPair<int, int>, field_info>& possibleMovements)
{
    int elapsedSpeed = 0;
    QVector<QPair<int, int> > path;
    path.reserve(speedLeft);

    possibleMovements.insert(qMakePair(coord_x, coord_y), (field_info){0, UNIT_POSSIBLE_MOVE_TYPE_ENEMY_FAR_OR_ALLY});
    depth_search_for_possible_moves(possibleMovements, coord_x, coord_y, elapsedSpeed, path);
    possibleMovements.remove(qMakePair(coord_x, coord_y));

    if (attackType == UNIT_ATTACK_TYPE_RANGED)
        width_search_for_enemies_in_attack_range(possibleMovements);
}

void Unit::reset_speed()
{
    speedLeft = speed;
}

int Unit::get_speed_left() const
{
    return speedLeft;
}

bool Unit::is_active() const
{
    return active;
}

void Unit::set_active()
{
    active = true;
    inactiveRect.hide();
}

void Unit::set_inactive()
{
    active = false;
    inactiveRect.show();
}

void Unit::add_inactive_rect()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(185, 185, 185, 200));
    inactiveRect.setRect(x(), y(), SOLE_SQUARE_FIELD_SIZE, SOLE_SQUARE_FIELD_SIZE);
    inactiveRect.setBrush(brush);
    inactiveRect.setPen(Qt::NoPen);
    factionRect.setZValue(0.8);
    inactiveRect.hide();

    addToGroup(&inactiveRect);
}

unit_attack_type Unit::get_attack_type() const
{
    return attackType;
}

void Unit::update_displayed_health()
{
    if (health != static_cast<int>(max_health))
    {
        healthText.setPlainText(QString("%1").arg(QString::number(health)));
        healthText.show();
    }
    else
        healthText.hide();
}

void Unit::get_name(QString& retName) const
{
    retName = name;
}

const defaultUnitStats* Unit::get_record_from_default_stats_table(const unit_type type)
{
    return &unitStatsLookupTable[type];
}

unit_move_direction Unit::determine_direction(const int to_x, const int to_y)
{
    int from_x = FROM_POS_TO_GAMEFIELD_COORD(this->x());
    int from_y = FROM_POS_TO_GAMEFIELD_COORD(this->y());

    int coordXDiff = to_x - from_x;
    int coordYDiff = to_y - from_y;

    unit_move_direction direction = UNIT_MOVE_DIRECTION_NONE;

    if (coordXDiff < 0)
    {
        Vx = -10;
        Vy = 0;
        direction = UNIT_MOVE_DIRECTION_LEFT;
    }
    else if (coordXDiff > 0)
    {
        Vx = 10;
        Vy = 0;
        direction = UNIT_MOVE_DIRECTION_RIGHT;
    }
    else if (coordYDiff < 0)
    {
        Vx = 0;
        Vy = -10;
        direction = UNIT_MOVE_DIRECTION_UP;
    }
    else
    {
        Vx = 0;
        Vy = 10;
        direction = UNIT_MOVE_DIRECTION_DOWN;
    }

    return direction;
}

void Unit::move()
{
    bool destinationReached = false;

    if (direction == UNIT_MOVE_DIRECTION_NONE)
        direction = determine_direction(way[curMoveDestIndex].first, way[curMoveDestIndex].second);

    switch (direction)
    {
        case UNIT_MOVE_DIRECTION_UP:
            if (this->y() <= static_cast<qreal>(FROM_GAMEFIELD_TO_POS_COORD(way[curMoveDestIndex].second)))
                destinationReached = true;
            break;
        case UNIT_MOVE_DIRECTION_DOWN:
            if (this->y() >= static_cast<qreal>(FROM_GAMEFIELD_TO_POS_COORD(way[curMoveDestIndex].second)))
                destinationReached = true;
            break;
        case UNIT_MOVE_DIRECTION_LEFT:
            if (this->x() <= static_cast<qreal>(FROM_GAMEFIELD_TO_POS_COORD(way[curMoveDestIndex].first)))
                destinationReached = true;
            break;
        case UNIT_MOVE_DIRECTION_RIGHT:
            if (this->x() >= static_cast<qreal>(FROM_GAMEFIELD_TO_POS_COORD(way[curMoveDestIndex].first)))
                destinationReached = true;
            break;
        default:
            break;
    }

    if (destinationReached)
    {
        if (++curMoveDestIndex == way.size())
        {
            moveTimer.stop();
            direction = UNIT_MOVE_DIRECTION_NONE;

            setPos(FROM_GAMEFIELD_TO_POS_COORD(way[curMoveDestIndex-1].first),
                   FROM_GAMEFIELD_TO_POS_COORD(way[curMoveDestIndex-1].second));
            way.clear();

            curMoveDestIndex = 0;
            return;
        }

        direction = determine_direction(way[curMoveDestIndex].first, way[curMoveDestIndex].second);
    }

    moveBy(Vx, Vy);
}

void Unit::start_damage_received_animation(const int damageReceived)
{
//    qDebug() << "DMG REC:"<<damageReceived<<endl;
    if (damageReceived)
    {
        damageReceivedText.setPlainText(QString("-%1").arg(QString::number(damageReceived)));
        damageReceivedText.setPos(30, 0);
        damageReceivedText.show();

        damageReceivedTimer.start(50);
    }
}

void Unit::show_damage_received()
{
    damageReceivedText.moveBy(0, 5);
    if (damageReceivedText.y() >= this->boundingRect().height() -
                                  damageReceivedText.boundingRect().height()*2)
    {
        damageReceivedTimer.stop();
        damageReceivedText.hide();
    }
}
