#ifndef TRAFFIC_SCENE_H
#define TRAFFIC_SCENE_H

#include <QGraphicsScene>

class TrafficScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit TrafficScene(QObject *parent = nullptr);

private:
  void setupCrossroad();
  void setupTrafficLights();
  void setupPedestrianLights();
  void setupCrosswalks();
};

#endif
