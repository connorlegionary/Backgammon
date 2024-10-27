#pragma once

#include "database.h"

#include <QObject>

class SettingsDatabase : public Database
{
    Q_OBJECT

    Q_PROPERTY(QString ip READ ip NOTIFY ipChanged)
    Q_PROPERTY(QList<QVariant> ips READ ips NOTIFY ipsChanged)
public:
    explicit SettingsDatabase(QObject *parent = nullptr);

    QString ip();
    QList<QVariant> ips();

    Q_INVOKABLE void addIp(QString ip);
    Q_INVOKABLE void setIp(int ip);
private:
    QString _ip;
    int _currentIp = 1;

signals:
    void ipChanged();
    void ipsChanged();
};
