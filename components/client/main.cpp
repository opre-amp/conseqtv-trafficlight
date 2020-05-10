//#include "mainwindow.h"

#include <QApplication>
#include <QtQuick>
#include <QQmlApplicationEngine>


#include <QGuiApplication>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtQml>

#include "messageTableModel.h"
#include "settings.h"
#include "user.h"
#include "screenProperty.h"

static void connectToDatabase()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
    }

    const QDir writeDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!writeDir.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));

    // Ensure that we have a writable location on all devices.
    const QString fileName = writeDir.absolutePath() + "/message-database.sqlite3";
    // When using the SQLite driver, open() will create the SQLite database if it doesn't exist.
    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    qmlRegisterType<messageTableModel>("tablemodel", 1, 0, "MessageTableModel");
    qmlRegisterType<loggedUser>("userdata", 1, 0, "LoggedUser");
    qmlRegisterType<delaySettings>("delaysettings", 1, 0, "DelaySettings");

    connectToDatabase();

    loggedUser user;
    delaySettings settings;
//    screenProperty screen;


    QQuickView view;
    view.setSource(QUrl("qrc:/screenLoader.qml"));
    view.show();

    return a.exec();

}
