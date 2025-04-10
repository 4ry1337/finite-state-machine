#include "traffic_light_state.h"
#include "traffic_light.h"
#include <chrono>

// GreenState implementation.
void GreenState::enter(TrafficLight *light) {
  std::cout << "Entering Green state" << std::endl;
}

void GreenState::execute(TrafficLight *light) {
  static auto startTime = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::steady_clock::now() - startTime;
  // Green lasts 5 seconds.
  if (elapsed > std::chrono::seconds(5)) {
    // If a pedestrian is waiting, transition directly to Red.
    if (light->isPedestrianWaiting()) {
      light->changeState(std::make_unique<RedState>());
      light->resetPedestrianRequest();
    } else {
      light->changeState(std::make_unique<YellowState>());
    }
    startTime = std::chrono::steady_clock::now();
  }
}

void GreenState::exit(TrafficLight *light) {
  std::cout << "Exiting Green state" << std::endl;
}
