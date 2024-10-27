#include "analtypes.h"

AnalysisMove::AnalysisMove(AnalysisMove* move)
{
    _from = move->from();
    _to = move->to();
}

AnalysisMove::AnalysisMove(int from, int to)
{
    _from = from;
    _to = to;
}

int AnalysisMove::move()
{
    int retVal = _to - _from;
    if (retVal < 0)
        retVal *= -1;
    return retVal;
}

Position::Position(QList<Piece*> pieces, bool copy)
{
    if (!copy)
    {
        _pieces = pieces;
    } else {
        for (int i = 0; i < pieces.size(); i++)
        {
            _pieces.append(new Piece(pieces.at(i)->id(),pieces.at(i)->rawPosition(),pieces.at(i)->tempDiff(),pieces.at(i)->isWhite()));
        }
    }
}

Position::Position(QList<Piece*> pieces, QList<AnalysisMove*> moves, bool copy, int index)
{
    if (!copy)
    {
        _pieces = pieces;
    } else {
        for (int i = 0; i < pieces.size(); i++)
        {
            _pieces.append(new Piece(pieces.at(i)->id(),pieces.at(i)->rawPosition(),pieces.at(i)->tempDiff(),pieces.at(i)->isWhite()));
        }
    }
    for (int i = 0; i < moves.size(); i++)
    {
        _moves.append(new AnalysisMove(moves.at(i)));
    }
    _index = index;
}

Position::Position(QList<Piece*> pieces, QList<AnalysisMove*> moves, int score)
{
    for (int i = 0; i < pieces.size(); i++)
    {
        _pieces.append(new Piece(pieces.at(i)->id(),pieces.at(i)->rawPosition(),pieces.at(i)->tempDiff(),pieces.at(i)->isWhite()));
    }
    for (int i = 0; i < moves.size(); i++)
    {
        _moves.append(new AnalysisMove(moves.at(i)));
    }
    _score = score;
}

Position::Position(QList<Piece*> pieces, QList<QVariant> moves, int score, int whitePointsLeft, int blackPointsLeft)
{
    for (int i = 0; i < pieces.size(); i++)
    {
        _pieces.append(new Piece(pieces.at(i)->id(),pieces.at(i)->rawPosition(),pieces.at(i)->tempDiff(),pieces.at(i)->isWhite()));
    }
    _score = score;
    for (int i = 0; i < moves.size(); i++)
    {
        _moves.append(new AnalysisMove(moves.at(i).value<AnalysisMove*>()));
    }
    _whitePointsLeft = whitePointsLeft;
    _blackPointsLeft = blackPointsLeft;
}

QList<QVariant> Position::getPieces()
{
    QList<QVariant> retVal;
    for (int i = 0; i < _pieces.size(); i++)
    {
        retVal.append(QVariant::fromValue(_pieces.at(i)));
    }
    return retVal;
}

void Position::setIndex(int index)
{
    _index = index;
}

Piece* Position::getOffPiece(int color)
{
    for (int i = 0; i < _pieces.size(); i++)
    {
        if (_pieces.at(i)->rawPosition() == -1 && _pieces.at(i)->position() == 0 && (_pieces.at(i)->isWhite() ? 1 : 0) == color)
        {
            return _pieces.at(i);
        }
    }
    return nullptr;
}

QList<QVariant> Position::getMoves() const
{
    QList<QVariant> moves;
    for (int i = 0 ; i < _moves.size(); i++)
    {
        moves.append(QVariant::fromValue(_moves.at(i)));
    }
    return moves;
}

bool Position::canMoveAllMoves()
{
    for (int i = 0; i < _pieces.size(); i++)
    {
        // return false;
    }
    return true;
}

QList<int> Position::positionsForColor(int color)
{
    QList<int> columns;
    for (int i = 0; i < _pieces.size(); i++)
    {
        Piece* piece = _pieces.at(i);
        qDebug() << "rawPosition: " << piece->rawPosition() << " Position: " << piece->position();
        if (piece->position() == 0)
        {
            if ((piece->isWhite() ? 1 : 0) == color)
            {
                if (!columns.contains(-1))
                {
                    columns.append(-1);
                    break;
                }
            }
        }
    }

    for (int i = 1; i < 25; i++)
    {
        Piece* piece = pieceAt(i);
        if (piece !=  nullptr)
        {
            if ((piece->isWhite() ? 1 : 0) == color)
            {
                if (i < 25)
                    columns.append(i);
            }
        }
    }

    if (color == 1)
    {
        std::reverse(columns.begin(), columns.end());
    }


    return columns;
}

Piece* Position::pieceAt(int position)
{
    for (int i = 0; i < _pieces.size(); i++)
    {
        if (_pieces.at(i)->position() == position)
        {
            return _pieces.at(i);
        }
    }
    return nullptr;
}

MoveSequence::MoveSequence()
{

}

MoveSequence::MoveSequence(QList<Position*> positions)
{
    _positions = positions;
}

bool MoveSequence::addPosition(Position* position)
{
    bool added = false;
    if (!containsMove(position))
    {
        added = true;
        _positions.append(position);
    }
    else {
//        qDebug() << "didn't add position because it is already in the list";
    }
    return true;
}

bool MoveSequence::containsMove(Position* position)
{
    int found = 0;
//    qDebug() << "initial position start";
//    for (int k = 0; k < position->moves().size(); k++)
//    {
//        qDebug() << "position->moves().at(k)->from(): " << position->moves().at(k)->from() << " position->moves().at(k)->to(): " << position->moves().at(k)->to();
//    }
//    qDebug() << "initial position finish";
    int movesCount = position->moves().size();
    for (int i = 0; i < _positions.size(); i++)
    {
        QList<AnalysisMove*> moves = _positions.at(i)->moves();
//        qDebug() << "Existing position start";
        for (int j = 0; j < moves.size(); j++)
        {
//            qDebug() << "moves.at(j)->from(): " << moves.at(j)->from() << " moves.at(j)->to(): " << moves.at(j)->to();
            for (int k = 0; k < position->moves().size(); k++)
            {
                if (moves.at(j)->from() == position->moves().at(k)->from() && moves.at(j)->to() == position->moves().at(k)->to())
                {
//                    qDebug() << "Existing position found";
                    found++;
                }
            }
        }

        if (found == movesCount)
        {
//            qDebug() << "Found move";
            return true;
        } else {
            found = 0;
        }
//        qDebug() << "Existing position finish";
    }

//    qDebug() << "Didnt find position";
    return false;
}

void MoveSequence::setScore(int score)
{
    _score = score;
}
