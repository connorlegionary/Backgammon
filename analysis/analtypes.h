#ifndef ANALTYPES_H
#define ANALTYPES_H

#include <QObject>
#include "src/piece.h"
#include "src/piecelist.h"

#include "diceroll.h"

class AnalysisMove : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant from READ from NOTIFY dataChanged)
    Q_PROPERTY(QVariant to READ to NOTIFY dataChanged)
public:
    AnalysisMove(AnalysisMove* move);
    AnalysisMove(int from, int to);

    int from() { return _from; }
    int to() { return _to; }
    int move();

private:
    int _from;
    int _to;

signals:
    void dataChanged();
};

class Position
{
public:
    Position(QList<Piece*> pieces, bool copy = false);
    Position(QList<Piece*> pieces, QList<AnalysisMove*> moves, bool copy = false, int index = 0);
    //    _game->pieces(), intermediatePosition.at(i)->position(), toPositionSecond, piece->position(), toPosition, score
    Position(QList<Piece*> pieces, QList<AnalysisMove*> moves, int score);
    Position(QList<Piece*> pieces, QList<QVariant> moves, int score, int whitePointsLeft, int blackPointsLeft);

    QList<Piece*> pieces() { return _pieces; }
    QList<QVariant> getPieces();
    QVariant piecesList() const {
        QList<QVariant> retVal;
        for (int i = 0; i < _pieces.size(); i++)
        {
            retVal.append(QVariant::fromValue(_pieces.at(i)));
        }
//        return retVal;

        return QVariant::fromValue(new PieceList(retVal));
    }
    DiceRoll* roll() { return _roll; }
    void setIndex(int index);
    int index() const { return _index; }
    int score() const { return _score; }
    QList<AnalysisMove*> moves() { return _moves; }
    Piece* getOffPiece(int color);
    QList<QVariant> getMoves() const;

    bool canMoveAllMoves();
    QList<int> positionsForColor(int color);
    Piece* pieceAt(int position);

    int whitePointsLeft() const { return _whitePointsLeft; }
    int blackPointsLeft() const { return _blackPointsLeft; }
private:
    QList<Piece*> _pieces;
    DiceRoll* _roll = nullptr;
    double _evalScore = 0;
    int _index = 0;
    QList<AnalysisMove*> _moves;
    int _score;
    int _whitePointsLeft = 0;
    int _blackPointsLeft = 0;
};

class MoveSequence : public QObject
{
    Q_OBJECT

public:

    MoveSequence();
    MoveSequence(QList<Position*> Positions);

    QList<Position*> positions() { return _positions; };
    bool addPosition(Position* move);

    bool containsMove(Position* position);
    int score() { return _score; }
    void setScore(int score);
private:
    QList<Position*> _positions;
    int _score;
};
#endif // ANALTYPES_H
