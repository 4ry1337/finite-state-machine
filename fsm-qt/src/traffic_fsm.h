#pragma once
#include "traffic_light_item.h"
#include "traffic_state.h"
#include <QGraphicsTextItem>
#include <QObject>
#include <QTimer>

class TrafficFSM : public QObject {
  Q_OBJECT

public:
  TrafficFSM(QObject *parent = nullptr);
  void start();

  void setTrafficLights(const QList<TrafficLightItem *> &northSouth,
                        const QList<TrafficLightItem *> &eastWest,
                        const QList<TrafficLightItem *> &pedestrian);

private:
  QList<TrafficLightItem *> nsLights;
  QList<TrafficLightItem *> ewLights;
  QList<TrafficLightItem *> pedLights;
  QList<TrafficLightItem *> pedNS;
  QList<TrafficLightItem *> pedEW;

  QList<TrafficState *> states;
  int currentIndex = 0;
  QTimer timer;
  QTimer countdownTimer;
  QTimer pedestrianCheckTimer;

  int secondsRemaining = 0;
  QGraphicsTextItem *countdownLabel = nullptr;

  void setupStates();
  void transitionToNextState();
  void splitPedestrianLights();
  void updateCountdownLabel();
  void checkPedestrianRequestDuringGreen();

private slots:
  void onCountdownTick();
};
