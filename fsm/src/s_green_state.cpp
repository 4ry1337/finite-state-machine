#include "traffic_light.h"
#include "traffic_light_state.h"
#include <chrono>

// GreenState implementation
void GreenState::enter(TrafficLight &light) {
  light.log("Entering Green State.");
  // Reset timer for GreenState.
}

void GreenState::exit(TrafficLight &light) {
  light.log("Exiting Green State.");
}

void GreenState::update(TrafficLight &light) {
  static auto startTime = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::steady_clock::now() - startTime;

  // If a pedestrian request is active and the minimum green duration has
  // passed...
  if (light.pedestrianRequested && (elapsed >= light.config[3])) {
    light.log("Pedestrian request detected, transitioning to Yellow.");
    startTime = std::chrono::steady_clock::now();
    light.changeState(std::make_unique<YellowState>());
  }
  // Otherwise, if the full green duration has elapsed, transition normally.
  else if (elapsed >= light.config[0]) {
    light.log("Green duration complete, transitioning to Yellow.");
    startTime = std::chrono::steady_clock::now();
    light.changeState(std::make_unique<YellowState>());
  }
}
