#pragma once

#include <QGraphicsItem>

class CrosswalkItem : public QGraphicsItem {
public:
    CrosswalkItem(QPointF pos, bool horizontal);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    bool horizontal;
};
