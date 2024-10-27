#include "movesdatabase.h"

#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>

#include <QVariant>

#include "move.h"

MovesDatabase::MovesDatabase(QObject *parent) : Database("moves", parent)
{

}

void MovesDatabase::setGameId(int gameId)
{
    _gameId = gameId;
    emit gameIdChanged();
}

QList<QVariant> MovesDatabase::moves()
{
    QList<QVariant> moves;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, gameId, fromPos, toPos, user, action, time, matchId FROM moves");
    if (query.exec())
    {
        while (query.next())
        {
            moves.append(QVariant::fromValue(new PieceMove(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt(), query.value(4).toInt(), query.value(5).toInt() == 1, query.value(7).toInt(), query.value(6).toDateTime(), query.value(8).toInt())));
        }
    }
    return moves;
}

void MovesDatabase::addMove(int id, int from, int to, int action, bool white, int matchId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("INSERT INTO moves (id, gameId, fromPos, toPos, isWhite, action, time, matchId) VALUES (?,?,?,?,?,?,?,?)");
    query.addBindValue(id);
    query.addBindValue(_gameId);
    query.addBindValue(from);
    query.addBindValue(to);
    query.addBindValue(white);
    query.addBindValue(action);
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(matchId);
    if (!query.exec())
    {
        qDebug() << "add move error: " << query.lastError().text();
    }
}

QList<QVariant> MovesDatabase::getMoves()
{
    QList<QVariant> moves;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, gameId, fromPos, toPos, isWhite, action, time, matchId FROM moves WHERE gameId = ?");
    query.addBindValue(_gameId);
    if (query.exec())
    {
        while (query.next())
        {
            moves.append(QVariant::fromValue(new PieceMove(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt(), query.value(2).toInt() - query.value(3).toInt(), query.value(4).toInt(), query.value(5).toInt() == 1, query.value(6).toDateTime(), query.value(7).toInt())));
        }
    } else {
        qDebug() << "Last Error: " << query.lastError().text();
    }
    return moves;
}

QList<QVariant> MovesDatabase::getMoves(int firstId, int matchId)
{
    QList<QVariant> moves;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, gameId, fromPos, toPos, isWhite, action, time, matchId FROM moves WHERE id >= ? AND matchId = ? AND gameId = ?");
    query.addBindValue(firstId);
    query.addBindValue(matchId);
    query.addBindValue(_gameId);
    if (query.exec())
    {
        while (query.next())
        {
            moves.append(QVariant::fromValue(new PieceMove(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt(), query.value(2).toInt() - query.value(3).toInt(), query.value(4).toInt(), query.value(5).toInt() == 1, query.value(6).toDateTime(), query.value(7).toInt())));
        }
    } else {
        qDebug() << "Last Error: " << query.lastError().text();
    }
    return moves;
}

void MovesDatabase::clearMoves()
{
    QList<QVariant> moves;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("DELETE FROM moves WHERE gameId = ?");
    query.addBindValue(_gameId);
    if (!query.exec())
    {
        qDebug() << "Failed to delete moves for game: " << _gameId;
    }
}

int MovesDatabase::lastMove()
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT MAX(id) FROM moves WHERE gameId = ?");
    query.addBindValue(_gameId);
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }

    return 0;
}
