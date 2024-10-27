#include "gamemodel.h"

#include <QDebug>

GameModel::GameModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void GameModel::addGame(const GameData &game)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _games << game;
    endInsertRows();
}

void GameModel::setGames(QList<GameData> games)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    for (int i = 0; i < games.size(); i++)
    {
        qDebug() << "Setting game: " << games.at(i).name();
        _games << games.at(i);
    }
    endInsertRows();
}

void GameModel::clearGames()
{
    beginResetModel();
    _games.clear();
    endResetModel();
}

int GameModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return _games.count();
}

QVariant GameModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= _games.count())
        return QVariant();

    const GameData &game = _games[index.row()];
    if (role == IndexRole)
        return game.id();
    else if (role == NameRole)
    {
        qDebug() << "Gettng game name: " << game.name();
        return game.name();
    }
    else if (role == ToScoreRole)
        return game.toScore();
    else if (role == Score1Role)
        return game.score1();
    else if (role == Score2Role)
        return game.score2();
    else if (role == DoubleRole)
        return game.doubleVal();
    else if (role == MatchRole)
        return game.matchId();

    return QVariant();
}

//![0]
QHash<int, QByteArray> GameModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[NameRole] = "name";
    roles[ToScoreRole] = "toScore";
    roles[Score1Role] = "score1";
    roles[Score2Role] = "score2";
    roles[DoubleRole] = "double";
    roles[MatchRole] = "match";

    return roles;
}
//![0]

