#include "traffic_light.h"
#include "traffic_light_state.h"
#include <chrono>
#include <thread>

void TrafficLight::changeState(std::unique_ptr<TrafficLightState> newState) {
  if (currentState)
    currentState->exit(*this);

  currentState = std::move(newState);

  if (currentState)
    currentState->enter(*this);
}

void TrafficLight::requestPedestrian() {
  if (currentState && dynamic_cast<GreenState *>(currentState.get())) {
    logger->log(Logger::LogLevel::INFO,
                "Pedestrian request received during GREEN state");
    pedestrianRequested = true;
  } else {
    logger->log(Logger::LogLevel::INFO,
                "Pedestrian request received (not in GREEN state)");
    pedestrianRequested = true;
  }
}

void TrafficLight::initialize(std::shared_ptr<Logger> log) {
  logger = log;

  if (!logger) {
    logger = std::make_shared<ConsoleLogger>();
  }

  // Validate configuration
  for (const auto &duration : config) {
    if (duration.count() <= 0 &&
        &duration != &config[3]) { // Allow min green duration to be 0
      logger->log(Logger::LogLevel::ERROR,
                  "Invalid traffic light configuration");
      changeState(std::make_unique<ErrorState>());
      return;
    }
  }

  currentState = std::make_unique<RedState>();
}

void TrafficLight::run() {
  if (!currentState) {
    logger->log(Logger::LogLevel::ERROR, "Traffic light not initialized");
    return;
  }

  logger->log(Logger::LogLevel::INFO, "Traffic light system starting");

  try {
    auto startTime = std::chrono::steady_clock::now();

    while (true) {
      auto currentTime = std::chrono::steady_clock::now();
      auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
          currentTime - startTime);

      auto previousState = currentState.get();

      // Update current state.
      currentState->update(*this, elapsedTime);

      // If the state was changed, reset the timer.
      if (previousState != currentState.get()) {
        startTime = std::chrono::steady_clock::now();
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  } catch (const std::exception &e) {
    logger->log(Logger::LogLevel::ERROR,
                "Exception in traffic light: " + std::string(e.what()));
    changeState(std::make_unique<ErrorState>());
  }
}
