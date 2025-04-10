#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "traffic_light_state.h"
#include <memory>

class TrafficLight {
public:
  TrafficLight(std::unique_ptr<TrafficLightState> initState)
      : currentState(std::move(initState)) {
    currentState->enter(*this);
  }

  void changeState(std::unique_ptr<TrafficLightState> newState) {
    if (currentState) {
      currentState->exit(*this);
    }
    currentState = std::move(newState);
    if (currentState) {
      currentState->enter(*this);
    }
  }

  void update() {
    if (currentState) {
      currentState->update(*this);
    }
  }

private:
  std::unique_ptr<TrafficLightState> currentState;
};

#endif // TRAFFIC_LIGHT_H
