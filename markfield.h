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

    void move(uint field_x, uint field_y);
    uint get_coord_x();
    uint get_coord_y();

    QPair<int, int>& get_marked_coord_pair();

private:
    uint markCoord_X;
    uint markCoord_Y;
    QPair<int, int> markCoordPair;

    QGraphicsPolygonItem* polygon1;
    QGraphicsPolygonItem* polygon2;
    QGraphicsPolygonItem* polygon3;
    QGraphicsPolygonItem* polygon4;
};

#endif // MARKFIELD_H
