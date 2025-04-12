#include "traffic_state.h"

TrafficState::TrafficState(QString name, int duration, std::function<void()> onEnter)
    : name(name), duration(duration), onEnter(onEnter) {}
