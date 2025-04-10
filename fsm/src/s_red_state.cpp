#include "traffic_light.h"
#include "traffic_light_state.h"
#include <iostream>

void RedState::enter(TrafficLight &light) {
  std::cout << "Entering Red State" << std::endl;
}

void RedState::exit(TrafficLight &light) {
  std::cout << "Exiting Red State" << std::endl;
}

void RedState::update(TrafficLight &light) {
  light.changeState(std::make_unique<GreenState>());
}
