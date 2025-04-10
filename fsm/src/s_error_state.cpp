
#include "traffic_light.h"
#include "traffic_light_state.h"
#include <iostream>

void ErrorState::enter(TrafficLight &light) {
  std::cout << "Entering Error State" << std::endl;
}

void ErrorState::exit(TrafficLight &light) {
  std::cout << "Exiting Error State" << std::endl;
}

void ErrorState::update(TrafficLight &light) {
  light.changeState(std::make_unique<RedState>());
}
