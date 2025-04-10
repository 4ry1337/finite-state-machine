#include "traffic_light_system.h"
#include <array>
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  std::array<std::chrono::milliseconds, 4> config = {
      std::chrono::milliseconds(30000), // Green: 30 seconds.
      std::chrono::milliseconds(5000),  // Yellow: 5 seconds.
      std::chrono::milliseconds(30000), // Red: 30 seconds.
      std::chrono::milliseconds(10000)  // Minimum Green Duration: 10 seconds.
  };

  // Create the traffic light system with the configuration.
  TrafficLightSystem system(config);
  system.start();

  // Simulate a pedestrian request after 15 seconds.
  std::this_thread::sleep_for(std::chrono::seconds(15));
  system.light.log("Pedestrian button pressed.");
  system.light.pedestrianRequested = true;

  // Let the system run for an additional 90 seconds.
  std::this_thread::sleep_for(std::chrono::seconds(90));
  system.stop();

  return 0;
}
