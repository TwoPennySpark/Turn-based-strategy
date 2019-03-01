#ifndef MARKFIELD_H
#define MARKFIELD_H

#include <QGraphicsItemGroup>
#include <QBrush>
#include <QPair>
#include "solefield.h"

class MarkField: public QGraphicsItemGroup
{
public:
    MarkField();
    ~MarkField();

    void move(int field_x, int field_y);
    int get_coord_x();
    int get_coord_y();

    QPair<int, int>& get_marked_coord_pair();
private:
    int markCoord_X;
    int markCoord_Y;
    QPair<int, int> markCoordPair;

    QGraphicsRectItem* rect1;
    QGraphicsRectItem* rect2;
    QGraphicsRectItem* rect3;
    QGraphicsRectItem* rect4;
};

#endif // MARKFIELD_H
