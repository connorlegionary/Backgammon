#ifndef GAMESDATABASE_H
#define GAMESDATABASE_H

#include "database.h"

#include <QObject>

#include "game.h"

class GamesDatabase : public Database
{
    Q_OBJECT

    Q_PROPERTY(QList<QVariant> games READ games NOTIFY gamesChanged)
    Q_PROPERTY(QList<QVariant> finishedGames READ finishedGames NOTIFY finishedGamesChanged)

public:
    explicit GamesDatabase(QObject *parent = nullptr);

    QList<QVariant> games();
    QList<QVariant> finishedGames();
    QList<GameData> gameData();
    Q_INVOKABLE QVariant getGame(int id);
    Q_INVOKABLE int lastId();
    Q_INVOKABLE int lastTurn(int gameId);
    Q_INVOKABLE int matchId(int gameId);
    Q_INVOKABLE int toScore(int gameId);
    Q_INVOKABLE bool isWhite(int gameId);
    Q_INVOKABLE void setFinished(int gameId);
    Q_INVOKABLE int getLastMoveId(int gameId);
    Q_INVOKABLE void updateTurn(int gameId, int turn);
    Q_INVOKABLE void clearLastAction(int gameId);
    Q_INVOKABLE void setCurrentMatchId(int gameId, int matchId);

    int getLastActionId(int gameId);
    void updateLastActionId(int gameId, int actionId);

    void addGame(Game* game);
    void deleteGame(int id);

signals:
    void gamesChanged();
    void finishedGamesChanged();
};

#endif // GAMESDATABASE_H
