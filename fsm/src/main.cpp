#include "traffic_light.h"
#include <chrono>
#include <thread>

int main() {
  TrafficLight trafficLight(std::make_unique<RedState>());

  while (true) {
    trafficLight.update();
  }

  return 0;
}
