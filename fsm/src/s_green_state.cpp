#include "traffic_light.h"
#include "traffic_light_state.h"
#include <iostream>

void GreenState::enter(TrafficLight &light) {
  std::cout << "Entering Green State" << std::endl;
}

void GreenState::exit(TrafficLight &light) {
  std::cout << "Exiting Green State" << std::endl;
}

void GreenState::update(TrafficLight &light) {
  light.changeState(std::make_unique<YellowState>());
}
