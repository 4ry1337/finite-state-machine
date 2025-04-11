#include "traffic_light.h"
#include "traffic_light_state.h"
#include <cstdlib>

void YellowState::enter(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::INFO, "Entering state: " + to_string());
}

void YellowState::exit(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::INFO, "Exiting state: " + to_string());
}

void YellowState::update(TrafficLight &light,
                         std::chrono::milliseconds elapsed) {
  if (elapsed >= light.config[YELLOW]) { // Yellow duration
    light.changeState(std::make_unique<RedState>());
  }
}

std::string YellowState::to_string() const { return "YELLOW"; }
