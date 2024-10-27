#include "settingsdatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

#include <QDebug>

SettingsDatabase::SettingsDatabase(QObject *parent) : Database("settings", parent)
{

}

QList<QVariant> SettingsDatabase::ips()
{
    QList<QVariant> retval;
    QSqlQuery query(_db);
    query.prepare("SELECT ip FROM settings");
    if (query.exec())
    {
        while (query.next())
        {
            retval.append(query.value(0).toString());
        }
    }

    qDebug() << "Retval: " << retval;

    return retval;
}

void SettingsDatabase::addIp(QString ip)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO settings (ip) VALUES (?)");
    query.addBindValue(ip);
    if (query.exec())
    {

    }
    emit ipsChanged();
}

QString SettingsDatabase::ip()
{
    QString retval;
    QSqlQuery query(_db);
    query.prepare("SELECT ip FROM settings WHERE id = ?");
    query.addBindValue(_currentIp);
    if (query.exec())
    {
        if (query.next())
        {
            retval = query.value(0).toString();
        }
    }

    return retval;
}

void SettingsDatabase::setIp(int ip)
{
    _currentIp = ip;
    qDebug() << "Set current ip: " << _currentIp;
}
