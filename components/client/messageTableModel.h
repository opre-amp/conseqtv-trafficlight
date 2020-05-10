#ifndef MESSAGETABLEMODEL_H
#define MESSAGETABLEMODEL_H

#include <QSqlTableModel>

class messageTableModel : public QSqlTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString group READ group WRITE setgroup NOTIFY groupChanged)

public:
    messageTableModel (QObject *parent = 0);

    QString group() const;
    void setgroup(const QString &group);
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void sendMessage(const QString &author, const QString &group, const QString &message);

signals:
    void groupChanged();


private:
    QString m_group;
};

#endif // MESSAGETABLEMODEL_H
