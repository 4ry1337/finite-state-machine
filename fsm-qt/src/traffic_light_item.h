#ifndef TRAFFIC_LIGHT_ITEM_H
#define TRAFFIC_LIGHT_ITEM_H

#include <QColor>
#include <QGraphicsItem>

class TrafficLightItem : public QGraphicsItem {
public:
  TrafficLightItem(QPointF pos, Qt::Orientation orientation,
                   bool pedestrian = false);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
  Qt::Orientation orientation;
  bool pedestrian;
  QColor currentColor;
  bool isPedestrian() const { return pedestrian; }
  bool buttonPressed = false;
};

#endif
