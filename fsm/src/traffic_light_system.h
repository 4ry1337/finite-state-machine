#ifndef TRAFFIC_LIGHT_SYSTEM_H
#define TRAFFIC_LIGHT_SYSTEM_H

#include "traffic_light.h"
#include "traffic_light_state.h"
#include <array>
#include <atomic>
#include <thread>

class TrafficLightSystem {
public:
  TrafficLight light;
  std::atomic<bool> running{false};
  std::thread worker;

  TrafficLightSystem(const std::array<std::chrono::milliseconds, 4> &config)
      : light(config) {}

  void start() {
    running = true;
    light.log("Traffic light system starting.");
    light.changeState(std::make_unique<GreenState>());
    worker = std::thread([this]() { run(); });
  }

  // Stop the system.
  void stop() {
    running = false;
    if (worker.joinable()) {
      worker.join();
    }
    light.log("Traffic light system stopped.");
  }

private:
  void run() {
    while (running) {
      if (light.currentState) {
        light.currentState->update(light);
      }
      // Sleep briefly to avoid busy waiting.
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
};

#endif // TRAFFIC_LIGHT_SYSTEM_H
