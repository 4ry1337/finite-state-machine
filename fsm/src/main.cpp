#include "logger.h"
#include "traffic_light.h"
#include <chrono>
#include <iostream>
#include <thread>
// <thread> is no longer needed since threading has been removed.

using namespace std::chrono_literals;

// The user input handler is retained for reference but not launched as a
// separate thread. In a single-threaded version, simultaneous input handling
// and simulation cannot run concurrently. You can choose to call this function
// at an appropriate point, or remove it entirely.
void userInputHandler(TrafficLight &trafficLight) {
  std::cout << "Press 'p' to request pedestrian crossing, 'q' to quit:"
            << std::endl;
  char input;
  while (true) {
    std::cin >> input;
    if (input == 'p' || input == 'P') {
      trafficLight.requestPedestrian();
      std::cout << "Pedestrian request sent!" << std::endl;
    } else if (input == 'q' || input == 'Q') {
      std::cout << "Exiting program..." << std::endl;
      std::exit(0);
    }
  }
}

int main() {
  try {
    auto logger = std::make_shared<ConsoleLogger>();

    std::array<std::chrono::milliseconds, 4> config = {
        5000ms, // Green duration (5 seconds, test value)
        2000ms, // Yellow duration (2 seconds, test value)
        3000ms, // Red duration (3 seconds, test value)
        2000ms  // Minimum green duration (2 seconds, test value)
    };

    TrafficLight trafficLight(config);
    trafficLight.initialize(logger);

    std::thread inputThread(userInputHandler, std::ref(trafficLight));
    inputThread.detach(); // Run concurrently with the simulation.

    trafficLight.run();
  } catch (const std::exception &e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
