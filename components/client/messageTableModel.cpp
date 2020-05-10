#include "messageTableModel.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

static const char *messageTableName = "Messages";

static void createTable()
{
    QSqlQuery query;
    query.exec("DROP TABLE 'Messages'");

    if (!query.exec(
        "CREATE TABLE IF NOT EXISTS 'Messages' ("
        "'author' TEXT NOT NULL,"
        "'group' TEXT NOT NULL,"
        "'timestamp' TEXT NOT NULL,"
        "'message' TEXT NOT NULL"
        ")")) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }

}

messageTableModel::messageTableModel(QObject *parent) :
    QSqlTableModel(parent)
{
    createTable();
    setTable(messageTableName);
    setSort(2, Qt::DescendingOrder);
    // Ensures that the model is sorted correctly after submitting a new row.
    setEditStrategy(QSqlTableModel::OnManualSubmit);
}

QString messageTableModel::group() const
{
    return m_group;
}

void messageTableModel::setgroup(const QString &group)
{
    if (group == m_group)
        return;
    m_group = group;
    emit groupChanged();
}

QVariant messageTableModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole)
        return QSqlTableModel::data(index, role);

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> messageTableModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "author";
    names[Qt::UserRole + 1] = "group";
    names[Qt::UserRole + 2] = "timestamp";
    names[Qt::UserRole + 3] = "message";
    return names;
}

void messageTableModel::sendMessage(const QString &author, const QString &group, const QString &message)
{
    const QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

    QSqlQuery query;
    query.prepare("INSERT INTO Messages "
                    "VALUES (:author, :group, :timestamp, :message)");
    query.bindValue(":author", author);
    query.bindValue(":group", group);
    query.bindValue(":timestamp", timestamp);
    query.bindValue(":message", message);
    query.exec();

    submitAll();
}
