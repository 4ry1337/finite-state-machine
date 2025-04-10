#include "traffic_light.h"
#include "traffic_light_state.h"
#include <chrono>

// RedState implementation
void RedState::enter(TrafficLight &light) {
  light.log("Entering Red State.");
  // Reset pedestrian request when red is active.
  light.pedestrianRequested = false;
}

void RedState::exit(TrafficLight &light) { light.log("Exiting Red State."); }

void RedState::update(TrafficLight &light) {
  static auto startTime = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::steady_clock::now() - startTime;
  if (elapsed >= light.config[2]) {
    light.log("Red duration complete, transitioning to Green.");
    startTime = std::chrono::steady_clock::now();
    // Transition to GreenState.
    light.changeState(std::make_unique<GreenState>());
  }
}
