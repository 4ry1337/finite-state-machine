#include "traffic_fsm.h"
#include "traffic_scene.h"
#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QGraphicsView view;
  auto *scene = new TrafficScene();
  view.setScene(scene);
  view.setRenderHint(QPainter::Antialiasing);
  view.setWindowTitle("Traffic Light FSM");
  view.resize(800, 600);
  view.show();

  TrafficFSM *fsm = new TrafficFSM();

  QList<TrafficLightItem *> northSouth;
  QList<TrafficLightItem *> eastWest;
  QList<TrafficLightItem *> pedestrians;

  for (QGraphicsItem *item : scene->items()) {
    if (auto *light = dynamic_cast<TrafficLightItem *>(item)) {
      if (light->isPedestrian())
        pedestrians.append(light);
      else if (light->pos().y() < 300 || light->pos().y() > 500)
        northSouth.append(light);
      else
        eastWest.append(light);
    }
  }

  fsm->setTrafficLights(northSouth, eastWest, pedestrians);
  fsm->start();

  return app.exec();
}
