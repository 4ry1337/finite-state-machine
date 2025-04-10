#include <QApplication>
#include <QGraphicsView>
#include "traffic_scene.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QGraphicsView view;
    auto *scene = new TrafficScene();
    view.setScene(scene);
    view.setRenderHint(QPainter::Antialiasing);
    view.setWindowTitle("Traffic Light FSM");
    view.resize(800, 600);
    view.show();

    return app.exec();
}
