#include "puzzledata.h"

#include <QVariant>
#include <QDebug>

PuzzleLocation::PuzzleLocation(int position, bool isWhite)
{
    _position = position;
    _isWhite = isWhite;
}

PuzzleMove::PuzzleMove(int from, int to, int roll, bool isWhite, int step)
{
    _from = from;
    _to = to;
    _roll = roll;
    _isWhite = isWhite;
    _step = step;
}

PuzzleData::PuzzleData(QString name, int index, int difficulty, bool isWhite) : _name(name), _index(index), _difficulty(difficulty), _isWhite(isWhite)
{
    _isWhite = isWhite;
}

int Puzzle::steps()
{
    int maxStep = 0;
    for (int i = 0; i < _moves.size(); i++)
    {
        qDebug() << "Step: " << _moves.at(i).step();
        if (_moves.at(i).step() > maxStep)
            maxStep = _moves.at(i).step();
    }
    return maxStep;
}

void PuzzleData::addPuzzleMove(PuzzleMove* move)
{
    _moves.append(move);
}

void PuzzleData::addPuzzleLocation(PuzzleLocation* location)
{
    _locations.append(location);
}

Puzzle::Puzzle(PuzzleData* data)
{
    _data = data;
}

QList<PuzzleMove> Puzzle::movesAtStep(int step)
{
    qDebug() << "moves at step: " << step;
    QList<PuzzleMove> retVal;
    for (int i =0; i < _moves.size(); i++)
    {
        if (_moves.at(i).step() == step)
        {
//            qDebug() << "roll: " << _moves.at(i).roll();
            retVal.append(_moves.at(i));
        }
    }
    return retVal;
}

bool Puzzle::checkMoveOnStep(int step, int move, int fromPos, int isWhite)
{
    QList<PuzzleMove> retVal;
//    qDebug() << "Checking against : " << move << " - " << fromPos << " - " << isWhite <;
    for (int i =0; i < _moves.size(); i++)
    {
        qDebug() << "_moves.at(i).step(): " << _moves.at(i).step();
        qDebug() << "step: " << step;
        if (_moves.at(i).step() == step)
        {
            qDebug() << "Checking : " << _moves.at(i).roll() << " - " << _moves.at(i).from() << " - " << _moves.at(i).isWhite();
            if (move == _moves.at(i).roll() && fromPos == _moves.at(i).from() && ((isWhite == _moves.at(i).isWhite() ? 1: 0) || isWhite == 2))
                return true;
        }
    }
    return false;
}

void Puzzle::setMoves(QList<PuzzleMove> moves)
{
    _moves = moves;
}

void Puzzle::setPieces(QList<QVariant> pieces)
{
//    _pieces = pieces;

    for (int i = 0; i < _pieces.size(); i++)
    {
        _pieces.append(pieces.at(i).value<Piece*>());
    }
}


