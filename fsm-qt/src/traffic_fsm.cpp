#include "traffic_fsm.h"
#include "qfont.h"
#include "qgraphicsscene.h"

TrafficFSM::TrafficFSM(QObject *parent) : QObject(parent) {
  connect(&timer, &QTimer::timeout, this, &TrafficFSM::transitionToNextState);
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
  if (!countdownLabel) {
    countdownLabel = new QGraphicsTextItem("Timer");
    countdownLabel->setDefaultTextColor(Qt::white);
    countdownLabel->setFont(QFont("Arial", 16, QFont::Bold));
    countdownLabel->setPos(390, 400); // adjust position to fit your scene
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
      pedNS.append(light); // crossing NS road
    else
      pedEW.append(light); // crossing EW road
  }
}

void TrafficFSM::setupStates() {
  states.clear();

  states.append(new TrafficState("NS_Green", 10000, [=]() {
    for (auto *light : nsLights) {
      light->currentColor = Qt::green;
      light->update();
    }
    for (auto *light : ewLights) {
      light->currentColor = Qt::red;
      light->update();
    }

    // Left/Right pedestrian lights go green (crossing EW)
    for (auto *light : pedNS) {
      light->currentColor = Qt::green;
      light->update();
    }
    for (auto *light : pedEW) {
      light->currentColor = Qt::red;
      light->update();
    }
  }));

  states.append(new TrafficState("NS_Yellow", 2000, [=]() {
    for (auto *light : nsLights) {
      light->currentColor = Qt::yellow;
      light->update();
    }
    for (auto *light : pedNS) {
      light->currentColor = Qt::red;
      light->update();
    } // yellow = stop crossing
  }));

  states.append(new TrafficState("All_Red1", 1000, [=]() {
    for (auto *light : nsLights) {
      light->currentColor = Qt::red;
      light->update();
    }
  }));

  states.append(new TrafficState("EW_Green", 10000, [=]() {
    for (auto *light : ewLights) {
      light->currentColor = Qt::green;
      light->update();
    }
    for (auto *light : nsLights) {
      light->currentColor = Qt::red;
      light->update();
    }

    // Top/Bottom pedestrian lights go green (crossing NS)
    for (auto *light : pedEW) {
      light->currentColor = Qt::green;
      light->update();
    }
    for (auto *light : pedNS) {
      light->currentColor = Qt::red;
      light->update();
    }
  }));

  states.append(new TrafficState("EW_Yellow", 2000, [=]() {
    for (auto *light : ewLights) {
      light->currentColor = Qt::yellow;
      light->update();
    }
    for (auto *light : pedEW) {
      light->currentColor = Qt::red;
      light->update();
    }
  }));

  states.append(new TrafficState("All_Red2", 1000, [=]() {
    for (auto *light : ewLights) {
      light->currentColor = Qt::red;
      light->update();
    }
  }));
}

void TrafficFSM::start() {
  if (states.isEmpty())
    return;

  currentIndex = 0;
  states[currentIndex]->onEnter();

  if (states[currentIndex]->name == "NS_Green" ||
      states[currentIndex]->name == "EW_Green") {
    pedestrianCheckTimer.start(250);
  }

  int duration = states[currentIndex]->duration;

  secondsRemaining = duration / 1000;
  updateCountdownLabel();

  timer.start(duration);
  countdownTimer.start(1000);
}

void TrafficFSM::transitionToNextState() {
  currentIndex = (currentIndex + 1) % states.size();
  auto *nextState = states[currentIndex];
  nextState->onEnter();

  int duration = nextState->duration;

  // Check for pedestrian request shortening
  if (nextState->name == "NS_Green") {
    bool requested =
        std::any_of(pedEW.begin(), pedEW.end(),
                    [](TrafficLightItem *l) { return l->buttonPressed; });
    if (requested) {
      qDebug() << "Pedestrian requested EW crossing. Reducing NS green time.";
      duration = std::min(duration, 5000); // shorten to 5s max
      for (auto *l : pedEW)
        l->buttonPressed = false;
    }
  }

  if (nextState->name == "EW_Green") {
    bool requested =
        std::any_of(pedNS.begin(), pedNS.end(),
                    [](TrafficLightItem *l) { return l->buttonPressed; });
    if (requested) {
      qDebug() << "Pedestrian requested NS crossing. Reducing EW green time.";
      duration = std::min(duration, 5000);
      for (auto *l : pedNS)
        l->buttonPressed = false;
    }
  }

  pedestrianCheckTimer.stop(); // Always stop it first
  if (nextState->name == "NS_Green" || nextState->name == "EW_Green") {
    pedestrianCheckTimer.start(250); // check 4x per sec for immediate response
  }

  // Start FSM timer
  timer.start(duration);
  countdownTimer.stop(); // just in case
  secondsRemaining = duration / 1000;
  updateCountdownLabel();
  countdownTimer.start(1000);
}

void TrafficFSM::updateCountdownLabel() {
  if (!countdownLabel)
    return;

  countdownLabel->setPlainText(QString::number(secondsRemaining));
}

void TrafficFSM::onCountdownTick() {
  if (secondsRemaining <= 0) {
    countdownTimer.stop();
    return;
  }

  secondsRemaining--;
  updateCountdownLabel();
}

void TrafficFSM::checkPedestrianRequestDuringGreen() {
  if (states[currentIndex]->name == "NS_Green") {
    bool requested =
        std::any_of(pedEW.begin(), pedEW.end(),
                    [](TrafficLightItem *l) { return l->buttonPressed; });

    if (requested && secondsRemaining > 5) {
      qDebug() << "Pedestrian requested EW crossing mid-cycle. Reducing NS "
                  "green duration.";
      secondsRemaining = 5;
      updateCountdownLabel();
      timer.start(5000);
      countdownTimer.start(1000); // reset countdown
      for (auto *l : pedEW)
        l->buttonPressed = false;
      pedestrianCheckTimer.stop();
    }

  } else if (states[currentIndex]->name == "EW_Green") {
    bool requested =
        std::any_of(pedNS.begin(), pedNS.end(),
                    [](TrafficLightItem *l) { return l->buttonPressed; });

    if (requested && secondsRemaining > 5) {
      qDebug() << "Pedestrian requested NS crossing mid-cycle. Reducing EW "
                  "green duration.";
      secondsRemaining = 5;
      updateCountdownLabel();
      timer.start(5000);
      countdownTimer.start(1000); // reset countdown
      for (auto *l : pedNS)
        l->buttonPressed = false;
      pedestrianCheckTimer.stop();
    }
  }
}
