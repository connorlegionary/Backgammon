#include "playbackmodel.h"

#include "src/database/move.h"

PlaybackModel::PlaybackModel(QObject *parent)
    : QObject{parent}
{

}

void PlaybackModel::setMoves(QList<QVariant> moves)
{
    _moves = moves;
}

void PlaybackModel::setPieceModel(QVariant model)
{
    _pieceModel = model.value<PieceModel*>();
}

void PlaybackModel::previousMove()
{
    if (_currentMove > 0)
    {
       resetPosition();

       _currentMove--;
       for (int i = 0; i < _currentMove; i++)
       {
           PieceMove* pieceMove = _moves.at(i).value<PieceMove*>();
           _pieceModel->loadMove(_pieceModel->getPiece(pieceMove->fromPos(), false), pieceMove->move());
       }
        emit currentMoveChanged();
    }
}

void PlaybackModel::nextMove()
{
    if (_currentMove < _moves.size())
    {
        PieceMove* pieceMove = _moves.at(_currentMove).value<PieceMove*>();
        if (_currentMove != 0)
        {
            if (pieceMove->matchId() != _moves.at(_currentMove-1).value<PieceMove*>()->matchId())
            {
                resetPosition();
            }
        }
        _pieceModel->loadMove(_pieceModel->getPiece(pieceMove->fromPos(), false), pieceMove->move());
        _currentMove++;
        emit currentMoveChanged();
    }
}

void PlaybackModel::resetPosition()
{
    _pieceModel->resetPositions();
}

