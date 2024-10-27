#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QAbstractListModel>
#include "../src/database/game.h"

class GameModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum GameRoles {
        IndexRole = Qt::UserRole + 1,
        NameRole,
        ToScoreRole,
        Score1Role,
        Score2Role,
        DoubleRole,
        MatchRole,
    };

    GameModel(QObject *parent = 0);

    void addGame(const GameData& game);
    void setGames(QList<GameData> games);
    void clearGames();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<GameData> _games;
};

#endif // GAMEMODEL_H
