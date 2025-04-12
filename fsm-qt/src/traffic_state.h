#ifndef TRAFFIC_STATE_H
#define TRAFFIC_STATE_H

#pragma once

#include <QObject>
#include <QTimer>
#include <QColor>
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
