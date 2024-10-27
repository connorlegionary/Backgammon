#include "computergamedatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDateTime>

#include <QDebug>

#include "computergame.h"

ComputerGamesDatabase::ComputerGamesDatabase(QObject *parent) : Database("ComputerGames", parent)
{

}

QList<QVariant> ComputerGamesDatabase::games()
{
    QList<QVariant> games;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, computer, toScore, timelimit, started FROM computer_games WHERE isFinished = 0");
    if (query.exec())
    {
        while (query.next())
        {
            games.append(QVariant::fromValue(new ComputerGame(this, query.value(1).toInt(), query.value(0).toInt(), query.value(2).toInt(), query.value(3).toInt(), query.value(4).toDateTime())));
        }
    }
    return games;
}

QList<QVariant> ComputerGamesDatabase::finishedGames()
{
    QList<QVariant> games;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, name, joined, turn, toScore FROM computer_games WHERE isFinished = 1");
    if (query.exec())
    {
        while (query.next())
        {
//            games.append(QVariant::fromValue(new Game(this, query.value(0).toInt(), query.value(1).toString(), query.value(2).toBool(), query.value(3).toInt(), query.value(4).toInt())));
        }
    }
    return games;
}

QVariant ComputerGamesDatabase::getGame(int id)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, name, joined, turn, toScore FROM computer_games WHERE id = ?");
    query.addBindValue(id);
    if (query.exec())
    {
        if (query.next())
        {
//            return QVariant::fromValue(new Game(this, query.value(0).toInt(), query.value(1).toString(), query.value(2).toBool(), query.value(3).toInt(), query.value(4).toInt()));
        }
    }
    return QVariant();
}

int ComputerGamesDatabase::lastId()
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT MAX(id) FROM computer_games");
    if (query.exec())
    {
        if (query.next())
        {
            return query.value(0).toInt();
        }
    }
    return -1;
}

int ComputerGamesDatabase::lastTurn(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT turn FROM computer_games WHERE id = ?");
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

int ComputerGamesDatabase::matchId(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT matchId FROM computer_games WHERE id = ?");
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

int ComputerGamesDatabase::toScore(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT toScore FROM computer_games WHERE id = ?");
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

int ComputerGamesDatabase::finishGame(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE computer_games SET isFinished = 1 WHERE id = ?");
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

bool ComputerGamesDatabase::isWhite(int gameId)
{
    //    qDebug() << "isWhite: " << gameId;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT isWhite FROM computer_games WHERE id = ?");
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

void ComputerGamesDatabase::setFinished(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE computer_games SET isFinished = 1 WHERE id = ?");
    query.addBindValue(gameId);
    if (!query.exec())
    {
    }
}

int ComputerGamesDatabase::getLastMoveId(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT MAX(id) FROM computer_games WHERE gameId = ?");
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

void ComputerGamesDatabase::updateTurn(int gameId, int turn)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE computer_games SET turn = ? WHERE id = ?");
    query.addBindValue(turn);
    query.addBindValue(gameId);
    query.exec();
}

void ComputerGamesDatabase::clearLastAction(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE computer_games SET lastAction = 0 WHERE id = ?");
    query.addBindValue(gameId);
    query.exec();
}

void ComputerGamesDatabase::setCurrentMatchId(int gameId, int matchId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE computer_games SET matchId = ? WHERE id = ?");
    query.addBindValue(matchId);
    query.addBindValue(gameId);
    query.exec();
}

int ComputerGamesDatabase::createGame(int computer, int score, int seconds)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("INSERT INTO computer_games (computer, toScore, timeLimit, started) VALUES (?,?,?,?)");
    query.addBindValue(computer);
    query.addBindValue(score);
    query.addBindValue(seconds);
    query.addBindValue(QDateTime::currentDateTime());
    query.exec();
    emit gamesChanged();
    return query.lastInsertId().toInt();
}

void ComputerGamesDatabase::saveGame(int id, QString data)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE computer_games SET data = ? WHERE id = ?");
    query.addBindValue(data);
    query.addBindValue(id);
    query.exec();
}

int ComputerGamesDatabase::getLastActionId(int gameId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT lastAction FROM computer_games WHERE id = ?");
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

void ComputerGamesDatabase::updateLastActionId(int gameId, int actionId)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("UPDATE computer_games SET lastAction = ? WHERE id = ?");
    query.addBindValue(actionId);
    query.addBindValue(gameId);
    query.exec();
}


void ComputerGamesDatabase::deleteGame(int id)
{
    qDebug() << "delted from computer_games: " << id;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("DELETE FROM computer_games WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec())
    {
        qDebug() << "error deleting game: " << query.lastError().text();
    } else {

    }

    query.prepare("DELETE FROM actions WHERE gameId = ? AND computer = 1");
    query.addBindValue(id);
    if (query.exec())
    {
        qDebug() << "error deleting actions: " << query.lastError().text();
    }
    emit gamesChanged();
}
