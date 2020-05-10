#ifndef SETTINGS_H
#define SETTINGS_H

#include <QNetworkAccessManager>
#include <QObject>

class delaySettings : public QObject
{
    Q_OBJECT

public:
    delaySettings();

public slots:
    void setTime(QString timer, int value);
    int getTime(int t);
    void upload();

private:   
    int timeset[9];
    QNetworkAccessManager manager;
};

#endif // SETTINGS_H
