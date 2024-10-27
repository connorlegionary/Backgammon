#ifndef POSITIONMODEL_H
#define POSITIONMODEL_H

#include "analysis.h"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>

enum PositionRoles {
    IndexRole = Qt::UserRole + 1,
    MovesRole,
    ScoreRole,
    WhitePointsLeftRole,
    BlackPointsLeftRole,
    PositionsRole
};

class PositionSort : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    PositionSort();
};

class PositionModel : public QAbstractListModel
{
    Q_OBJECT
public:


    PositionModel(QObject *parent = 0);


    void addPosition(const Position &position);
    void addPreviousPosition(const Position &position);
    void clearPositions();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Position> _positions;
};

#endif // POSITIONMODEL_H
