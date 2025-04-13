#include "traffic_light_item.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

TrafficLightItem::TrafficLightItem(QPointF pos, Qt::Orientation orientation,
                                   bool pedestrian)
    : orientation(orientation), pedestrian(pedestrian), currentColor(Qt::red) {
  setPos(pos);
  setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF TrafficLightItem::boundingRect() const {
  return pedestrian ? QRectF(0, 0, 20, 60) : QRectF(0, 0, 20, 60);
}

void TrafficLightItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *, QWidget *) {
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setBrush(Qt::black);
  painter->drawRect(boundingRect());

  if (pedestrian) {
    // Red human figure (top)
    QRectF redBox(5, 5, 10, 10);
    painter->setBrush(currentColor == Qt::red ? Qt::red : Qt::darkRed);
    painter->drawRect(redBox);
    // Green walking figure (middle)
    QRectF greenBox(5, 25, 10, 10);
    painter->setBrush(currentColor == Qt::green ? Qt::green : Qt::darkGreen);
    painter->drawRect(greenBox);

    // Button (bottom)
    painter->setBrush(buttonPressed ? Qt::darkGray : Qt::lightGray);
    painter->setPen(Qt::black);
    painter->drawRect(5, 45, 10, 10);
  } else {
    // Standard vehicle traffic light
    painter->setBrush(currentColor == Qt::red ? Qt::red : Qt::darkRed);
    painter->drawEllipse(5, 5, 10, 10);
    painter->setBrush(currentColor == Qt::yellow ? Qt::yellow : Qt::darkYellow);
    painter->drawEllipse(5, 25, 10, 10);
    painter->setBrush(currentColor == Qt::green ? Qt::green : Qt::darkGreen);
    painter->drawEllipse(5, 45, 10, 10);
  }
}

void TrafficLightItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (pedestrian) {
    QRectF buttonRect(5, 45, 10, 10); // Match your button location
    if (buttonRect.contains(event->pos())) {
      buttonPressed = true;
      qDebug() << "Pedestrian button pressed at:" << pos();
      update();
    }
  }
}
