#ifndef PUZZLEDATA_H
#define PUZZLEDATA_H

#include <QObject>
#include <QString>

#include "src/piece.h"

class PuzzleLocation {
public:
    PuzzleLocation(int position, bool isWhite);

    int position() const { return _position; }
    bool isWhite() const { return _isWhite; }

private:
    int _position = 0;
    bool _isWhite = false;
};

class PuzzleMove {
public:
    PuzzleMove(int from, int to, int roll, bool isWhite, int step);

    int from() const { return _from; }
    int to() const { return _to; }
    int roll() const { return _roll; }
    int step() const { return _step; }
    bool isWhite() const { return _isWhite; }
    int id() const { return 0; }

private:
    int _from = 0;
    int _to = 0;
    int _roll = 0;
    int _step = 0;
    bool _isWhite = false;
};

class PuzzleData {

  public:
    // Constructor
    PuzzleData(QString name, int index, int difficulty, bool isWhite);


    QString name() const {
        return _name;
    }
    void setName(QString name) {
        _name = name;
    }

    int id() const {
        return _index;
    }

    void setIndex(int index) {
        _index = index;
    }

    int difficulty() const {
        return _difficulty;
    }
    void setDifficulty(int difficulty) {
        _difficulty = difficulty;
    }

    bool isWhite() const { return _isWhite; }
    void setIsWhite(bool isWhite) { _isWhite = isWhite;}

    void addPuzzleMove(PuzzleMove* move);
    void addPuzzleLocation(PuzzleLocation* location);

    QList<PuzzleMove*> moves() const { return _moves; }
    QList<PuzzleLocation*> locations() const { return _locations; }

private:
    QString _name;
    int _index;
    int _difficulty;
    bool _isWhite;
    QList<PuzzleMove*> _moves;
    QList<PuzzleLocation*> _locations;
};



class Puzzle : public QObject {
    Q_OBJECT

public:
    Puzzle(PuzzleData* data);

    QString name() { return _data->name(); }
    int id() { return _data->id(); }
    int difficulty() { return _data->difficulty(); }
    bool isWhite() { return _data->isWhite(); }
    int steps();

    QList<PuzzleMove> moves() { return _moves; }
    QList<Piece*> pieces() { return _pieces; }

    QList<PuzzleMove> movesAtStep(int step);
    bool checkMoveOnStep(int step, int move, int fromPos, int isWhite = 2);

    void setMoves(QList<PuzzleMove> moves);
    void setPieces(QList<QVariant> pieces);

private:
    PuzzleData* _data;
    QList<PuzzleMove> _moves;
    QList<Piece*> _pieces;
};

#endif // PUZZLEDATA_H
