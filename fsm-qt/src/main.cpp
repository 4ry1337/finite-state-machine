#include "fsm.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  fsm window;
  window.show();
  return 0;
}
