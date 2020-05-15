QT       += core gui
QT       += quick
QT       += widgets
QT       += qml
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TEMPLATE += app

SOURCES += main.cpp 

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml/qmlresources.qrc \
    scripts/scriptresources.qrc \
    img/imgresources.qrc
