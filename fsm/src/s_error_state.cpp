#include "traffic_light.h"
#include "traffic_light_state.h"

// ErrorState implementation
void ErrorState::enter(TrafficLight &light) {
  light.log("Entering Error State.");
}

void ErrorState::exit(TrafficLight &light) {
  light.log("Exiting Error State.");
}

void ErrorState::update(TrafficLight &light) {
  light.log("Error State active. Attempting recovery...");
  // You could transition out of ErrorState here if conditions improve.
}
