#include "traffic_fsm.h"
#include "qgraphicsscene.h"
#include <QDebug>
#include <QFont>
#include <QTimer>
#include <algorithm>

TrafficFSM::TrafficFSM(QObject *parent) : QObject(parent) {
  connect(&countdownTimer, &QTimer::timeout, this,
          &TrafficFSM::onCountdownTick);
  connect(&pedestrianCheckTimer, &QTimer::timeout, this,
          &TrafficFSM::checkPedestrianRequestDuringGreen);
}

void TrafficFSM::setTrafficLights(const QList<TrafficLightItem *> &northSouth,
                                  const QList<TrafficLightItem *> &eastWest,
                                  const QList<TrafficLightItem *> &pedestrian) {
  nsLights = northSouth;
  ewLights = eastWest;
  pedLights = pedestrian;
  splitPedestrianLights();
  setupStates();

  // Timer until next state and it's position
  if (!countdownLabel) {
    countdownLabel = new QGraphicsTextItem("Timer");
    countdownLabel->setDefaultTextColor(Qt::white);
    countdownLabel->setFont(QFont("Arial", 16, QFont::Bold));
    countdownLabel->setPos(390, 400);
    if (!nsLights.isEmpty())
      nsLights.first()->scene()->addItem(countdownLabel);
  }
}

// logic to combine pedestrians lights into pairs
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

// states logic
void TrafficFSM::setupStates() {
  nsGreen = new QState();
  nsYellow = new QState();
  allRed1 = new QState();
  ewGreen = new QState();
  ewYellow = new QState();
  allRed2 = new QState();

  nsGreen->setProperty("stateName", "NS_Green");
  ewGreen->setProperty("stateName", "EW_Green");

  connect(nsGreen, &QState::entered, [=, this]() {
    for (auto *l : nsLights)
      l->currentColor = Qt::green;
    for (auto *l : ewLights)
      l->currentColor = Qt::red;
    for (auto *l : pedNS)
      l->currentColor = Qt::green;
    for (auto *l : pedEW)
      l->currentColor = Qt::red;
    for (auto *l : nsLights + ewLights + pedNS + pedEW)
      l->update();

    secondsRemaining = 10;
    updateCountdownLabel();
    countdownTimer.start(1000);
    pedestrianCheckTimer.start(250);
    shortenCurrentGreen = false;

    QTimer *tickTimer = new QTimer(this);
    int *ticks = new int(1);
    connect(tickTimer, &QTimer::timeout, this, [=, this]() mutable {
      if (++(*ticks) >= (shortenCurrentGreen ? 5 : 10)) {
        tickTimer->stop();
        emit triggerNSYellow();
      }
    });
    tickTimer->start(1000);
  });

  connect(nsYellow, &QState::entered, [=, this]() {
    for (auto *l : nsLights)
      l->currentColor = Qt::yellow;
    for (auto *l : pedNS)
      l->currentColor = Qt::red;
    for (auto *l : nsLights + pedNS)
      l->update();

    secondsRemaining = 2;
    updateCountdownLabel();
    countdownTimer.start(1000);
    pedestrianCheckTimer.stop();

    QTimer::singleShot(2000, this, [=, this]() { emit triggerAllRed1(); });
  });

  connect(allRed1, &QState::entered, [=, this]() {
    for (auto *l : nsLights)
      l->currentColor = Qt::red;
    for (auto *l : nsLights)
      l->update();

    secondsRemaining = 1;
    updateCountdownLabel();
    countdownTimer.start(1000);

    QTimer::singleShot(1000, this, [=, this]() { emit triggerEWGreen(); });
  });

  connect(ewGreen, &QState::entered, [=, this]() {
    for (auto *l : ewLights)
      l->currentColor = Qt::green;
    for (auto *l : nsLights)
      l->currentColor = Qt::red;
    for (auto *l : pedEW)
      l->currentColor = Qt::green;
    for (auto *l : pedNS)
      l->currentColor = Qt::red;
    for (auto *l : nsLights + ewLights + pedNS + pedEW)
      l->update();

    secondsRemaining = 10;
    updateCountdownLabel();
    countdownTimer.start(1000);
    pedestrianCheckTimer.start(250);
    shortenCurrentGreen = false;

    QTimer *tickTimer = new QTimer(this);
    int *ticks = new int(1);
    connect(tickTimer, &QTimer::timeout, this, [=, this]() mutable {
      if (++(*ticks) >= (shortenCurrentGreen ? 5 : 10)) {
        tickTimer->stop();
        emit triggerEWYellow();
      }
    });
    tickTimer->start(1000);
  });

  connect(ewYellow, &QState::entered, [=, this]() {
    for (auto *l : ewLights)
      l->currentColor = Qt::yellow;
    for (auto *l : pedEW)
      l->currentColor = Qt::red;
    for (auto *l : ewLights + pedEW)
      l->update();

    secondsRemaining = 2;
    updateCountdownLabel();
    countdownTimer.start(1000);
    pedestrianCheckTimer.stop();

    QTimer::singleShot(2000, this, [=, this]() { emit triggerAllRed2(); });
  });

  connect(allRed2, &QState::entered, [=, this]() {
    for (auto *l : ewLights)
      l->currentColor = Qt::red;
    for (auto *l : ewLights)
      l->update();

    secondsRemaining = 1;
    updateCountdownLabel();
    countdownTimer.start(1000);

    QTimer::singleShot(1000, this, [=, this]() { emit triggerNSGreen(); });
  });

  nsGreen->addTransition(this, SIGNAL(triggerNSYellow()), nsYellow);
  nsYellow->addTransition(this, SIGNAL(triggerAllRed1()), allRed1);
  allRed1->addTransition(this, SIGNAL(triggerEWGreen()), ewGreen);
  ewGreen->addTransition(this, SIGNAL(triggerEWYellow()), ewYellow);
  ewYellow->addTransition(this, SIGNAL(triggerAllRed2()), allRed2);
  allRed2->addTransition(this, SIGNAL(triggerNSGreen()), nsGreen);

  machine.addState(nsGreen);
  machine.addState(nsYellow);
  machine.addState(allRed1);
  machine.addState(ewGreen);
  machine.addState(ewYellow);
  machine.addState(allRed2);
  machine.setInitialState(nsGreen);
}

void TrafficFSM::start() { machine.start(); }

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

  auto handleRequest = [&](QList<TrafficLightItem *> &redPed,
                           QList<TrafficLightItem *> &greenPed) {
    bool requested =
        std::any_of(redPed.begin(), redPed.end(),
                    [](TrafficLightItem *l) { return l->buttonPressed; });

    bool isRed =
        std::all_of(redPed.begin(), redPed.end(), [](TrafficLightItem *l) {
          return l->currentColor == Qt::red;
        });

    if (requested && isRed && secondsRemaining > 5) {
      shortenCurrentGreen = true;
      secondsRemaining = 5;
      countdownTimer.stop();
      countdownTimer.start(1000);
      updateCountdownLabel();
      pedestrianCheckTimer.stop();
      for (auto *l : redPed)
        l->buttonPressed = false;
    }
  };

  if (stateName == "NS_Green")
    handleRequest(pedEW, pedNS);
  if (stateName == "EW_Green")
    handleRequest(pedNS, pedEW);
}
