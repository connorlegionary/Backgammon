#ifndef PUZZLEMODEL_H
#define PUZZLEMODEL_H

#include <QAbstractListModel>
#include "../src/database/game.h"
#include "puzzles/puzzledata.h"
#include "src/database/puzzledatabase.h"

class PuzzleModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QVariant puzzlesDatabase READ puzzleDatabase WRITE setPuzzleDatabase NOTIFY puzzleDatabaseChanged)
public:
    enum PuzzleRoles {
        IndexRole = Qt::UserRole + 1,
        NameRole,
        DifficultyRole,
        DataRole,
    };

    PuzzleModel(QObject *parent = 0);

    void addPuzzle(const PuzzleData& game);
    void setPuzzles(QList<PuzzleData> games);
    void clearPuzzles();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QVariant puzzleDatabase() { return QVariant::fromValue(_puzzleDatabase); }
    void setPuzzleDatabase(QVariant puzzles);
protected:
    QHash<int, QByteArray> roleNames() const;
    PuzzleDatabase* _puzzleDatabase;

private:
    QList<PuzzleData> _puzzles;

signals:
    void puzzleDatabaseChanged();

public slots:
    void addedPuzzle();
};

#endif // PUZZLEMODEL_H
