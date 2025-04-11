#include "crosswalk_item.h"
#include <QPainter>

CrosswalkItem::CrosswalkItem(QPointF pos, bool horizontal)
    : horizontal(horizontal) {
    setPos(pos);
}

QRectF CrosswalkItem::boundingRect() const {
    return horizontal ? QRectF(0, 0, 200, 10) : QRectF(0, 0, 10, 200);
}

void CrosswalkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::white);
    if (horizontal) {
        for (int i = 0; i < 10; ++i)
            painter->drawRect(i * 20, 0, 10, 20);
    } else {
        for (int i = 0; i < 10; ++i)
            painter->drawRect(0, i * 20, 20, 10);
    }
}
