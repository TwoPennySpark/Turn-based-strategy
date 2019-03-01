#include "markfield.h"

MarkField::MarkField()
{
    rect1 = new QGraphicsRectItem(10, 10, 10, 10, this);
    rect2 = new QGraphicsRectItem(80, 10, 10, 10, this);
    rect3 = new QGraphicsRectItem(10, 80, 10, 10, this);
    rect4 = new QGraphicsRectItem(80, 80, 10, 10, this);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);

    rect1->setBrush(brush);
    rect2->setBrush(brush);
    rect3->setBrush(brush);
    rect4->setBrush(brush);

    markCoord_X = 0;
    markCoord_Y = 0;

    // bring mark to front(foreground)
    setZValue(1);

    setPos(0, 0);
}

MarkField::~MarkField()
{
    delete rect1;
    delete rect2;
    delete rect3;
    delete rect4;
}

void MarkField::move(int field_x, int field_y)
{
    setPos(field_x * SOLE_SQUARE_FIELD_SIZE, field_y * SOLE_SQUARE_FIELD_SIZE);
    markCoord_X = field_x;
    markCoord_Y = field_y;
}

int MarkField::get_coord_x()
{
    return markCoord_X;
}

int MarkField::get_coord_y()
{
    return markCoord_Y;
}

QPair<int, int> &MarkField::get_marked_coord_pair()
{
    markCoordPair.first = markCoord_X;
    markCoordPair.second = markCoord_Y;
    return markCoordPair;
}
