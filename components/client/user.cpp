#include "user.h"

#include <QDateTime>
#include <QDebug>

void loggedUser::setToken(QString token)
{
    if (token != m_token) {
        m_token = token;
        emit tokenChanged(token);

    }
}
void loggedUser::setUsername(QString name){

    m_username = name;

}
