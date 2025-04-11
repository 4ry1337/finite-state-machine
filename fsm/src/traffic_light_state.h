#ifndef TRAFFIC_LIGHT_STATE_H
#define TRAFFIC_LIGHT_STATE_H

#include <chrono>
#include <memory>
#include <string>

class TrafficLight; // Forward declaration

class TrafficLightState {
public:
  virtual ~TrafficLightState() = default;
  virtual void enter(TrafficLight &light) = 0;
  virtual void exit(TrafficLight &light) = 0;
  virtual void update(TrafficLight &light,
                      std::chrono::milliseconds duration) = 0;
  virtual std::string to_string() const = 0;
};

class RedState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light, std::chrono::milliseconds duration) override;
  std::string to_string() const override;
};

class YellowState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light, std::chrono::milliseconds duration) override;
  std::string to_string() const override;
};

class GreenState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light, std::chrono::milliseconds duration) override;
  std::string to_string() const override;
};

class ErrorState : public TrafficLightState {
public:
  void enter(TrafficLight &light) override;
  void exit(TrafficLight &light) override;
  void update(TrafficLight &light, std::chrono::milliseconds duration) override;
  std::string to_string() const override;
};

#endif // TRAFFIC_LIGHT_STATE_H
