#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "traffic_light_state.h"
#include <array>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

class TrafficLight {
public:
  // Array holding configuration parameters:
  // [0] Green duration, [1] Yellow duration,
  // [2] Red duration, [3] Minimum green duration.
  const std::array<std::chrono::milliseconds, 4> config;
  std::atomic<bool> pedestrianRequested{false};
  std::unique_ptr<TrafficLightState> currentState;
  // Mutex for thread-safe state transitions.
  std::mutex stateMutex;

  TrafficLight(const std::array<std::chrono::milliseconds, 4> &cfg)
      : config(cfg) {}

  void log(const std::string &message) {
    std::cout << "["
              << std::chrono::system_clock::to_time_t(
                     std::chrono::system_clock::now())
              << "] " << message << std::endl;
  }

  // Change the state safely by calling exit and then enter on the new state.
  void changeState(std::unique_ptr<TrafficLightState> newState) {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (currentState) {
      currentState->exit(*this);
    }
    currentState = std::move(newState);
    if (currentState) {
      currentState->enter(*this);
    }
  }
};

#endif // TRAFFIC_LIGHT_H
