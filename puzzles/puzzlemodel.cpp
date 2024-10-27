#include "puzzlemodel.h"

#include <QDebug>

PuzzleModel::PuzzleModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void PuzzleModel::addPuzzle(const PuzzleData &puzzle)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _puzzles << puzzle;
    endInsertRows();
}

void PuzzleModel::setPuzzles(QList<PuzzleData> puzzles)
{
    qDebug() << "Set puzzles: " << puzzles.size();
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    for (int i = 0; i < puzzles.size(); i++)
    {
        _puzzles << puzzles.at(i);
    }
    endInsertRows();
}

void PuzzleModel::setPuzzleDatabase(QVariant puzzles) {
    _puzzleDatabase = puzzles.value<PuzzleDatabase*>();
    connect(_puzzleDatabase, &PuzzleDatabase::puzzleAdded, this, &PuzzleModel::addedPuzzle);
    setPuzzles(_puzzleDatabase->getPuzzles());
}

void PuzzleModel::clearPuzzles()
{
    beginResetModel();
    _puzzles.clear();
    endResetModel();
}

int PuzzleModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return _puzzles.count();
}

QVariant PuzzleModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= _puzzles.count())
        return QVariant();

    const PuzzleData &puzzle = _puzzles[index.row()];
    if (role == IndexRole)
        return puzzle.id();
    else if (role == NameRole)
        return puzzle.name();
    else if (role == DifficultyRole)
        return puzzle.difficulty();

    return QVariant();
}

void PuzzleModel::addedPuzzle()
{
    setPuzzles(_puzzleDatabase->getPuzzles());
}

//![0]
QHash<int, QByteArray> PuzzleModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IndexRole] = "index";
    roles[NameRole] = "name";
    roles[DifficultyRole] = "difficulty";
    roles[DataRole] = "data";

    return roles;
}
//![0]

