#ifndef PUZZLEDATABASE_H
#define PUZZLEDATABASE_H

#include <QObject>

#include "puzzles/puzzledata.h"
#include "database.h"
#include "puzzlemovedatabase.h"

class PuzzleDatabase : public Database
{
    Q_OBJECT
public:
    explicit PuzzleDatabase(QObject *parent = nullptr);

    void insertPuzzle(const PuzzleData& puzzle);
    void updatePuzzle(const PuzzleData& puzzle);
    void deletePuzzle(int index);
    PuzzleData getPuzzle(int index);
    QList<PuzzleData> getPuzzles();
    Q_INVOKABLE QVariant getPuzzleWithData(int index);

    Q_INVOKABLE QList<QVariant> getLocations(int puzzleId) { return _puzzleMoveDb->getLocations(puzzleId);}

    Q_INVOKABLE void puzzleFinished(int id, int score);

    int lastId();
private:
    PuzzleMoveDatabase* _puzzleMoveDb;
signals:
    void puzzleAdded();
};

#endif // PUZZLEDATABASE_H
