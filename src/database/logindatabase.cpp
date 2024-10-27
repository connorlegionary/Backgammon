#include "logindatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

LoginDatabase::LoginDatabase(QObject *parent) : Database("login", parent)
{

}

QString LoginDatabase::username()
{
    QSqlQuery query(_db);
    query.prepare("SELECT username FROM login");
    if (query.exec())
    {
        while (query.next())
        {
            return query.value(0).toString();
        }
    }

    return "";
}

QString LoginDatabase::authToken()
{
    QSqlQuery query(_db);
    query.prepare("SELECT auth_token FROM login");
    if (query.exec())
    {
        while (query.next())
        {
            return query.value(0).toString();
        }
    }

    return "";
}

int LoginDatabase::id()
{
    QSqlQuery query(_db);
    query.prepare("SELECT id FROM login");
    if (query.exec())
    {
        while (query.next())
        {
            return query.value(0).toInt();
        }
    }

    return -1;
}

void LoginDatabase::setUsername(QString username)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE login SET username = ?");
    query.addBindValue(username);
    if (query.exec())
    {

    }
    emit usernameChanged();
}

void LoginDatabase::setAuthToken(QString token)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE login SET auth_token = ?");
    query.addBindValue(token);
    query.exec();
}

void LoginDatabase::setId(int id)
{
    QSqlQuery query(_db);
    query.prepare("UPDATE login SET id = ?");
    query.addBindValue(id);
    query.exec();
}
