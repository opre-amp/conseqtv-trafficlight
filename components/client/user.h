#ifndef USER_H
#define USER_H
#include <QObject>
#include <QDebug>

class loggedUser : public QObject
{
    Q_OBJECT

public:
    loggedUser() { m_token = "" ; m_username = "";}
    QString token() const { return m_token; }

public slots:
    void setToken(QString value);
    void setUsername(QString name);

signals:
    void tokenChanged(QString newValue);

private:
    QString m_token;
    QString m_username;
};

#endif // USER_H
