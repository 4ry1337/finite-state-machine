#include "traffic_light.h"
#include "traffic_light_state.h"
#include <iostream>

void YellowState::enter(TrafficLight &light) {
  std::cout << "Entering Yellow State" << std::endl;
}

void YellowState::exit(TrafficLight &light) {
  std::cout << "Exiting Yellow State" << std::endl;
}

void YellowState::update(TrafficLight &light) {
  light.changeState(std::make_unique<RedState>());
}
