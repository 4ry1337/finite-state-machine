#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "logger.h"
#include "traffic_light_state.h"
#include <array>
#include <chrono>
#include <memory>

#define GREEN 0
#define YELLOW 1
#define RED 2
#define MINGREEN 3

class TrafficLight {
public:
  // Configuration parameters:
  // [0] Green duration, [1] Yellow duration,
  // [2] Red duration, [3] Minimum green duration.
  const std::array<std::chrono::milliseconds, 4> config;

  bool pedestrianRequested{false};
  std::unique_ptr<TrafficLightState> currentState;
  std::shared_ptr<Logger> logger;

  TrafficLight(const std::array<std::chrono::milliseconds, 4> &cfg)
      : config(cfg) {}

  void changeState(std::unique_ptr<TrafficLightState> newState);
  void initialize(std::shared_ptr<Logger> logger);
  void requestPedestrian();
  void run();
};

#endif // TRAFFIC_LIGHT_H
