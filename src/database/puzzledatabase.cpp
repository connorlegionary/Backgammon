#include "puzzledatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PuzzleDatabase::PuzzleDatabase(QObject *parent) : Database("puzzles", parent)
{
    _puzzleMoveDb = new PuzzleMoveDatabase("PuzzleChildDb");
}

void PuzzleDatabase::insertPuzzle(const PuzzleData& puzzle) {
    QSqlQuery query(_db);
    qDebug() << "inserting puzzle";
    query.prepare("INSERT INTO puzzles (name, id, difficulty, isWhite) VALUES (?,?,?,?)");
    query.addBindValue(puzzle.name());
    query.addBindValue(puzzle.id());
    query.addBindValue(puzzle.difficulty());
    query.addBindValue(puzzle.isWhite());
    if (!query.exec()) {
        qWarning() << "Failed to insert puzzle:" << query.lastError().text();
    }

    QList<PuzzleMove*> moves =puzzle.moves();
    for (int i = 0; i < moves.size(); i++)
    {
        _puzzleMoveDb->addPuzzleMove(puzzle.id(), *moves.at(i));
    }

    QList<PuzzleLocation*> locations =puzzle.locations();
    for (int i = 0; i < locations.size(); i++)
    {
        _puzzleMoveDb->addPuzzleLocation(puzzle.id(), *locations.at(i));
    }
}

void PuzzleDatabase::updatePuzzle(const PuzzleData& puzzle) {
    QSqlQuery query(_db);
    query.prepare("UPDATE puzzles SET name=?, difficulty=?, isWhite=? WHERE id=?");
    query.addBindValue(puzzle.name());
    query.addBindValue(puzzle.id());
    query.addBindValue(puzzle.difficulty());
    query.addBindValue(puzzle.isWhite());
    if (!query.exec()) {
        qWarning() << "Failed to update puzzle:" << query.lastError().text();
    }
}

void PuzzleDatabase::deletePuzzle(int index) {
    QSqlQuery query(_db);
    query.prepare("DELETE FROM puzzles WHERE id=?");
    query.addBindValue(index);
    if (!query.exec()) {
        qWarning() << "Failed to delete puzzle:" << query.lastError().text();
    }
}

PuzzleData PuzzleDatabase::getPuzzle(int index) {
    QSqlQuery query(_db);
    query.prepare("SELECT name, difficulty, isWhite FROM puzzles WHERE id=?");
    query.addBindValue(index);
    if (!query.exec() || !query.first()) {
        qWarning() << "Failed to retrieve puzzle:" << query.lastError().text();
        return PuzzleData("", 0, -1, true);
    }
    QString name = query.value(0).toString();
    int difficulty = query.value(1).toInt();
    bool isWhite = query.value(2).toBool();
    return PuzzleData(name, index, difficulty, isWhite);
}

QVariant PuzzleDatabase::getPuzzleWithData(int index)
{
    QSqlQuery query(_db);
    query.prepare("SELECT name, difficulty, isWhite FROM puzzles WHERE id=?");
    query.addBindValue(index);
    if (!query.exec() || !query.first()) {
        qWarning() << "Failed to retrieve puzzle:" << query.lastError().text();
        return QVariant::fromValue(new Puzzle(new PuzzleData("", 0, -1, true)));
    }
    QString name = query.value(0).toString();
    int difficulty = query.value(1).toInt();
    bool isWhite = query.value(2).toBool();
    Puzzle* puzzleData = new Puzzle(new PuzzleData(name, index, difficulty, isWhite));
    puzzleData->setMoves(_puzzleMoveDb->getPuzzleMoves(index));
    puzzleData->setPieces(_puzzleMoveDb->getLocations(index));
    return QVariant::fromValue(puzzleData);
}

void PuzzleDatabase::puzzleFinished(int id, int score)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO puzzle_results (id, score) VALUES (?,?)");
    query.addBindValue(id);
    query.addBindValue(score);
    if (query.exec())
    {

    }
}

int PuzzleDatabase::lastId()
{
    int retval = 0;
    QSqlQuery query("SELECT MAX(id) FROM puzzles", _db);
    if (query.next())
    {
        retval = query.value(0).toInt();
    }
    return retval;
}

QList<PuzzleData> PuzzleDatabase::getPuzzles() {
    QList<PuzzleData> puzzles;
    QSqlQuery query("SELECT name, id, difficulty, isWhite FROM puzzles WHERE id NOT IN (SELECT id FROM puzzle_results)", _db);
    while (query.next()) {
        QString name = query.value(0).toString();
        int index = query.value(1).toInt();
        int difficulty = query.value(2).toInt();
        bool isWhite = query.value(3).toBool();
        puzzles.append(PuzzleData(name, index, difficulty, isWhite));
    }
    return puzzles;
}
