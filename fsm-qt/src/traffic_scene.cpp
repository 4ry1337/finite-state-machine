#include "traffic_scene.h"
#include "crosswalk_item.h"
#include "traffic_light_item.h"
#include <QGraphicsRectItem>

TrafficScene::TrafficScene(QObject *parent) : QGraphicsScene(parent) {
  setSceneRect(0, 0, 1000, 1000);
  setupCrossroad();
  setupTrafficLights();
  setupPedestrianLights();
  setupCrosswalks();
}

void TrafficScene::setupCrossroad() {
  // horizontal line
  addRect(0, 300, 800, 200, QPen(Qt::NoPen), QBrush(Qt::darkGray));
  // vertical line
  addRect(300, 0, 200, 800, QPen(Qt::NoPen), QBrush(Qt::darkGray));
}

void TrafficScene::setupTrafficLights() {
  // top
  addItem(new TrafficLightItem(QPointF(380, 200), Qt::Vertical));
  // bottom
  addItem(new TrafficLightItem(QPointF(380, 540), Qt::Vertical));
  // left
  addItem(new TrafficLightItem(QPointF(200, 370), Qt::Vertical));
  // right
  addItem(new TrafficLightItem(QPointF(540, 370), Qt::Vertical));
}

void TrafficScene::setupPedestrianLights() {
  // top
  addItem(new TrafficLightItem(QPointF(420, 200), Qt::Vertical, true));
  // bottom
  addItem(new TrafficLightItem(QPointF(420, 540), Qt::Vertical, true));
  // left
  addItem(new TrafficLightItem(QPointF(240, 370), Qt::Vertical, true));
  // right
  addItem(new TrafficLightItem(QPointF(580, 370), Qt::Vertical, true));
}

void TrafficScene::setupCrosswalks() {
  // horizontal crosswalks
  addItem(new CrosswalkItem(QPointF(305, 270), true));
  addItem(new CrosswalkItem(QPointF(305, 510), true));
  // vertical crosswalks
  addItem(new CrosswalkItem(QPointF(270, 305), false));
  addItem(new CrosswalkItem(QPointF(510, 305), false));
}
