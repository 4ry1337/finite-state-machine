#pragma once

#include "traffic_light_item.h"
#include <QGraphicsTextItem>
#include <QFinalState>
#include <QSignalTransition>
#include <QState>
#include <QStateMachine>
#include <QTimer>
#include <QObject>

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

    QStateMachine machine;
    QTimer countdownTimer;
    QTimer pedestrianCheckTimer;
    QTimer stateTimer;
    QGraphicsTextItem *countdownLabel = nullptr;
    int secondsRemaining = 0;

    void splitPedestrianLights();
    void updateCountdownLabel();
    void setupStates();

private slots:
    void onCountdownTick();
    void checkPedestrianRequestDuringGreen();
};
