//#include "mainwindow.h"

#include <QApplication>
#include <QtQuick>
#include <QQmlApplicationEngine>


#include <QGuiApplication>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtQml>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQuickView view;
    view.setSource(QUrl("qrc:/screenLoader.qml"));
    QObject::connect(view.engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
    view.showFullScreen();
    return a.exec();
}
