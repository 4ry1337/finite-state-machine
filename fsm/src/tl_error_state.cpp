#include "logger.h"
#include "traffic_light.h"
#include "traffic_light_state.h"
#include <cstdlib>

void ErrorState::enter(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::ERROR, "Entering state: " + to_string());
}

void ErrorState::exit(TrafficLight &light) {
  light.logger->log(Logger::LogLevel::ERROR, "Exiting state: " + to_string());
}

void ErrorState::update(TrafficLight &light,
                        std::chrono::milliseconds /* duration */) {
  light.logger->log(Logger::LogLevel::ERROR, "Traffic light in ERROR state");
}

std::string ErrorState::to_string() const { return "ERROR"; }
