#pragma once

#include "database.h"

#include <QObject>

class LoginDatabase : public Database
{
    Q_OBJECT

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
public:
    explicit LoginDatabase(QObject *parent = nullptr);

    QString username();
    QString authToken();
    int id();

    void setUsername(QString username);
    void setAuthToken(QString token);
    void setId(int id);

private:
    QString _username;
    QString _authToken;

signals:
    void usernameChanged();
};
