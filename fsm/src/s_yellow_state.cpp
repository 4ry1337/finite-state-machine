#include "traffic_light.h"
#include "traffic_light_state.h"
#include <chrono>

void YellowState::enter(TrafficLight &light) {
  light.log("Entering Yellow State.");
}

void YellowState::exit(TrafficLight &light) {
  light.log("Exiting Yellow State.");
}

void YellowState::update(TrafficLight &light) {
  static auto startTime = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::steady_clock::now() - startTime;
  if (elapsed >= light.config[1]) {
    light.log("Yellow duration complete, transitioning to Red.");
    startTime = std::chrono::steady_clock::now();
    light.changeState(std::make_unique<RedState>());
  }
}
