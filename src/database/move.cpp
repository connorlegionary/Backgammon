#include "move.h"

PieceMove::PieceMove(int id, int gameId, int fromPos, int move, int action, int context, bool isWhite, QDateTime time, int matchId, QObject *parent) : QObject(parent)
{
    _id = id;
    _gameId = gameId;
    _matchId = matchId;
    _fromPos = fromPos;
    _move = move;
    _action = action;
    _context = context;
    _isWhite = isWhite;
    _time = time;
}

PieceMove::PieceMove()
{

}


void PieceMove::setId(int id)
{
    _id = id;
}

void PieceMove::setGameId(int gameId)
{
    _gameId = gameId;
}

void PieceMove::setMatchId(int matchId)
{
    _matchId = matchId;
}

void PieceMove::setFromPos(int fromPos)
{
    _fromPos = fromPos;
}

void PieceMove::setMove(int move)
{
    _move = move;
}

void PieceMove::setAction(int action)
{
    _action = action;
}

void PieceMove::setIsWhite(bool isWhite)
{
    _isWhite = isWhite;
}

void PieceMove::setTime(QDateTime time)
{
    _time = time;
}
