#include "gamesdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

GamesDatabase::GamesDatabase(QObject *parent) : Database("games", parent)
{

}

QList<QVariant> GamesDatabase::games()
{
    QList<QVariant> games;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, name, joined, turn, toScore FROM games WHERE isFinished = 0");
    if (query.exec())
    {
        while (query.next())
        {
            games.append(QVariant::fromValue(new Game(this, query.value(0).toInt(), query.value(1).toString(), query.value(2).toBool(), query.value(3).toInt(), query.value(4).toInt())));
        }
    }
    return games;
}

QList<QVariant> GamesDatabase::finishedGames()
{
    QList<QVariant> games;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, name, joined, turn, toScore FROM games WHERE isFinished = 1");
    if (query.exec())
    {
        while (query.next())
        {
            games.append(QVariant::fromValue(new Game(this, query.value(0).toInt(), query.value(1).toString(), query.value(2).toBool(), query.value(3).toInt(), query.value(4).toInt())));
        }
    }
    return games;
}

QList<GameData> GamesDatabase::gameData()
{
    QList<GameData> games;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, name, toScore FROM games WHERE isFinished = 0");
    if (query.exec())
    {
        while (query.next())
        {
            games.append(GameData(query.value(0).toInt(), query.value(1).toString(), query.value(2).toInt(), 0,0,0,0));
        }
    }
    return games;
}

QVariant GamesDatabase::getGame(int id)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, name, joined, turn, toScore FROM games WHERE id = ?");
    query.addBindValue(id);
    if (query.exec())
    {
        if (query.next())
        {
            return QVariant::fromValue(new Game(this, query.value(0).toInt(), query.value(1).toString(), query.value(2).toBool(), query.value(3).toInt(), query.value(4).toInt()));
        }
    }
    return QVariant();
}

int GamesDatabase::lastId()
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT MAX(id) FROM games");
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}

int GamesDatabase::lastTurn(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT turn FROM games WHERE id = ?");
    query.addBindValue(gameId);
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}

int GamesDatabase::matchId(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT matchId FROM games WHERE id = ?");
    query.addBindValue(gameId);
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}

int GamesDatabase::toScore(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT toScore FROM games WHERE id = ?");
    query.addBindValue(gameId);
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}

bool GamesDatabase::isWhite(int gameId)
{
//    qDebug() << "isWhite: " << gameId;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT isWhite FROM games WHERE id = ?");
    query.addBindValue(gameId);
    if (query.exec())
    {
        if (query.next())
        {
            qDebug() << "isWhite for: " << gameId << " = " << query.value(0).toBool();
            return query.value(0).toBool();
        }
    }
    return -1;
}

void GamesDatabase::setFinished(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE games SET isFinished = 1 WHERE id = ?");
    query.addBindValue(gameId);
    if (!query.exec())
    {
    }
}

int GamesDatabase::getLastMoveId(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT MAX(id) FROM moves WHERE gameId = ?");
    query.addBindValue(gameId);
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}

void GamesDatabase::updateTurn(int gameId, int turn)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE games SET turn = ? WHERE id = ?");
    query.addBindValue(turn);
    query.addBindValue(gameId);
    query.exec();
}

void GamesDatabase::clearLastAction(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE games SET lastAction = 0 WHERE id = ?");
    query.addBindValue(gameId);
    query.exec();
}

void GamesDatabase::setCurrentMatchId(int gameId, int matchId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE games SET matchId = ? WHERE id = ?");
    query.addBindValue(matchId);
    query.addBindValue(gameId);
    query.exec();
}

int GamesDatabase::getLastActionId(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT lastAction FROM games WHERE id = ?");
    query.addBindValue(gameId);
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}

void GamesDatabase::updateLastActionId(int gameId, int actionId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE games SET lastAction = ? WHERE id = ?");
    query.addBindValue(actionId);
    query.addBindValue(gameId);
    query.exec();
}

void GamesDatabase::addGame(Game* game)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("INSERT INTO games (id, name, joined, isWhite, turn, toScore) VALUES (?,?,?,?,?,?)");
    query.addBindValue(game->id());
    query.addBindValue(game->name());
    query.addBindValue(game->joined());
    query.addBindValue(game->isWhite());
    query.addBindValue(game->turn());
    query.addBindValue(game->toScore());
    if (!query.exec())
    {

    }
}

void GamesDatabase::deleteGame(int id)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("DELETE FROM games WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
    {
    }
}
