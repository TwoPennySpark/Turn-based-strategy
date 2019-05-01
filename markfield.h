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
    int get_coord_x() const;
    int get_coord_y() const;

    QPair<int, int>& get_marked_coord_pair();

private:
    int markCoord_X;
    int markCoord_Y;
    QPair<int, int> markCoordPair;

    QGraphicsPolygonItem* polygon1;
    QGraphicsPolygonItem* polygon2;
    QGraphicsPolygonItem* polygon3;
    QGraphicsPolygonItem* polygon4;
};

#endif // MARKFIELD_H
