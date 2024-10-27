#include "puzzlemovedatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PuzzleMoveDatabase::PuzzleMoveDatabase(QString name, QObject* parent) : Database(name, parent)
{

}

bool PuzzleMoveDatabase::addPuzzleMove(int id, const PuzzleMove& move) {
    QSqlQuery query(_db);
    query.prepare("INSERT INTO puzzle_moves (puzzle_id, from_pos, to_pos, roll, step, is_white) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(move.from());
    query.addBindValue(move.to());
    query.addBindValue(move.roll());
    query.addBindValue(move.step());
    query.addBindValue(move.isWhite());
    if (query.exec()) {
        qDebug() << "Added puzzle move with id" << query.lastInsertId().toInt();
        return true;
    }
    else {
        qDebug() << "Failed to add puzzle move:" << query.lastError();
        return false;
    }
}

QList<PuzzleMove> PuzzleMoveDatabase::getPuzzleMoves(int puzzleId) {
    QList<PuzzleMove> moves;
    QSqlQuery query(_db);
    query.prepare("SELECT from_pos, to_pos, roll, is_white, step FROM puzzle_moves WHERE puzzle_id = ?");
    query.addBindValue(puzzleId);
    if (query.exec()) {
        while (query.next()) {
            int fromPos = query.value(0).toInt();
            int toPos = query.value(1).toInt();
            int roll = query.value(2).toInt();
            bool isWhite = query.value(3).toBool();
            int step = query.value(4).toInt();
            moves.append(PuzzleMove(fromPos, toPos, roll, isWhite, step));
        }
        qDebug() << "Retrieved" << moves.size() << "puzzle moves for puzzle with id" << puzzleId;
    }
    else {
        qDebug() << "Failed to retrieve puzzle moves:" << query.lastError();
    }
    return moves;
}

bool PuzzleMoveDatabase::addPuzzleLocation(int puzzleId, const PuzzleLocation& location) {
    QSqlQuery query(_db);
    query.prepare("INSERT INTO puzzle_locations (puzzle_id, position, is_white) "
                  "VALUES (?,?,?)");
    query.addBindValue(puzzleId);
    qDebug() << "adding location at position: " << location.position();
    query.addBindValue(location.position());
    query.addBindValue(location.isWhite());
    if (query.exec()) {
        qDebug() << "Added puzzle location with id" << query.lastInsertId().toInt();
        return true;
    }
    else {
        qDebug() << "Failed to add puzzle location:" << query.lastError();
        return false;
    }
}

QList<PuzzleLocation> PuzzleMoveDatabase::getPuzzleLocations(int puzzleId) {
    QList<PuzzleLocation> moves;
    QSqlQuery query(_db);
    query.prepare("SELECT position, is_white FROM puzzle_locations WHERE puzzle_id = ?");
    query.addBindValue(puzzleId);
    if (query.exec()) {
        while (query.next()) {
            moves.append(PuzzleLocation(query.value(0).toInt(), query.value(1).toBool()));
        }
        qDebug() << "Retrieved" << moves.size() << "puzzle moves for puzzle with id" << puzzleId;
    }
    else {
        qDebug() << "Failed to retrieve puzzle moves:" << query.lastError();
    }
    return moves;
}

QList<QVariant> PuzzleMoveDatabase::getLocations(int puzzleId)
{
    QList<QVariant> pieces;
    QSqlQuery query(_db);
    query.prepare("SELECT position, is_white FROM puzzle_locations WHERE puzzle_id = ?");
    query.addBindValue(puzzleId);
    int id = 0;
    if (query.exec()) {
        while (query.next()) {
            pieces.append(QVariant::fromValue(new Piece(id, query.value(0).toInt(), query.value(1).toBool(), this)));
            id++;
        }
        qDebug() << "Retrieved" << pieces.size() << "puzzle pieces for puzzle with id" << puzzleId;
    }
    else {
        qDebug() << "Failed to retrieve puzzle moves:" << query.lastError();
    }
    return pieces;
}
