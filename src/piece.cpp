#include "piece.h"

Piece::Piece(int id, int position, bool isWhite, QObject* parent) : QObject(parent)
{
    _id = id;
    _position = position;
    _isWhite = isWhite;
}

Piece::Piece(int id, int rawPosition, int tempDiff, bool isWhite, QObject* parent)
{
    _id = id;
    _position = rawPosition;
    _tempDiff = tempDiff;
    _isWhite = isWhite;
}

bool Piece::containsTempMove(int move)
{
//    qDebug() << "piece temp moves stored: " << _tempMoves;
    for (int i = 0 ; i < _tempMoves.size(); i++)
    {
        if (_tempMoves.at(i) == move || _tempMoves.at(i)*-1 == move)
        {
            return true;
        }
    }
    return false;
}
