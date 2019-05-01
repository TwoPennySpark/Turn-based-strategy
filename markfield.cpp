#include "markfield.h"
//#include <algorithm>
MarkField::MarkField()
{
    QVector<QPointF> markPoints;
    markPoints << QPointF(1,1) << QPointF(1,3) << QPointF(3,3)
               << QPointF(3,2) << QPointF(2,2) << QPointF(2,1);

    std::for_each(markPoints.begin(), markPoints.end(), [](QPointF& point){int scaleBy = 5; point*=scaleBy;});

    QPolygonF tempPolygon(markPoints);

    QGraphicsPolygonItem* polygon1 = new QGraphicsPolygonItem(tempPolygon, this);
    QGraphicsPolygonItem* polygon2 = new QGraphicsPolygonItem(tempPolygon, this);
    QGraphicsPolygonItem* polygon3 = new QGraphicsPolygonItem(tempPolygon, this);
    QGraphicsPolygonItem* polygon4 = new QGraphicsPolygonItem(tempPolygon, this);

    polygon1->setTransformOriginPoint(polygon1->boundingRect().center());
    polygon1->setRotation(90);
    polygon1->setPos(0, 0);

    polygon2->setTransformOriginPoint(polygon2->boundingRect().center());
    polygon2->setRotation(180);
    polygon2->setPos(80, 0);

    polygon3->setTransformOriginPoint(polygon3->boundingRect().center());
    polygon3->setRotation(270);
    polygon3->setPos(80, 80);

    polygon4->setTransformOriginPoint(polygon4->boundingRect().center());
    polygon4->setRotation(0);
    polygon4->setPos(0, 80);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);

    polygon1->setBrush(brush);
    polygon2->setBrush(brush);
    polygon3->setBrush(brush);
    polygon4->setBrush(brush);

    markCoord_X = 0;
    markCoord_Y = 0;

    // bring mark to front(foreground)
    setZValue(1);

    setPos(0, 0);
}

MarkField::~MarkField()
{
    delete polygon1;
    delete polygon2;
    delete polygon3;
    delete polygon4;
}

void MarkField::move(int field_x, int field_y)
{
    setPos(FROM_GAMEFIELD_TO_POS_COORD(field_x),
           FROM_GAMEFIELD_TO_POS_COORD(field_y));
    markCoord_X = field_x;
    markCoord_Y = field_y;
}

int MarkField::get_coord_x() const
{
    return markCoord_X;
}

int MarkField::get_coord_y() const
{
    return markCoord_Y;
}

QPair<int, int> &MarkField::get_marked_coord_pair()
{
    markCoordPair.first = markCoord_X;
    markCoordPair.second = markCoord_Y;
    return markCoordPair;
}
