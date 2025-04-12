#ifndef TRAFFIC_STATE_H
#define TRAFFIC_STATE_H

#pragma once

#include <QColor>
#include <QObject>
#include <QTimer>
#include <functional>

class TrafficState : public QObject {
  Q_OBJECT
public:
  TrafficState(QString name, int duration, std::function<void()> onEnter);

  QString name;
  int duration; // in ms
  std::function<void()> onEnter;
};

#endif // TRAFFIC_STATE_H
