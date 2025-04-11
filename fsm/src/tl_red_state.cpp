#include "logger.h"
#include "traffic_light.h"
#include "traffic_light_state.h"
#include <cstdlib>

void RedState::enter(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::INFO, "Entering state: " + to_string());
}

void RedState::exit(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::INFO, "Exiting state: " + to_string());
}

void RedState::update(TrafficLight &light, std::chrono::milliseconds elapsed) {
  if (elapsed >= light.config[RED]) { // Red duration
    light.changeState(std::make_unique<GreenState>());
  }
}

std::string RedState::to_string() const { return "RED"; }
