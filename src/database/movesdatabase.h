#ifndef MOVESDATABASE_H
#define MOVESDATABASE_H

#include "database.h"

#include <QObject>

class MovesDatabase : public Database
{
    Q_OBJECT

    Q_PROPERTY(int gameId READ gameId WRITE setGameId NOTIFY gameIdChanged)
public:
    explicit MovesDatabase(QObject *parent = nullptr);

    int gameId() { return _gameId; }
    void setGameId(int gameId);

    Q_INVOKABLE QList<QVariant> moves();
    Q_INVOKABLE void addMove(int id, int from, int to, int action, bool white, int matchId);
    Q_INVOKABLE QList<QVariant> getMoves();
    Q_INVOKABLE QList<QVariant> getMoves(int firstId, int matchId);
    Q_INVOKABLE void clearMoves();
    int lastMove();
//    void deleteGame(int id);

private:
    int _gameId;

signals:
    void gameIdChanged();
};

#endif // MOVESDATABASE_H
