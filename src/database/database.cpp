#include "database.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QBuffer>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QStandardPaths>
#include <QDateTime>

#include <QDebug>

Database::Database(QObject *parent) : QObject(parent)
{

}

Database::Database(QString connectionName, QObject *parent) : QObject(parent)
{
    initialise(connectionName);
}

void Database::initialise(QString connectionName)
{
    _db = QSqlDatabase::addDatabase("QSQLITE", connectionName);

#ifdef Q_OS_IOS // TODO figure out why Q_OS_IOS doesn't work
    _db.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/database");
#else
    _db.setDatabaseName("metadata.db");
#endif

    if (!_db.open())
    {
        qDebug() << "Couldn't load db";
    }

    if (_db.tables().count() == 0)
    {
        qDebug() << "Create db create db create db create db create db create db create db create db";
        QSqlQuery query(_db);
        if (!query.exec("CREATE TABLE games (id INTEGER PRIMARY KEY, name TEXT, joined INTEGER, lastAction INTEGER, turn INTEGER, isWhite TINYINT, isFinished TINYINT DEFAULT 0, toScore INTEGER, matchId INTEGER);"))
        {

        }

        if (!query.exec("CREATE TABLE computer_games (id INTEGER PRIMARY KEY, computer TEXT, isFinished TINYINT DEFAULT 0, toScore INTEGER, timeLimit INTEGER, data TEXT, started DATE);"))
        {

        }


        if (!query.exec("CREATE TABLE moves (id INTEGER PRIMARY KEY, gameId INTEGER, fromPos INTEGER, toPos INTEGER, action INTEGER, context INTEGER, isWhite INTEGER, time DATE, matchId INTEGER);"))
        {

        }

        if (query.exec("CREATE TABLE login (id INTEGER, username TEXT, auth_token TEXT);"))
        {
            query.exec("INSERT INTO login (username, auth_token) VALUES ('','')");
        }

        if (query.exec("CREATE TABLE settings (id INTEGER PRIMARY KEY, dbVersion INTEGER, ip TEXT);"))
        {
            query.prepare("INSERT INTO settings (dbVersion, ip) VALUES ('1',?)");

#ifdef Q_OS_IOS
    query.addBindValue("http://192.168.2.154:5001/");
#elif defined(Q_OS_ANDROID)
    query.addBindValue("http://192.168.0.16:5001/");
#else
    query.addBindValue("http://127.0.0.1:5001/");
#endif
            query.exec();
        }
        query.exec("CREATE TABLE puzzles (name TEXT, id INTEGER PRIMARY KEY, difficulty INTEGER, isWhite TINYINT)");

        query.exec("CREATE TABLE IF NOT EXISTS puzzle_moves ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "puzzle_id INTEGER,"
                       "from_pos INTEGER,"
                       "to_pos INTEGER,"
                       "is_white INTEGER,"
                       "step INTEGER,"
                       "roll INTEGER)");

        query.exec("CREATE TABLE IF NOT EXISTS puzzle_locations ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "puzzle_id INTEGER,"
                       "position INTEGER,"
                       "is_white INTEGER)");

        query.exec("CREATE TABLE IF NOT EXISTS puzzle_results ("
                   "id INTEGER,"
                   "score INTEGER)");

        query.exec("CREATE TABLE actions ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                   "matchId INTEGER, "
                   "gameId INTEGER, "
                   "dice1 INTEGER, "
                   "dice2 INTEGER, "
                   "isWhite INTEGER, "
                   "action INTEGER, "
                   "context INTEGER DEFAULT 0, "
                   "computer INTEGER DEFAULT 0, "
                   "time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP);");
    }
}

Database::~Database()
{
    _db.close();
}

void Database::setConnectionName(QString connectionName)
{
    _connectionName = connectionName;
    initialise(connectionName);
    emit connectionNameChanged();
}

bool Database::clearData()
{
    QSqlQuery query(_db);
    query.exec("DELETE FROM moves");
    query.exec("UPDATE login SET username = '' AND auth_token = ''");
    query.exec("DELETE FROM games");
    return true;
}
