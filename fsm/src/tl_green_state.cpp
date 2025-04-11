#include "traffic_light.h"
#include "traffic_light_state.h"
#include <cstdlib>

void GreenState::enter(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::INFO, "Entering state: " + to_string());
}

void GreenState::exit(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::INFO, "Exiting state: " + to_string());
}

void GreenState::update(TrafficLight &light,
                        std::chrono::milliseconds elapsed) {
  if (elapsed >= light.config[GREEN] ||
      (light.pedestrianRequested && elapsed >= light.config[MINGREEN])) {
    if (light.pedestrianRequested) {
      light.logger->log(Logger::LogLevel::INFO,
                        "Pedestrian request acknowledged");
    }

    light.changeState(std::make_unique<YellowState>());
  }
}
std::string GreenState::to_string() const { return "GREEN"; }
