#include "traffic_fsm.h"
#include "qgraphicsscene.h"
#include <QFont>
#include <algorithm>

TrafficFSM::TrafficFSM(QObject *parent) : QObject(parent) {
    connect(&countdownTimer, &QTimer::timeout, this, &TrafficFSM::onCountdownTick);
    connect(&pedestrianCheckTimer, &QTimer::timeout, this, &TrafficFSM::checkPedestrianRequestDuringGreen);
}

void TrafficFSM::setTrafficLights(const QList<TrafficLightItem *> &northSouth,
                                  const QList<TrafficLightItem *> &eastWest,
                                  const QList<TrafficLightItem *> &pedestrian) {
    nsLights = northSouth;
    ewLights = eastWest;
    pedLights = pedestrian;
    splitPedestrianLights();
    setupStates();

    if (!countdownLabel) {
        countdownLabel = new QGraphicsTextItem("Timer");
        countdownLabel->setDefaultTextColor(Qt::white);
        countdownLabel->setFont(QFont("Arial", 16, QFont::Bold));
        countdownLabel->setPos(390, 400);
        if (!nsLights.isEmpty())
            nsLights.first()->scene()->addItem(countdownLabel);
    }
}

void TrafficFSM::splitPedestrianLights() {
    pedNS.clear();
    pedEW.clear();
    for (auto *light : pedLights) {
        QPointF pos = light->pos();
        if (pos.x() < 300 || pos.x() > 500)
            pedNS.append(light);
        else
            pedEW.append(light);
    }
}

void TrafficFSM::setupStates() {
    auto *nsGreen = new QState();
    auto *nsYellow = new QState();
    auto *allRed1 = new QState();
    auto *ewGreen = new QState();
    auto *ewYellow = new QState();
    auto *allRed2 = new QState();

    nsGreen->assignProperty(this, "stateName", "NS_Green");
    connect(nsGreen, &QState::entered, [=]() {
        for (auto *l : nsLights) l->currentColor = Qt::green;
        for (auto *l : ewLights) l->currentColor = Qt::red;
        for (auto *l : pedNS) l->currentColor = Qt::green;
        for (auto *l : pedEW) l->currentColor = Qt::red;
        for (auto *l : nsLights + ewLights + pedNS + pedEW) l->update();

        secondsRemaining = 10;
        updateCountdownLabel();
        countdownTimer.start(1000);
        stateTimer.start(10000);
        pedestrianCheckTimer.start(250);
    });

    connect(nsYellow, &QState::entered, [=]() {
        for (auto *l : nsLights) l->currentColor = Qt::yellow;
        for (auto *l : pedNS) l->currentColor = Qt::red;
        for (auto *l : nsLights + pedNS) l->update();

        secondsRemaining = 2;
        updateCountdownLabel();
        countdownTimer.start(1000);
        stateTimer.start(2000);
        pedestrianCheckTimer.stop();
    });

    connect(allRed1, &QState::entered, [=]() {
        for (auto *l : nsLights) l->currentColor = Qt::red;
        for (auto *l : nsLights) l->update();

        secondsRemaining = 1;
        updateCountdownLabel();
        countdownTimer.start(1000);
        stateTimer.start(1000);
    });

    connect(ewGreen, &QState::entered, [=]() {
        for (auto *l : ewLights) l->currentColor = Qt::green;
        for (auto *l : nsLights) l->currentColor = Qt::red;
        for (auto *l : pedEW) l->currentColor = Qt::green;
        for (auto *l : pedNS) l->currentColor = Qt::red;
        for (auto *l : nsLights + ewLights + pedNS + pedEW) l->update();

        secondsRemaining = 10;
        updateCountdownLabel();
        countdownTimer.start(1000);
        stateTimer.start(10000);
        pedestrianCheckTimer.start(250);
    });

    connect(ewYellow, &QState::entered, [=]() {
        for (auto *l : ewLights) l->currentColor = Qt::yellow;
        for (auto *l : pedEW) l->currentColor = Qt::red;
        for (auto *l : ewLights + pedEW) l->update();

        secondsRemaining = 2;
        updateCountdownLabel();
        countdownTimer.start(1000);
        stateTimer.start(2000);
        pedestrianCheckTimer.stop();
    });

    connect(allRed2, &QState::entered, [=]() {
        for (auto *l : ewLights) l->currentColor = Qt::red;
        for (auto *l : ewLights) l->update();

        secondsRemaining = 1;
        updateCountdownLabel();
        countdownTimer.start(1000);
        stateTimer.start(1000);
    });

    nsGreen->addTransition(&stateTimer, &QTimer::timeout, nsYellow);
    nsYellow->addTransition(&stateTimer, &QTimer::timeout, allRed1);
    allRed1->addTransition(&stateTimer, &QTimer::timeout, ewGreen);
    ewGreen->addTransition(&stateTimer, &QTimer::timeout, ewYellow);
    ewYellow->addTransition(&stateTimer, &QTimer::timeout, allRed2);
    allRed2->addTransition(&stateTimer, &QTimer::timeout, nsGreen);

    machine.addState(nsGreen);
    machine.addState(nsYellow);
    machine.addState(allRed1);
    machine.addState(ewGreen);
    machine.addState(ewYellow);
    machine.addState(allRed2);
    machine.setInitialState(nsGreen);
}

void TrafficFSM::start() {
    machine.start();
}

void TrafficFSM::onCountdownTick() {
    secondsRemaining--;
    updateCountdownLabel();
    if (secondsRemaining <= 0)
        countdownTimer.stop();
}

void TrafficFSM::updateCountdownLabel() {
    if (countdownLabel)
        countdownLabel->setPlainText(QString::number(secondsRemaining));
}

void TrafficFSM::checkPedestrianRequestDuringGreen() {
    auto state = *machine.configuration().begin();
    QString stateName = state->property("stateName").toString();

    auto reduceIfRequested = [&](QList<TrafficLightItem *> &oppositeLights, QList<TrafficLightItem *> &currentLights) {
        bool requested = std::any_of(oppositeLights.begin(), oppositeLights.end(), [](TrafficLightItem *l) {
            return l->buttonPressed;
        });
        bool isCurrentRed = std::all_of(oppositeLights.begin(), oppositeLights.end(), [](TrafficLightItem *l) {
            return l->currentColor == Qt::red;
        });
        if (requested && isCurrentRed && secondsRemaining > 5) {
            secondsRemaining = 5;
            stateTimer.stop();
            stateTimer.start(5000);
            countdownTimer.stop();
            countdownTimer.start(1000);
            updateCountdownLabel();
            for (auto *l : oppositeLights) l->buttonPressed = false;
            pedestrianCheckTimer.stop();
        }
    };

    if (stateName == "NS_Green") reduceIfRequested(pedEW, pedNS);
    if (stateName == "EW_Green") reduceIfRequested(pedNS, pedEW);
}
