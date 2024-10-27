#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QVariant>

class Database : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString connectionName READ connectionName WRITE setConnectionName NOTIFY connectionNameChanged)

public:
    explicit Database(QObject *parent = nullptr);
    explicit Database(QString connectionName, QObject *parent = nullptr);
    void initialise(QString connectionName);
    ~Database();

    QSqlDatabase getDatabase();

    QString connectionName() { return _connectionName; }
    void setConnectionName(QString connectionName);

    Q_INVOKABLE bool clearData();
protected:
    QSqlDatabase _db;
    QString _connectionName;

signals:
    void connectionNameChanged();
};
