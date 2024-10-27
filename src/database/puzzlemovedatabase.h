#ifndef PUZZLEMOVEDATABASE_H
#define PUZZLEMOVEDATABASE_H


#include "puzzles/puzzledata.h"
#include "database.h"
#include "src/piecemodel.h"

class PuzzleMoveDatabase : public Database
{
public:
    PuzzleMoveDatabase(QString name = "puzzlemoves", QObject* parent = nullptr);

    QList<PuzzleMove> getPuzzleMoves(int puzzleId);
    bool addPuzzleMove(int puzzleId, const PuzzleMove& move);

    QList<PuzzleLocation> getPuzzleLocations(int puzzleId);
    Q_INVOKABLE QList<QVariant> getLocations(int puzzleId);
    bool addPuzzleLocation(int puzzleId, const PuzzleLocation& locaton);
};

#endif // PUZZLEMOVEDATABASE_H
