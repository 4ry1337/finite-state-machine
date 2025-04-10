#ifndef FSM_H
#define FSM_H

#include <QMainWindow>

enum TrafficLightState { GREEN, YELLOW, RED };

class fsm : public QMainWindow {
  Q_OBJECT

public:
  fsm(QWidget *parent = nullptr);
  ~fsm();
};

#endif // FSM_H
