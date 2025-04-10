#ifndef TRAFFIC_LIGHT_STATE_H
#define TRAFFIC_LIGHT_STATE_H

#include <memory>

class TrafficLight; // Forward declaration

class TrafficLightState {
public:
  virtual ~TrafficLightState() = default;
  virtual void enter(TrafficLight &light) = 0;
  virtual void exit(TrafficLight &light) = 0;
  virtual void update(TrafficLight &light) = 0;
};

class RedState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light) override;
};

class YellowState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light) override;
};

class GreenState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light) override;
};

class ErrorState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light) override;
};

#endif // TRAFFIC_LIGHT_STATE_H
