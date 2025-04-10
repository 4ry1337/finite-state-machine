#pragma once

#include <QGraphicsItem>
#include <QColor>

class TrafficLightItem : public QGraphicsItem {
public:
    TrafficLightItem(QPointF pos, Qt::Orientation orientation, bool pedestrian = false);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    Qt::Orientation orientation;
    bool pedestrian;
    QColor currentColor;
    bool buttonPressed = false;
};
