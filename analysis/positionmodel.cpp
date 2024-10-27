#include "positionmodel.h"

PositionSort::PositionSort()
{
    setDynamicSortFilter(true);
    setSortRole(ScoreRole);
}

PositionModel::PositionModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void PositionModel::addPosition(const Position &position)
{
//    int insert = 0;
//    for (int i = 0; i < _positions.size(); i++)
//    {
//        if (_positions.at(i).score() < position.score())
//        {
//            insert = i
//            break;
//        }
//    }
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    _positions << position;
    endInsertRows();
}

void PositionModel::addPreviousPosition(const Position &position)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _positions << position;
    endInsertRows();
}

void PositionModel::clearPositions()
{
    beginResetModel();
    _positions.clear();
    endResetModel();
}

int PositionModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return _positions.count();
}

QVariant PositionModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= _positions.count())
        return QVariant();

    const Position &position = _positions[index.row()];
    if (role == IndexRole)
        return position.index();
    else if (role == MovesRole)
        return position.getMoves();
    else if (role == BlackPointsLeftRole)
        return position.blackPointsLeft();
    else if (role == ScoreRole)
        return position.score();
    else if (role == WhitePointsLeftRole)
        return position.whitePointsLeft();
    else if (role == PositionsRole)
        return position.piecesList();
    return QVariant();
}

//![0]
QHash<int, QByteArray> PositionModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[MovesRole] = "moves";
    roles[ScoreRole] = "score";
    roles[WhitePointsLeftRole] = "whitePointsLeft";
    roles[BlackPointsLeftRole] = "blackPointsLeft";
    roles[PositionsRole] = "positions";

    return roles;
}
//![0]

