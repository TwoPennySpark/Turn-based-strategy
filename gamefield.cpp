#include "gamefield.h"
#include "game.h"

#include <QHash>
#include <QPair>
#include <QVector>

extern Game* game;

GameField::GameField(QGraphicsView *view, QWidget *parent): QGraphicsScene(parent)
{
    this->setFocus();
    this->setSceneRect(0, 0, 5000, 2500);
    view->setScene(this);

    viewPointX = 0;
    viewPointY = 0;
    gameFieldView = view;

    addItem(&mark);

    prepare_hud();
    prepare_info_rect();
    prepare_unit_purchase_scene();
    prepare_cur_player_rect();

    create_gamefield();

    view->show();

    horizontalBar = view->horizontalScrollBar();
    verticalBar   = view->verticalScrollBar();
    horizontalBar->setValue(viewPointX);
    verticalBar->setValue(viewPointY);

    // tests
        place_new_unit_on_gamefield(0, 0, UNIT_TYPE_DRAGON);
        place_new_unit_on_gamefield(5, 0, UNIT_TYPE_DRAGON);
//    place_new_unit_on_gamefield(1, 2, UNIT_TYPE_ARCHER);
//    place_new_unit_on_gamefield(2, 2, UNIT_TYPE_MAGE);
//    place_new_unit_on_gamefield(3, 8, UNIT_TYPE_WARRIOR);
//    place_new_unit_on_gamefield(5, 6, UNIT_TYPE_ARCHER);
//    place_new_unit_on_gamefield(6, 4, UNIT_TYPE_KNIGHT);
//    place_new_unit_on_gamefield(7, 5, UNIT_TYPE_CATAPULT);
//    place_new_unit_on_gamefield(9, 9, UNIT_TYPE_WATER_ELEMENTAL);
//    game->next_turn();
//    place_new_unit_on_gamefield(3, 3, UNIT_TYPE_ARCHER);
//    place_new_unit_on_gamefield(4, 4, UNIT_TYPE_WARRIOR);
//    place_new_unit_on_gamefield(7, 8, UNIT_TYPE_CATAPULT);
//    place_new_unit_on_gamefield(8, 8, UNIT_TYPE_KNIGHT);
//    place_new_unit_on_gamefield(13, 5, UNIT_TYPE_DRAGON);

    update_info_rect();
    update_info_rect_color();
}

GameField::~GameField()
{
    for (QGraphicsRectItem* rect: posMovesRects)
        delete rect;

    for (auto& el: castles)
        delete el.fractionRect;

    for (int i = 0; i < gameFieldWidth; i++)
        delete[] fields[i];
    delete[] fields;

    QList<QGraphicsItem*> list = unitPurchaseSceneGroup->childItems();
    for (auto el: list)
        if (!dynamic_cast<QGraphicsTextItem*>(el))
            delete el;

    delete[] nameString;
    delete[] costString;
    delete[] strengthString;
    delete[] speedString;
    delete[] attackTypeString;
    delete[] attackRangeString;
    delete[] descriptionString;

    delete unitPurchaseSceneGroup;
}

void GameField::keyPressEvent(QKeyEvent *event)
{
    static QHash<QPair<int, int>, field_info> possibleMovements;
    game->show_player_won_msg_box("name");
    switch (event->key())
    {
        case (Qt::Key_W):
        case (Qt::Key_Up):
            if (game->get_state() != STATE_UNIT_PURCHASE)
            {
                if (mark.get_coord_y()-1 >= 0)
                {
                    mark.move(mark.get_coord_x(), mark.get_coord_y()-1);

                    if (get_marked_field()->y() < viewPointY)
                    {
                        viewPointY -= SOLE_SQUARE_FIELD_SIZE;
                        move_view();
                    }
                    update_info_rect();
                }
            }
            break;
        case (Qt::Key_S):
        case (Qt::Key_Down):
            if (game->get_state() != STATE_UNIT_PURCHASE)
            {
                if (mark.get_coord_y()+1 < FIELD_NUM_Y)
                {
                    mark.move(mark.get_coord_x(), mark.get_coord_y()+1);

                    if (get_marked_field()->y() >= viewPointY + gameFieldView->height())
                    {
                        viewPointY += SOLE_SQUARE_FIELD_SIZE;
                        move_view();
                    }
                    update_info_rect();
                }
            }
            break;
        case (Qt::Key_D):
        case (Qt::Key_Right):
            if (game->get_state() == STATE_UNIT_PURCHASE)
            {
                if (mark.get_coord_x()+1 <= unitPurchaseSceneMarkBorder_X2 + FROM_POS_TO_GAMEFIELD_COORD(unitPurchaseSceneGroup->x()))
                {
                    mark.move(mark.get_coord_x()+1, mark.get_coord_y());
                    selectedUnit = static_cast<unit_type>(static_cast<int>(selectedUnit)+1);
                }
                else
                {
                    mark.move(mark.get_coord_x()-UNIT_TYPE_MAX+1, mark.get_coord_y());
                    selectedUnit = static_cast<unit_type>(UNIT_TYPE_NONE+1);
                }
                show_unit_info_on_purchase_scene();
            }
            else
            {
                if (mark.get_coord_x()+1 < FIELD_NUM_X)
                {
                    mark.move(mark.get_coord_x()+1, mark.get_coord_y());

                    if (get_marked_field()->x() >= viewPointX + gameFieldView->width())
                    {
                        viewPointX += SOLE_SQUARE_FIELD_SIZE;
                        move_view();
                    }
                    update_info_rect();
                }
            }
            break;
        case (Qt::Key_A):
        case (Qt::Key_Left):
            if (game->get_state() == STATE_UNIT_PURCHASE)
            {
                if (mark.get_coord_x()-1 >= unitPurchaseSceneMarkBorder_X1 + FROM_POS_TO_GAMEFIELD_COORD(unitPurchaseSceneGroup->x()))
                {
                    mark.move(mark.get_coord_x()-1, mark.get_coord_y());
                    selectedUnit = static_cast<unit_type>(static_cast<int>(selectedUnit)-1);
                }
                else
                {
                    mark.move(mark.get_coord_x()+UNIT_TYPE_MAX-1, mark.get_coord_y());
                    selectedUnit = static_cast<unit_type>(UNIT_TYPE_MAX-1);
                }
                show_unit_info_on_purchase_scene();
            }
            else
            {
                if (mark.get_coord_x()-1 >= 0)
                {
                    mark.move(mark.get_coord_x()-1, mark.get_coord_y());

                    if (get_marked_field()->x() < viewPointX)
                    {
                        viewPointX -= SOLE_SQUARE_FIELD_SIZE;
                        move_view();
                    }
                    update_info_rect();
                }
            }
            break;
        case (Qt::Key_Return):
            if (game->get_state() == STATE_BASIC)
            {
                if (game->is_player_losing(game->get_cur_player_color()))
                    game->decrement_countdown(game->get_cur_player_color());

                next_turn();
            }
            break;
        case (Qt::Key_Space):
        {
            switch (game->get_state())
            {
                case STATE_BASIC:
                    if (get_marked_field_unit() && get_marked_field_unit()->get_fraction() == game->get_cur_player_color() &&
                            get_marked_field_unit()->is_active())
                    {
                        selectedUnitField = get_marked_field();
                        calculate_and_show_possible_movements(possibleMovements);
                        game->set_state(STATE_UNIT_SELECTED);
                    }
                    else if (get_marked_field_unit() == nullptr && get_marked_field()->get_terrain_type() == TERRAIN_TYPE_CASTLE
                            && castles.value(mark.get_marked_coord_pair()).fraction == game->get_cur_player_color())
                    {
                        show_unit_purchase_scene();
                    }
                    break;
                case STATE_UNIT_SELECTED:
                    if (possibleMovements.contains(mark.get_marked_coord_pair()) &&
                            selectedUnitField->get_unit()->is_active())
                    {
                        // if field is a possible attack direction
                        if (possibleMovements.value(mark.get_marked_coord_pair()).moveType
                                                            == UNIT_POSSIBLE_MOVE_TYPE_ENEMY_IN_ATTACK_RANGE)
                        {
                            unit_combat_outcome combatOutcome = selectedUnitField->get_unit()->attack(
                                        get_marked_field_unit(),
                                        fields[mark.get_coord_x()][mark.get_coord_y()].get_defense_bonus());
                            switch (combatOutcome)
                            {
                                case UNIT_COMBAT_DEFENDER_DESTROYED:
                                    remove_unit_from_gamefield(get_marked_field());

                                    if (selectedUnitField->get_unit()->get_attack_type() == UNIT_ATTACK_TYPE_MELEE)
                                    {
                                        move_unit_to_another_field(selectedUnitField, get_marked_field(),
                                                possibleMovements.value(mark.get_marked_coord_pair()).minElapsedSpeed);
                                    }
                                    break;
                                case UNIT_COMBAT_ATTACKER_DESTROYED:
                                    remove_unit_from_gamefield(selectedUnitField);
                                    break;
                                default:
                                    break;
                            }

                        }
                        else if (possibleMovements.value(mark.get_marked_coord_pair()).moveType
                                                                == UNIT_POSSIBLE_MOVE_TYPE_ALLOWED)
                        {
                            move_unit_to_another_field(selectedUnitField, get_marked_field(),
                                possibleMovements.value(mark.get_marked_coord_pair()).minElapsedSpeed);
                        }

                    }
                    delete_possible_movements_rect();
                    possibleMovements.clear();
                    game->set_state(STATE_BASIC);
                    for (Unit *u: unitsOnGamefield)
                    {
                        u->set_active();
                        u->reset_speed();
                    }
                    break;
                case STATE_UNIT_PURCHASE:
                    mark.move(castleSpawnCoord_X, castleSpawnCoord_Y);
                    if (fields[castleSpawnCoord_X][castleSpawnCoord_Y].get_unit() == nullptr &&
                            game->get_cur_player_money() - Unit::get_record_from_default_stats_table(selectedUnit)->cost >= 0)
                    {
                        game->change_cur_player_money_amount(-Unit::get_record_from_default_stats_table(selectedUnit)->cost);
                        place_new_unit_on_gamefield(castleSpawnCoord_X, castleSpawnCoord_Y, selectedUnit);
                        update_info_rect();
                        update_hud();
                    }
                    game->set_state(STATE_BASIC);

                    unitPurchaseSceneGroup->hide();

                    infoRectGroup->show();
                    break;
                default:
                    qDebug() << "[-]WTF\n";
                    break;
            }
            break;
        }
        case (Qt::Key_F):
                if (game->get_state() == STATE_UNIT_PURCHASE)
                {
                    game->set_state(STATE_BASIC);

                    unitPurchaseSceneGroup->hide();
                    mark.move(castleSpawnCoord_X, castleSpawnCoord_Y);

                    infoRectGroup->show();
                }
                else if (game->get_state() == STATE_BASIC)
                {
                    if (get_marked_field()->get_terrain_type() == TERRAIN_TYPE_CASTLE
                            && castles.value(mark.get_marked_coord_pair()).fraction == game->get_cur_player_color())
                    {
                        show_unit_purchase_scene();
                    }
                }
            break;
        case (Qt::Key_Escape):
        {
            if (game->get_state() == STATE_UNIT_SELECTED)
            {
                possibleMovements.clear();
                delete_possible_movements_rect();
                game->set_state(STATE_BASIC);
            }
            else if (game->get_state() == STATE_UNIT_PURCHASE)
            {
                unitPurchaseSceneGroup->hide();
                infoRectGroup->show();
                mark.move(castleSpawnCoord_X, castleSpawnCoord_Y);
                game->set_state(STATE_BASIC);
            }
            break;
        }
        default:
            break;
    }
}

void GameField::calculate_and_show_possible_movements(QHash<QPair<int, int>, field_info>& possibleMovements)
{
    get_marked_field_unit()->calculate_possible_movements(possibleMovements);
    show_possible_movements(possibleMovements);
}

void GameField::show_possible_movements(QHash<QPair<int, int>, field_info> &possibleMovements)
{
    QBrush brushBlue;
    brushBlue.setStyle(Qt::SolidPattern);
    brushBlue.setColor(QColor(0, 0, 255, 128));

    QBrush brushRed;
    brushRed.setStyle(Qt::SolidPattern);
    brushRed.setColor(QColor(255, 0, 0, 128));

    int i = 0;
    int prevSize = posMovesRects.size();
    if (prevSize < possibleMovements.size())
    {
        posMovesRects.resize(possibleMovements.size());

        for (int j = prevSize; j < posMovesRects.size(); j++)
        {
            posMovesRects[j] = new QGraphicsRectItem(0, 0, SOLE_SQUARE_FIELD_SIZE,
                                                           SOLE_SQUARE_FIELD_SIZE);
            addItem(posMovesRects[j]);
        }
    }

    for (auto it = possibleMovements.begin(); it != possibleMovements.end(); it++)
    {
            if (it.value().moveType == UNIT_POSSIBLE_MOVE_TYPE_ENEMY_IN_ATTACK_RANGE)
                posMovesRects[i]->setBrush(brushRed);
            else if (it.value().moveType == UNIT_POSSIBLE_MOVE_TYPE_ALLOWED)
                posMovesRects[i]->setBrush(brushBlue);
            else
                continue;

            posMovesRects[i]->setPos(fields[it.key().first][it.key().second].x(),
                                     fields[it.key().first][it.key().second].y());
            posMovesRects[i]->show();
            i++;
    }

//    for (int j = prevSize; j < posMovesRects.size(); j++)
//    addItem(posMovesRects[j]);
}

void GameField::delete_possible_movements_rect()
{
    for(QGraphicsRectItem *rect: posMovesRects)
        rect->hide();
}

void GameField::place_new_unit_on_gamefield(int field_x, int field_y, unit_type type)
{
    Unit* newUnit = new Unit(field_x, field_y, type, game->get_cur_player_color());

    fields[field_x][field_y].set_unit(newUnit);

    addItem(fields[field_x][field_y].get_unit());

    unitsOnGamefield.push_back(newUnit);
}

void GameField::move_unit_to_another_field(SoleField *from, SoleField *to, int speedElapsed)
{
    if (from->get_unit() && to->get_unit() == nullptr)
    {
        from->get_unit()->move_unit(to->x(), to->y(), speedElapsed);

        to->set_unit(from->get_unit());
        from->set_unit(nullptr);

        if (to->get_terrain_type() == TERRAIN_TYPE_CASTLE)
        {
            // if the fraction of the unit being move doesn't
            // coincide with the fraction of the castle
            // change castle fraction
            int castleCoordX = FROM_POS_TO_GAMEFIELD_COORD(to->x());
            int castleCoordY = FROM_POS_TO_GAMEFIELD_COORD(to->y());
            if (castles.value(qMakePair(castleCoordX, castleCoordY)).fraction !=
                              to->get_unit()->get_fraction())
            set_new_castle_owner(qMakePair(castleCoordX, castleCoordY),
                                 to->get_unit()->get_fraction());
        }

        update_info_rect();
    }
}

void GameField::remove_unit_from_gamefield(SoleField *unitField)
{
    removeItem(unitField->get_unit());
    unitsOnGamefield.removeOne(unitField->get_unit());
    unitField->del_unit();
}

void GameField::update_info_rect()
{
    QString terrain;
    QString unitType;
    QString unitName;
    QString fractionName;
    QString defenseBonus;
    QString speedModificator;

    get_marked_field()->get_terrain_name(terrain);
    if (get_marked_field()->get_terrain_type() == TERRAIN_TYPE_CASTLE)
    {
        QString castleFractionName;
        switch (castles.value(mark.get_marked_coord_pair()).fraction)
        {
            case PLAYER_BLUE:
                castleFractionName = "blue";
                break;
            case PLAYER_GREEN:
                castleFractionName = "green";
                break;
            case PLAYER_RED:
                castleFractionName = "red";
                break;
            case PLAYER_BLACK:
                castleFractionName = "black";
                break;
            case PLAYER_NONE:
                castleFractionName = "none";
                break;
            default:
                castleFractionName = "default";
                break;
        }
        terrain = QString("%1 (%2)").arg(terrain, castleFractionName);
    }

    speedModificator = QString::number(get_marked_field()->get_speed_modificator());
    defenseBonus = QString::number(get_marked_field()->get_defense_bonus());

    if (get_marked_field_unit())
    {
        get_marked_field_unit()->get_fraction_name(fractionName);
        get_marked_field_unit()->get_name(unitType);
        unitType = QString("%1 (%2)").arg(unitType, fractionName);
    }
    infoRectText->setPlainText(QString("%1\n\nTerrain: %2\nDefense Bonus: %3%\nSpeed Modificator: %4\n").arg
                               (unitType, terrain, defenseBonus, speedModificator));
}

void GameField::update_info_rect_color()
{
    QList<QGraphicsItem*>list = infoRectGroup->childItems();
    for (QGraphicsItem* item: list)
    {
        QGraphicsRectItem* rect = dynamic_cast<QGraphicsRectItem*>(item);
        if (rect)
        {
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);

            switch (game->get_cur_player_color())
            {
                case PLAYER_BLUE:
                    brush.setColor(QColor(114, 123, 255, 200));
                    break;
                case PLAYER_GREEN:
                    brush.setColor(QColor(35, 193, 0, 200));
                    break;
                case PLAYER_RED:
                    brush.setColor(QColor(255, 0, 0, 200));
                    break;
                case PLAYER_BLACK:
                    brush.setColor(QColor(0, 0, 0, 200));
                    break;
                case PLAYER_NONE:
                    brush.setColor(Qt::white);
                    break;
                default:
                    break;
            }
            rect->setBrush(brush);
            break;
        }
    }
}

void GameField::move_view()
{
    horizontalBar->setValue(viewPointX);
    verticalBar->setValue(viewPointY);

    hud->setPos(viewPointX, viewPointY);
    curPlayerRectGroup->setPos(viewPointX, viewPointY +
                               gameFieldView->height()/2 - SOLE_SQUARE_FIELD_SIZE);
    infoRectGroup->setPos(viewPointX, viewPointY + gameFieldView->height() - 250);
}

void GameField::prepare_unit_purchase_scene()
{
    QBrush brush;
    QGraphicsPixmapItem *unitImgs[UNIT_TYPE_MAX];

    unitPurchaseSceneGroup = new QGraphicsItemGroup;

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);

    QGraphicsRectItem* unitPurchaseSceneRect = new QGraphicsRectItem(0, 0, 900, 700, unitPurchaseSceneGroup);
    unitPurchaseSceneRect->setBrush(brush);
//    unitPurchaseSceneGroup->addToGroup(unitPurchaseSceneRect);

    QGraphicsRectItem* nameBlock = new QGraphicsRectItem(0, 0, 900, 100, unitPurchaseSceneGroup);
    QGraphicsRectItem* strengthBlock = new QGraphicsRectItem(0, 100, 450, 100, unitPurchaseSceneGroup);
    QGraphicsRectItem* speedBlock = new QGraphicsRectItem(450, 100, 450, 100, unitPurchaseSceneGroup);
    QGraphicsRectItem* attackTypeBlock = new QGraphicsRectItem(0, 200, 450, 100, unitPurchaseSceneGroup);
    QGraphicsRectItem* attackRangeBlock = new QGraphicsRectItem(450, 200, 450, 100, unitPurchaseSceneGroup);
    QGraphicsRectItem* descriptionBlock = new QGraphicsRectItem(0, 300, 900, 100, unitPurchaseSceneGroup);

    nameString = new QGraphicsTextItem[UNIT_TYPE_MAX];
    costString = new QGraphicsTextItem[UNIT_TYPE_MAX];
    strengthString = new QGraphicsTextItem[UNIT_TYPE_MAX];
    speedString = new QGraphicsTextItem[UNIT_TYPE_MAX];
    attackTypeString = new QGraphicsTextItem[UNIT_TYPE_MAX];
    attackRangeString = new QGraphicsTextItem[UNIT_TYPE_MAX];
    descriptionString = new QGraphicsTextItem[UNIT_TYPE_MAX];

//    QGraphicsItemGroup *infoStringsGroup = new QGraphicsItemGroup(unitPurchaseSceneGroup);
    for (int i = 0; i < UNIT_TYPE_MAX; i++)
    {
        unitPurchaseSceneGroup->addToGroup(&nameString[i]);
        unitPurchaseSceneGroup->addToGroup(&costString[i]);
        unitPurchaseSceneGroup->addToGroup(&strengthString[i]);
        unitPurchaseSceneGroup->addToGroup(&speedString[i]);
        unitPurchaseSceneGroup->addToGroup(&attackTypeString[i]);
        unitPurchaseSceneGroup->addToGroup(&attackRangeString[i]);
        unitPurchaseSceneGroup->addToGroup(&descriptionString[i]);
    }
//    unitPurchaseSceneGroup->addToGroup(infoStringsGroup);
    QPixmap i(":/terrain/img/coin.png");
    QGraphicsPixmapItem *coin = new QGraphicsPixmapItem(unitPurchaseSceneGroup);
    coin->setPixmap(i.scaled(QSize(55, 55), Qt::KeepAspectRatio));
    coin->setPos(700, 35);

    for (int i = 0; i < UNIT_TYPE_MAX; i++)
    {
        const defaultUnitStats* p = Unit::get_record_from_default_stats_table(static_cast<unit_type>(i));

        nameString[i].setPlainText(p->name);
        nameString[i].setScale(3);
        nameString[i].setPos(nameBlock->boundingRect().width()/2-nameString[i].boundingRect().width()*3/2, 25);
        nameString[i].hide();

        costString[i].setPlainText(QString::number(p->cost));
        costString[i].setScale(3);
        costString[i].setPos(coin->x()+55, 25);
        costString[i].hide();

        strengthString[i].setPlainText(QString("Strength: %1").arg(QString::number(p->strength)));
        strengthString[i].setScale(3);
        strengthString[i].setPos(strengthBlock->boundingRect().width()/2 - strengthString[i].boundingRect().width()*3/2,
                                 strengthBlock->boundingRect().y() + strengthBlock->boundingRect().height()/2 - strengthString[i].boundingRect().height()*3/2);
        strengthString[i].hide();

        speedString[i].setPlainText(QString("Speed: %1").arg(QString::number(p->speed)));
        speedString[i].setScale(3);
        speedString[i].setPos(speedBlock->boundingRect().x() + speedBlock->boundingRect().width()/2 - speedString[i].boundingRect().width()*3/2,
                              speedBlock->boundingRect().y() + speedBlock->boundingRect().height()/2 - speedString[i].boundingRect().height()*3/2);
        speedString[i].hide();

        if (p->attackType == UNIT_ATTACK_TYPE_MELEE)
            attackTypeString[i].setPlainText("Attack type: Melee");
        else
            attackTypeString[i].setPlainText("Attack Type: Ranged");
        attackTypeString[i].setScale(3);
        attackTypeString[i].setPos(attackTypeBlock->boundingRect().width()/2 - attackTypeString[i].boundingRect().width()*3/2,
                                   attackTypeBlock->boundingRect().y() + attackTypeBlock->boundingRect().height()/2 - attackTypeString[i].boundingRect().height()*3/2);
        attackTypeString[i].hide();

        attackRangeString[i].setPlainText(QString("Attack Range: %1").arg(QString::number(p->attackRange)));
        attackRangeString[i].setScale(3);
        attackRangeString[i].setPos(attackRangeBlock->boundingRect().x() + attackRangeBlock->boundingRect().width()/2 - attackRangeString[i].boundingRect().width()*3/2,
                                    attackRangeBlock->boundingRect().y() + attackRangeBlock->boundingRect().height()/2 - attackRangeString[i].boundingRect().height()*3/2);
        attackRangeString[i].hide();

        QPixmap img(p->imgPath);
        unitImgs[i] = new QGraphicsPixmapItem(unitPurchaseSceneGroup);
        unitImgs[i]->setPixmap(img.scaled(QSize(SOLE_SQUARE_FIELD_SIZE, SOLE_SQUARE_FIELD_SIZE), Qt::KeepAspectRatio));
        unitImgs[i]->setPos(100*(i+1), 500);
    }

    unitPurchaseSceneMarkBorder_X1 = FROM_POS_TO_GAMEFIELD_COORD(unitImgs[0]->pos().x());
    unitPurchaseSceneMarkBorder_X2 = FROM_POS_TO_GAMEFIELD_COORD(unitImgs[UNIT_TYPE_MAX-1]->pos().x());
    unitPurchaseSceneMarkBorder_Y  = FROM_POS_TO_GAMEFIELD_COORD(unitImgs[0]->pos().y());

    unitPurchaseSceneGroup->setZValue(0.9);
    addItem(unitPurchaseSceneGroup);
    unitPurchaseSceneGroup->hide();
}

void GameField::show_unit_info_on_purchase_scene()
{
    static unit_type prevSelectedUnit = UNIT_TYPE_WARRIOR;

    nameString[prevSelectedUnit].hide();
    nameString[selectedUnit].show();
    costString[prevSelectedUnit].hide();
    costString[selectedUnit].show();
    strengthString[prevSelectedUnit].hide();
    strengthString[selectedUnit].show();
    speedString[prevSelectedUnit].hide();
    speedString[selectedUnit].show();
    attackTypeString[prevSelectedUnit].hide();
    attackTypeString[selectedUnit].show();
    attackRangeString[prevSelectedUnit].hide();
    attackRangeString[selectedUnit].show();

    prevSelectedUnit = selectedUnit;
}

void GameField::set_new_castle_owner(QPair<int, int> castleCoord, player_color newOwner)
{
    real_estate newRecord;
    real_estate prevRecord;

    prevRecord = castles.value(castleCoord);
    newRecord.fraction = newOwner;
    newRecord.fractionRect = prevRecord.fractionRect;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    switch (newRecord.fraction)
    {
        case PLAYER_BLUE:
            brush.setColor(Qt::blue);
            break;
        case PLAYER_GREEN:
            brush.setColor(Qt::green);
            break;
        case PLAYER_RED:
            brush.setColor(Qt::red);
            break;
        case PLAYER_BLACK:
            brush.setColor(Qt::black);
            break;
        default:
            brush.setColor(Qt::white);
            break;
    }
    newRecord.fractionRect->setBrush(brush);

    castles.insert(castleCoord, newRecord);

    if (prevRecord.fraction != PLAYER_NONE)
    {
        game->change_player_income(prevRecord.fraction, -INCOME_CASTLE);
        check_if_player_has_any_castles_left(prevRecord.fraction);
    }

    if (newOwner != PLAYER_NONE)
    {
        game->change_player_income(newOwner, INCOME_CASTLE);
        if (game->is_player_losing(newOwner))
            game->set_player_countdown(newOwner, false);
    }
    update_hud();
}

void GameField::check_if_player_has_any_castles_left(player_color player)
{
    for (auto it = castles.begin(); it != castles.end(); it++)
        if (it.value().fraction == player)
            return;

    game->set_player_countdown(player, true);
}

void GameField::parse_map_file()
{
    std::ifstream mapFile;
    mapFile.open("map.txt");
    std::string line;

    if (!mapFile.is_open())
    {
        qDebug() << "[-]Can't open map file\n";
        return;
    }

    // first line should be like this: <width,height>
    getline(mapFile, line);
    auto openBracketPos  = line.rfind('<');
    auto closeBracketPos = line.rfind('>');
    auto firstCommaPos = line.rfind(',');
    gameFieldWidth  = std::stoi(line.substr(openBracketPos+1, firstCommaPos-1));
    gameFieldHeight = std::stoi(line.substr(firstCommaPos+1, closeBracketPos-1));

    fields = new SoleField*[gameFieldWidth];
    for (int i = 0; i < gameFieldWidth; i++)
        fields[i] = new SoleField[gameFieldHeight];

    while (std::getline(mapFile, line))
    {
        auto commaPos = line.rfind(',');
        auto semicolonPos = line.rfind(':');
        int coordX = std::stoi(line.substr(0, commaPos));
        int coordY = std::stoi(line.substr(commaPos+1, semicolonPos-2));
        terrain_type tType = static_cast<terrain_type>(std::stoi(line.substr(semicolonPos+1)));
        //qDebug() << "[" << coordX << ":" << coordY << "]:" << tType << endl;

        fields[coordX][coordY].setImg(tType);

        fields[coordX][coordY].setPos(coordX*SOLE_SQUARE_FIELD_SIZE, coordY*SOLE_SQUARE_FIELD_SIZE);
        this->addItem(&fields[coordX][coordY]);

//        QGraphicsTextItem *text  = new QGraphicsTextItem;
//        text->setPlainText(QString("%1:%2").arg(QString::number(coordX), QString::number(coordY)));
//        text->setPos(fields[coordX][coordY].x()+10, fields[coordX][coordY].y()+10);
//        text->setZValue(0.75);
//        addItem(text);
    }
}

void GameField::next_turn()
{
    game->next_turn();
    game->change_cur_player_money_amount(game->get_cur_player_income());

    update_hud();
    show_cur_player_rect();
    update_info_rect_color();

    for (Unit *u: unitsOnGamefield)
    {
        u->set_active();
        u->reset_speed();
    }
}

void GameField::update_hud()
{
    moneyAmountText->setPlainText(QString("%1(+%2)").arg
                           (QString::number(game->get_cur_player_money()),
                            QString::number(game->get_cur_player_income())));
}

SoleField *GameField::get_marked_field()
{
    return &fields[mark.get_coord_x()][mark.get_coord_y()];
}

Unit *GameField:: get_marked_field_unit()
{
    return fields[mark.get_coord_x()][mark.get_coord_y()].get_unit();
}

void GameField::prepare_info_rect()
{
    infoRectGroup = new QGraphicsItemGroup;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255, 90, 90, 232));

    QGraphicsRectItem *infoRect = new QGraphicsRectItem(0, 0, 300, 150, infoRectGroup);
    infoRect->setBrush(brush);

    infoRectText  = new QGraphicsTextItem(infoRectGroup);
    infoRectText->setDefaultTextColor(Qt::white);
    infoRectText->setZValue(1);
    infoRectText->setPos(infoRect->x() + 50, infoRect->y() + 25);

    infoRectGroup->setPos(viewPointX, viewPointY + gameFieldView->height() - 250);
    infoRectGroup->setZValue(1);

    addItem(infoRectGroup);
}

void GameField::prepare_hud()
{
    hud = new QGraphicsItemGroup;
    hud->setPos(0, 0);

    QPixmap i(":/terrain/img/coin.png");
    QGraphicsPixmapItem *coin = new QGraphicsPixmapItem(hud);
    coin->setPixmap(i.scaled(QSize(60, 60), Qt::KeepAspectRatio));
    coin->setPos(gameFieldView->width() - 300, 10);

    moneyAmountText = new QGraphicsTextItem(hud);
    moneyAmountText->setPlainText(QString("%1(+%2)").arg
                           (QString::number(game->get_cur_player_money()),
                            QString::number(game->get_cur_player_income())));
    moneyAmountText->setDefaultTextColor(Qt::yellow);
    moneyAmountText->setTransformOriginPoint(moneyAmountText->boundingRect().center());
    moneyAmountText->setScale(2);
    moneyAmountText->setPos(coin->x() + coin->boundingRect().width() + moneyAmountText->boundingRect().width()/2,
                            coin->y() + coin->boundingRect().height()/2 - moneyAmountText->boundingRect().height()/2);

    hud->setZValue(1);

    addItem(hud);
}

void GameField::prepare_cur_player_rect()
{
    curPlayerRectGroup = new QGraphicsItemGroup;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(0, 0, 0, 200));

    QGraphicsRectItem* curPlayerRect =
            new QGraphicsRectItem(0, 0, gameFieldView->width(), SOLE_SQUARE_FIELD_SIZE*2, curPlayerRectGroup);
    curPlayerRect->setBrush(brush);
    curPlayerRect->boundingRect().height();

    brush.setColor(Qt::white);
    QGraphicsLineItem *line1 = new QGraphicsLineItem(curPlayerRect->x(), curPlayerRect->y(),
                                                     curPlayerRect->x()+curPlayerRect->boundingRect().width(),
                                                     curPlayerRect->y(),
                                                     curPlayerRectGroup);
    QGraphicsLineItem *line2 = new QGraphicsLineItem(curPlayerRect->x(), curPlayerRect->y()+curPlayerRect->boundingRect().height(),
                                                     curPlayerRect->x()+curPlayerRect->boundingRect().width(),
                                                     curPlayerRect->y()+curPlayerRect->boundingRect().height(),
                                                     curPlayerRectGroup);
    QPen pen;
    pen.setColor(Qt::white);
    line1->setPen(pen);
    line2->setPen(pen);

    curPlayerRectText = new QGraphicsTextItem(curPlayerRectGroup);
    curPlayerRectText->setDefaultTextColor(Qt::white);
    curPlayerRectText->setPlainText("Green player turn");
    curPlayerRectText->setTransformOriginPoint(curPlayerRectText->boundingRect().center());
    curPlayerRectText->setScale(3);
    curPlayerRectText->setPos(curPlayerRect->boundingRect().width()/2 - curPlayerRectText->boundingRect().width()/2,
                              curPlayerRect->boundingRect().height()/2 - curPlayerRectText->boundingRect().height()/2);

    curPlayerRectGroup->setPos(0, gameFieldView->height()/2 - curPlayerRect->boundingRect().height()/2);
    curPlayerRectGroup->setZValue(1);

    addItem(curPlayerRectGroup);
    curPlayerRectGroup->hide();
}

void GameField::create_gamefield()
{
    parse_map_file();

    fields[1][1].setImg(TERRAIN_TYPE_CASTLE);
    castles.insert(qMakePair(1, 1), (real_estate){PLAYER_NONE, new QGraphicsRectItem(fields[1][1].x(), fields[1][1].y(), 15, 15)});
    fields[1][5].setImg(TERRAIN_TYPE_CASTLE);
    castles.insert(qMakePair(1, 5), (real_estate){PLAYER_NONE, new QGraphicsRectItem(fields[1][5].x(), fields[1][5].y(), 15, 15)});
    fields[5][1].setImg(TERRAIN_TYPE_CASTLE);
    castles.insert(qMakePair(5, 1), (real_estate){PLAYER_NONE, new QGraphicsRectItem(fields[5][1].x(), fields[5][1].y(), 15, 15)});
    fields[5][5].setImg(TERRAIN_TYPE_CASTLE);
    castles.insert(qMakePair(5, 5), (real_estate){PLAYER_NONE, new QGraphicsRectItem(fields[5][5].x(), fields[5][5].y(), 15, 15)});
    fields[3][3].setImg(TERRAIN_TYPE_CASTLE);
    castles.insert(qMakePair(3, 3), (real_estate){PLAYER_NONE, new QGraphicsRectItem(fields[3][3].x(), fields[3][3].y(), 15, 15)});

    int i = 0;
    for (auto it = castles.begin(); it != castles.end(); it++)
    {
        if (i < game->get_player_num())
            set_new_castle_owner(it.key(), static_cast<player_color>(i));
        else
            set_new_castle_owner(it.key(), PLAYER_NONE);
        addItem(it.value().fractionRect);
        i++;
    }
}

void GameField::show_cur_player_rect()
{
    QString curPlayerName;
    game->get_cur_player_name(curPlayerName);

    if (game->is_player_losing(game->get_cur_player_color()))
        curPlayerRectText->setPlainText(QString("%1 player turn (Turns before losing:%2)").arg(
                                            curPlayerName, QString::number(game->get_turns_left(game->get_cur_player_color()))));
    else
        curPlayerRectText->setPlainText(QString("%1 player turn").arg(curPlayerName));

    curPlayerRectText->setPos(gameFieldView->width()/2  - curPlayerRectText->boundingRect().width()/2*3,
                              curPlayerRectText->y());
    if (curPlayerRectGroup->isVisible())
        qDebug() << gameFieldView->width()/2 << "-" << curPlayerRectText->boundingRect().width()/2; //TODO
    curPlayerRectGroup->show();

    QTimer timer;
    auto localGroup = curPlayerRectGroup;
    timer.singleShot(5000, [localGroup](){localGroup->hide();});
}

void GameField::update_cur_player_rect()
{
    curPlayerRectGroup->setPos(viewPointX, viewPointY +
                               gameFieldView->height()/2 - SOLE_SQUARE_FIELD_SIZE);
    curPlayerRectText->setPos(gameFieldView->width()/2 - curPlayerRectText->boundingRect().width()/2,
                              gameFieldView->height()/2 - curPlayerRectText->boundingRect().height()/2);
}

void GameField::show_unit_purchase_scene()
{
    game->set_state(STATE_UNIT_PURCHASE);
    selectedUnit = UNIT_TYPE_WARRIOR;

    castleSpawnCoord_X = mark.get_coord_x();
    castleSpawnCoord_Y = mark.get_coord_y();

    unitPurchaseSceneGroup->setPos(viewPointX + 400, viewPointY + 100);
    unitPurchaseSceneGroup->show();
    show_unit_info_on_purchase_scene();
    mark.move(unitPurchaseSceneMarkBorder_X1 + FROM_POS_TO_GAMEFIELD_COORD(unitPurchaseSceneGroup->x()),
              unitPurchaseSceneMarkBorder_Y  + FROM_POS_TO_GAMEFIELD_COORD(unitPurchaseSceneGroup->y()));

    infoRectGroup->hide();
}
