QT       += core gui #statemachine
TEMPLATE = app
TARGET = fsm
INCLUDEPATH += .
CONFIG += c++20

SOURCES += $$files(src/*.cpp, true) \

HEADERS += $$files(src/*.h, true) \

TRANSLATIONS += translations/en_US.ts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
