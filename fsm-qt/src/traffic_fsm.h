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

    QEvent *nextEvent = new QEvent(QEvent::Type(QEvent::User + 1));
    QState *nsGreen, *nsYellow, *allRed1, *ewGreen, *ewYellow, *allRed2;

    bool shortenCurrentGreen = false;  // NEW

    int secondsRemaining = 0;
    int currentStateDuration = 10000;
    int defaultGreenDuration = 10000;
    int overriddenGreenDuration = -1;

    void splitPedestrianLights();
    void updateCountdownLabel();
    void setupStates();

private slots:
    void onCountdownTick();
    void checkPedestrianRequestDuringGreen();

signals:
    void triggerNSYellow();
    void triggerAllRed1();
    void triggerEWGreen();
    void triggerEWYellow();
    void triggerAllRed2();
    void triggerNSGreen();


};
