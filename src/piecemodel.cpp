#include "piecemodel.h"

#include "database/move.h"
#include "analysis/analysis.h"
#include "analysis/positionmodel.h"

#include "database/computergame.h"

#include <QDateTime>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>

GameMove::GameMove(Piece* piece, int diceVal, int fromPosition, bool isWhite,int turn)
{
    _piece = piece;
    if (diceVal < 0)
        diceVal = diceVal*-1;
    _diceVal = diceVal;
    _fromPosition = fromPosition;
    _isWhite = isWhite;
    //    _removedPiece = removedPiece;
    _turn = turn;
}

PieceModel::PieceModel(QObject *parent) : QObject(parent)
{
    resetPositions();
    _generator = QRandomGenerator(QDateTime::currentSecsSinceEpoch());

}

void PieceModel::setTurnFinished(bool finished)
{
    _turnFinished = finished;
    emit turnFinishedChanged();
}

void PieceModel::setStarted(bool started)
{
    _started = started;
    emit startedChanged();
}

bool PieceModel::checkTook()
{
    for (int i =0 ; i < _positions.size(); i++)
    {
        if (_positions.at(i)->position() == -1 && _positions.at(i)->rawPosition() != -1)
        {
            return true;
        }
    }
    return false;
}

void PieceModel::setWhoseDouble(int whoseDouble)
{
    _whoseDouble = whoseDouble;
    emit whoseDoubleChanged();
}

// calculate likelyhood of point being taken, how close other pieces are (piece < 7 away from opposing piece that could take it should lose much more)
// increase this for each unique dice roll that can hit the piece
int PieceModel::calculateTakeRisk(int piecePosition, bool isWhite)
{
    int takeRisk = 0;
    if (isWhite) // black
    {
        for (int i = piecePosition; i > 0; i--)
        {
            if (!getPiece(i, false, 1).isNull())
            {
                int distance = (piecePosition - i);
                if (distance < 12)
                {
                    int risk = 7 - distance;
                    if (risk < 0)
                        risk *= -1;
                    takeRisk += 7 - risk;
                } else { // add double risk
                    if (distance%3==0)
                        takeRisk += 1;
                    if (distance%4==0)
                        takeRisk += 1;
                    if (distance%5==0)
                        takeRisk += 1;
                    if (distance%6==0)
                        takeRisk += 1;
                    // check if distance divisible by double starting with 3
                    // if divisible, add 1 to take risk, unlikely hits
                }
            }
        }
    } else { // white
        for (int i = piecePosition; i < 25; i++)
        {
            if (!getPiece(i, false, 0).isNull())
            {
                int distance = (piecePosition - i);
                if (distance < 12)
                {
                    int risk = 7 - distance;
                    if (risk < 0)
                        risk *= -1;
                    takeRisk += risk;
                } else {
                    if (distance%3==0)
                        takeRisk += 1;
                    if (distance%4==0)
                        takeRisk += 1;
                    if (distance%5==0)
                        takeRisk += 1;
                    if (distance%6==0)
                        takeRisk += 1;
                    // check if distance divisible by double starting with 3
                    // if divisible, add 1 to take risk, unlikely hits
                }
            }
        }
    }
    qDebug() << "calculated take risk: " << takeRisk;
    return takeRisk;
}

// calculate potential next moves that are made available from having a piece as a blot, allows computer to make moves that still have some risk even though the piece could be taken to allow more dynamic play, remove this on lower difficulty levels
int PieceModel::calculatePotential(int piecePosition, bool isWhite)
{
 // check how many friendly blocks are within 5 positions of this piece, this should give a rough idea of how many new points can be made, amount of pieces shouldn't matter because we already remove score for individual pieces elsewhere
}

void PieceModel::setActionDatabase(QVariant actionDatabase)
{
    _actionDb = actionDatabase.value<ActionDatabase*>();
}

QList<QVariant> PieceModel::getPieces()
{
    QList<QVariant> retVal;
    for (int i = 0; i < _positions.size(); i++)
    {
        retVal.append(QVariant::fromValue(_positions.at(i)));
    }

    return retVal;
}

void PieceModel::setCurrentSide(bool currentSide)
{
    _whiteTurn = currentSide;
    emit currentSideChanged();
}

QList<QVariant> PieceModel::moves()
{
    QList<int> moves = _moves;

    for (int i = _usedMoves.size() - 1; i >= 0; i--)
    {
        //        if (moves.contains())
        moves.removeOne(_usedMoves.at(i).move());
    }

    QList<QVariant> retVal;
    for (int i = 0; i < moves.size(); i++)
    {
        retVal.append(moves.at(i));
    }

    return retVal;
}

QList<QVariant> PieceModel::displayDice()
{
    QList<int> moves = _displayDice;

    for (int i = _usedMoves.size() - 1; i >= 0; i--)
    {
        if (moves.contains(_usedMoves.at(i).move()))
            moves.removeOne(_usedMoves.at(i).move());
    }

    QList<QVariant> retVal;
    for (int i = 0; i < moves.size(); i++)
    {
        retVal.append(moves.at(i));
    }

    return retVal;
}

void PieceModel::userFinishedTurn(bool addAction)
{
    if (addAction)
        _actionDb->insertAction(_matchId, _gameId, 0, 0, _whiteTurn, ActionTypes::TurnFinish, 0, _isComputer);
    setTurnFinished(false);
    emit syncTurnFinished();
    clearMoves();
}

void PieceModel::setDisplayDice(int dice1, int dice2)
{
    if (_displayDice.size() == 0)
    {
        _displayDice.append(dice1);
        _displayDice.append(dice2);
        emit movesChanged();
    }
}

void PieceModel::confirmMoves()
{
    for (int i = 0; i < _positions.size(); i++)
    {
        _positions.at(i)->confirmMove();
    }
    emit movesChanged();
    emit positionsChanged();
}

void PieceModel::endTurn()
{
    _currentTurn++;
    for (int i = 0; i < _positions.size(); i++)
    {
        _positions.at(i)->undo();
    }

    emit positionsChanged();
}

int PieceModel::pips(bool side)
{
    int retVal = 0;

    for (int i = 0; i < _positions.size(); i++)
    {
        if (!_positions.at(i)->inRack())
        {
            if (_positions.at(i)->isWhite() == side)
            {
                if (_positions.at(i)->isWhite())
                {
                    retVal += (25 - _positions.at(i)->position());
                        // close to 24
                } else  {
                    retVal += _positions.at(i)->position();
                    // close to 0
                }
            }
        }
    }
    return retVal;
}

bool PieceModel::isBlocked(int isWhite, int position)
{
    QList<QVariant> pieces = getPieces(position);
    if (pieces.size() > 1)
    {
        if (pieces.at(0).value<Piece*>()->isWhite() == isWhite)
        {
            return true;
        }
    }

    return false;
}

bool PieceModel::isBlot(int isWhite, int position)
{
    QList<QVariant> pieces = getPieces(position);
    if (pieces.size() == 1)
    {
        if (pieces.at(0).value<Piece*>()->isWhite() == isWhite)
        {
            return true;
        }
    }
    return false;
}

bool PieceModel::isHighestPiece(bool isWhite, int position)
{
    //    25 = white
    //    0 = black
    int highestPositionFound = isWhite ? 25 : 0;
    for (int i = 0; i < _positions.size(); i++)
    {
        if (_positions.at(i)->isWhite() == isWhite)
        {
            if (isWhite == 0)
            {
                if (_positions.at(i)->position() > highestPositionFound)
                    highestPositionFound = _positions.at(i)->position();
            } else {
                if (_positions.at(i)->position() < highestPositionFound)
                    highestPositionFound = _positions.at(i)->position();
            }

        }
    }

    return highestPositionFound == position;
}

void PieceModel::loadPieces(QList<QVariant> pieces)
{
    _positions.clear();
    for (int i = 0; i < pieces.size(); i++)
    {
        //        qDebug() << "Adding piece: " << pieces.at(i).value<Piece*>()->position();
        _positions.append(pieces.at(i).value<Piece*>());
    }
    emit positionsChanged();
}

void PieceModel::loadMoves(QList<QVariant> moves)
{
    for (int i = 0; i < moves.size(); i++)
    {
        setStarted(true);
        PieceMove* move = moves.at(i).value<PieceMove*>();

        endTurn();
        loadMove(getPiece(move->fromPos()), move->move());
    }
    emit positionsChanged();
}

int PieceModel::tempMove(int fromPos, bool isMyTurn, bool fromServer, int isWhiteTurn)
{
    for (int i = 0; i < moves().size(); i++)
    {
        int move = moves().at(i).toInt();
        Piece* piece = getPiece(fromPos, false, !fromServer ? isWhiteTurn : 2).value<Piece*>();

        int toPosition = 0;
        int took = false;
        bool wasOff = false;
        if (piece != nullptr)
        {
            if (!piece->isOff())
            {
                move = (piece->isWhite() ? move : move*-1);
            }

            //            toPosition = piece->position() + move;
            if (!piece->isOff()) // move piece on board
            {
                toPosition = piece->position() + move;
                if (toPosition == -1)
                    toPosition = -2;
            }
            else // bring piece back on
            {
                toPosition = (piece->isWhite() ? move : 25 - move);
                wasOff = true;
            }
            bool moved = false;
            if (toPosition > 0 && toPosition < 25)
            {
                QList<QVariant> piecesAtPosition = getPieces(toPosition);
                int sideAtPosition = 2;
                if (piecesAtPosition.size() != 0)
                {
                    sideAtPosition = piecesAtPosition.at(0).value<Piece*>()->isWhite() ? 1 : 0;
                }

                if (hasPieceOff() && !piece->isOff()) // Force user to move piece thats off first
                {
                    return 0;
                }

                if (piecesAtPosition.size() < 2 || sideAtPosition == (piece->isWhite() ? 1 : 0)) // Regular Move
                {
                    took = checkTake(piecesAtPosition, piece, sideAtPosition, true);
                    removeDice(piece->id(), move, piece->position(), took);
                    if (piece->isOff())
                    {
                        _actionDb->insertAction(_matchId, _gameId, piece->position(), toPosition + 1, piece->isWhite(), ActionTypes::Move, took, _isComputer);
                        piece->tempMove(toPosition + 1);
                    }
                    else {
                        _actionDb->insertAction(_matchId, _gameId, piece->position(), move, piece->isWhite(), ActionTypes::Move, took, _isComputer);
                        piece->tempMove(move);
                    }
                    moved = true;
                }
            } else if (allInHome())
            {
                // white home 25
                // black home 0
                //                qDebug() << "all home move: " << toPosition << " Highest piece: " << isHighestPiece(piece->isWhite(), piece->position());
                if (((piece->isWhite() && toPosition >= 25) || (!piece->isWhite() && toPosition <= 0)) || isHighestPiece(piece->isWhite(), piece->position()))
                {
                    //                    qDebug() << "removing piece, move: " << move;
                    removeDice(piece->id(), move, piece->position());
                    _actionDb->insertAction(_matchId, _gameId, piece->position(), piece->isWhite() ? 50 : -50, piece->isWhite(), ActionTypes::Move, 0, _isComputer);
                    piece->tempMove(piece->isWhite() ? 50 : -50);
                    moved = true;
                }
            }

            if (moved)
            {
                checkMoves(isMyTurn);
                emit positionsChanged();

                if (moves().size() > 0)
                {
                    if (!canMove(isWhite()) || allOff())
                    {
                        _turnFinished = true;
                        emit turnFinishedChanged();
                        emit noMoves();
                    }
                } else {
                    _turnFinished = true;
                    emit turnFinishedChanged();
                }

                if (!fromServer)
                {
                    int fromPosition = fromPos;
                    if (wasOff)
                    {
                        fromPosition = piece->isWhite() ? 0 : 25;
                        if (!piece->isWhite()) move *= -1;
                        emit sendMove(-1, fromPosition + move, isWhite(), took);

                    } else {
                        emit sendMove(fromPos, toPosition, isWhite(), took);
                    }

                }
                return toPosition;
            }
        }
    }
    return 0;
}

int PieceModel::analysisMove(int fromPos, int move)
{
    qDebug() << "checking analysis move, from: " << fromPos << " move: " << move;
    Piece* piece = getPiece(fromPos, false).value<Piece*>(); // Somehow from pos is empty? moved twice?

    int toPosition = 0;
    if (piece != nullptr)
    {
        if (!piece->isOff())
        {
            move = (piece->isWhite() ? move : move*-1);
        }

        toPosition = piece->position() + move;
        if (!piece->isOff()) // move piece on board
        {
            toPosition = piece->position() + move;
            if (toPosition == -1)
                toPosition = -2;
        }
        else // bring piece back on
        {
            toPosition = (piece->isWhite() ? move : 25 - move);
        }

        bool moved = false;
        if (toPosition > 0 && toPosition < 25)
        {
            QList<QVariant> piecesAtPosition = getPieces(toPosition);
            int sideAtPosition = 2;
            if (piecesAtPosition.size() != 0)
            {
                sideAtPosition = piecesAtPosition.at(0).value<Piece*>()->isWhite() ? 1 : 0;
            }

            if (hasPieceOff() && !piece->isOff()) // Force user to move piece thats off first
            {
                qDebug() << "no move 1";
                return 0;
            }

            if (piecesAtPosition.size() < 2 || sideAtPosition == (piece->isWhite() ? 1 : 0)) // Regular Move
            {
                int take = checkTake(piecesAtPosition, piece, sideAtPosition, true);
                removeDice(piece->id(), move, fromPos, take);
                //                took = checkTake(piecesAtPosition, piece, sideAtPosition, true);
                if (piece->isOff())
                {
                    piece->tempMove(toPosition + 1);
                    //                    _movesThisGame.append(new GameMove(piece, toPosition + 1, fromPos, piece->isWhite(), _currentTurn));
                }
                else {
                    piece->tempMove(move);
                    //                    _movesThisGame.append(new GameMove(piece, move, fromPos, piece->isWhite(), _currentTurn));
                }

                moved = true;
            } else {
                qDebug() << "no move 2";
                return 0;
            }
        } else if (allInHome())
        {
            qDebug() << "exact move to rack: " << piece->exactMoveToRack(move) << " from: " << piece->position() << " move: " << move;
            if (piece->exactMoveToRack(move) || isHighestPiece(piece->isWhite(), piece->position()))
            {
                removeDice(piece->id(), move, fromPos);
                piece->tempMove(piece->isWhite() ? 50 : -50);
//                piece->tempMove(100);
                moved = true;
                toPosition = piece->position();

            }
        }

        if (moved)
        {

            emit positionsChanged();

            if (moves().size() > 0)
            {
                if (!canMove(isWhite()))
                {
                    _turnFinished = true;
                    emit turnFinishedChanged();
                    emit noMoves();
                }
            } else {
                _turnFinished = true;
                emit turnFinishedChanged();
            }
            qDebug() << "moved: " << toPosition;
            return toPosition;
        }
    }

    qDebug() << "no move 3";
    return 0;
}

int PieceModel::checkTake(QList<QVariant> piecesAtPosition, Piece* takingPiece, int sideAtPosition, bool temp)
{
    if (piecesAtPosition.size() == 1)
    {
        if ((sideAtPosition != (takingPiece->isWhite() ? 1 : 0) || sideAtPosition == 2)) { // Take piece
            Piece* pieceToTake = piecesAtPosition.at(0).value<Piece*>();
            if (temp)
                pieceToTake->tempMove((pieceToTake->position() + 1)*-1); // take piece off
            else
                pieceToTake->setPosition(-1); // take piece off

            qDebug() << "found taken piece: " << pieceToTake->id();
            return pieceToTake->id();
        }
    }
    return -1000;
}

bool PieceModel::loadMove(QVariant pieceVar, int to)
{
    Piece* piece = pieceVar.value<Piece*>();

    if (piece)
    {
        QList<QVariant> piecesAtPosition = getPieces(to);
        int sideAtPosition = 2;
        if (piecesAtPosition.size() != 0)
        {
            sideAtPosition = piecesAtPosition.at(0).value<Piece*>()->isWhite() ? 1 : 0;
        }

        checkTake(piecesAtPosition, piece, sideAtPosition, false);
        piece->setPosition(to);
        emit positionsChanged();
        return true;
    }

    return false;
}

bool PieceModel::loadUndo(QVariant pieceVar, int move, int context)
{
    Piece* piece = pieceVar.value<Piece*>();

    if (piece)
    {
        endTurn();
    }

    return false;
}

void PieceModel::checkFinished()
{
    int piecesInHome = 0;
    int notInHome = 0;
    for (int i = 0; i < _positions.size(); i++)
    {

        if (_positions.at(i)->isWhite() == isWhite() && _positions.at(i)->inRack())
        {
            piecesInHome++;
        } else {
            notInHome++;
        }
    }
    if (piecesInHome == 15)
    {
        emit wonGame(isWhite());
    }
}

bool PieceModel::undoMoveAction()
{
    if (_actionDb != nullptr)
        _actionDb->insertAction(_matchId, _gameId, 0,0,1,ActionTypes::Undo,0, _isComputer);
    return undoFunction();
}

bool PieceModel::undoLastMoveAction()
{
    if (_usedMoves.size() > 0)
    {
        for (int i = 0; i < _positions.size() ;i++)
        {
            if (_positions.at(i)->id() == _usedMoves.last().pieceId())
            {
                qDebug() << "found piece for undo";
                if (_usedMoves.last().takenPieceId() != -1000)
                {
                    for (int j = 0; j < _positions.size() ;j++)
                    {
                        qDebug() << "checking for take for piece: " << _positions.at(j)->id();
                        if (_positions.at(j)->id() == _usedMoves.last().takenPieceId())
                        {
                            qDebug() << "Undoing taken piece: " << _usedMoves.last().takenPieceId();
                            _positions.at(j)->undoLastMove();
                        }
                    }
                }
                _positions.at(i)->undoLastMove();
                _usedMoves.removeLast();
                _movesThisGame.removeLast();
                break;
            }
        }

        _turnFinished = false;
        emit turnFinishedChanged();
        emit positionsChanged();
        emit movesChanged();
        emit undid();
        return true;
    }
    return false;
}

bool PieceModel::clearMoves()
{
    _turnFinished = false;
    emit turnFinishedChanged();
    _usedMoves.clear();
    _displayDice.clear();
    _moves.clear();
    emit movesChanged();
    return true;
}

void PieceModel::removePiece(int fromPosition)
{
    QList<QVariant> pieces;
    for (int i = 0; i < _positions.size(); i++)
    {
        Piece* current = _positions.at(i);
        if (current->position() == fromPosition)
        {
            _positions.removeAt(i);
            return;
        }
    }
}

QList<QVariant> PieceModel::getPieces(int position, int color)
{
    QList<QVariant> pieces;
    for (int i = 0; i < _positions.size(); i++)
    {
        Piece* current = _positions.at(i);
        if (current->position() == position)
        {
            if (color == 2 || color == current->isWhite() ? 1 : 0)
                pieces.append(QVariant::fromValue(current));
        }
    }

    return pieces;
}

QList<QVariant> PieceModel::getOffPieces(int color)
{
    QList<QVariant> pieces;
    for (int i = 0; i < _positions.size(); i++)
    {
        Piece* current = _positions.at(i);
        if (current->rawPosition() == -1 && current->position() == 0)
        {
            if ((current->isWhite() ? 1 : 0) == color || color == 2)
                pieces.append(QVariant::fromValue(current));
        }
    }

    return pieces;
}

QVariant PieceModel::getPiece(int position, bool onlyUnmoved, int color)
{
    for (int i = 0; i < _positions.size(); i++)
    {
        Piece* current = _positions.at(i);
        if (position == -1 && current->rawPosition() == -1)
        {
            if (color == 2 || color == current->isWhite())
            {
                if (onlyUnmoved)
                    return QVariant::fromValue(current);
                else if (current->position() == 0 && !onlyUnmoved)
                {
                    return QVariant::fromValue(current);
                }
            }
        }
        if (current->position() == position)
        {
            return QVariant::fromValue(current);
        }
    }

    return QVariant();
}

void PieceModel::resetPositions()
{
    _positions.clear();
    _positions.append(new Piece(0, 1, true, this));
    _positions.append(new Piece(1, 1, true, this));

    _positions.append(new Piece(2, 6, false, this));
    _positions.append(new Piece(3, 6, false, this));
    _positions.append(new Piece(4, 6, false, this));
    _positions.append(new Piece(5, 6, false, this));
    _positions.append(new Piece(6, 6, false, this));

    _positions.append(new Piece(7, 8, false, this));
    _positions.append(new Piece(8, 8, false, this));
    _positions.append(new Piece(9, 8, false, this));

    _positions.append(new Piece(10, 12, true, this));
    _positions.append(new Piece(11, 12, true, this));
    _positions.append(new Piece(12, 12, true, this));
    _positions.append(new Piece(13, 12, true, this));
    _positions.append(new Piece(14, 12, true, this));

    _positions.append(new Piece(15, 13, false, this));
    _positions.append(new Piece(16, 13, false, this));
    _positions.append(new Piece(17, 13, false, this));
    _positions.append(new Piece(18, 13, false, this));
    _positions.append(new Piece(19, 13, false, this));

    _positions.append(new Piece(20, 17, true, this));
    _positions.append(new Piece(21, 17, true, this));
    _positions.append(new Piece(22, 17, true, this));

    _positions.append(new Piece(23, 19, true, this));
    _positions.append(new Piece(24, 19, true, this));
    _positions.append(new Piece(25, 19, true, this));
    _positions.append(new Piece(26, 19, true, this));
    _positions.append(new Piece(27, 19, true, this));

    _positions.append(new Piece(28, 24, false, this));
    _positions.append(new Piece(29, 24, false, this));

    _moves.clear();
    _usedMoves.clear();
    emit positionsChanged();
    emit movesChanged();
}

bool compareDice(int dicex1, int dicex2, int dicey1, int dicey2)
{
    QList<int> diceToMatch;
    diceToMatch.append(dicex1);
    diceToMatch.append(dicex2);
    // on double check this is fucked, need to ensure same dice isn't checked twice

    if (diceToMatch.contains(dicey1))
    {
        diceToMatch.removeOne(dicey1);
    }
    if (diceToMatch.contains(dicey2))
    {
        diceToMatch.removeOne(dicey2);
    }
    return diceToMatch.size() == 0;
}

void PieceModel::setDice(bool isWhite, int dice1, int dice2, bool currentAction, bool forceCalc, bool addAction)
{
    setStarted(true);
    bool calcMoves = forceCalc;
    if (moves().size() == 0)
    {
        calcMoves= true;
    } else if (!compareDice(_moves.at(0), _moves.at(1), dice1, dice2)) {
        calcMoves = true;
    }
    _usedMoves.clear();
    _lastRoll = new DiceRoll(dice1, dice2);
    if (addAction)
        _actionDb->insertAction(_matchId, _gameId, dice1, dice2, isWhite, ActionTypes::Roll, 0, _isComputer);
    if (calcMoves)
    {
        _moves.clear();
        if (dice1 == dice2)
        {
            _moves.append(dice1);
            _moves.append(dice1);
            _moves.append(dice1);
            _moves.append(dice1);
        } else {
            _moves.append(dice1);
            _moves.append(dice2);
        }
        if (currentAction)
        {
            if (isWhite == _whiteTurn)
            {
                if (!canMove(isWhite))
                {
                    _actionDb->insertAction(_matchId, _gameId, dice1, dice2, isWhite, ActionTypes::TurnFinish, 0, _isComputer);
                    _turnFinished = true;
                    emit turnFinishedChanged();
                    emit noMoves();
                }
            }

        }
        emit movesChanged();
    }
}

void PieceModel::reverseDice()
{
    std::reverse(_moves.begin(), _moves.end());
    emit movesChanged();
}

int PieceModel::calculateToPosition(bool isPieceWhite, bool isUserWhite, int fromPosition, int move)
{
    return fromPosition == -1 ? isPieceWhite ? 25 - move : move : fromPosition + (!isUserWhite ? move : move*-1);
}

bool PieceModel::inHome(int position, int color)
{
    if (color == 0 ? (position < 7 && position > 0) : ( position < 25 && position > 18))
    {
        return true;
    }
    return false;
}

int PieceModel::numPiecesOnBoard(int color)
{
    int retVal = 0;
    for (int i = 0; i < _positions.size(); i++)
    {
        if ((_positions.at(i)->isWhite() ? 1 : 0) == color && _positions.at(i)->position() > 0 && _positions.at(i)->position() < 25)
        {
            retVal++;
        }
    }
    return retVal;
}

void PieceModel::printPositions()
{
    QDebug dbg(QtDebugMsg);
    for (int i = 0; i < _positions.size(); i++)
    {
        //        dbg << "(" << _positions.at(i)->position() << ":" << _positions.at(i)->tempDiff() << ")" << ", ";
    }
}

void PieceModel::removeDice(int piece, int diceVal, int fromPos, int taken)
{
    //    qDebug() << "remove dice: " << diceVal;
    int diceFinal = 0;
    if (diceVal < 0)
        diceFinal = diceVal*-1;
    else
        diceFinal = diceVal;

    _usedMoves.append(UndoMove(piece, taken, diceFinal));
    _movesThisGame.append(new GameMove(nullptr, diceVal, fromPos, _whiteTurn, _currentTurn));

    emit movesChanged();
}

void PieceModel::checkMoves(bool isMyTurn)
{
    if (moves().size() == 0)
    {
        _turnFinished = true;
        emit turnFinishedChanged();
        emit turnFinished(isMyTurn);
    } else
        if (!canMove(isWhite()))
        {
            _turnFinished = true;
            emit turnFinishedChanged();
            emit noMoves();
        }
}

bool PieceModel::hasPieceOff()
{
    QList<QVariant> offPieces = getOffPieces();
    for (int i = 0; i < offPieces.size(); i++)
    {
        if (offPieces.at(i).value<Piece*>()->isWhite() == isWhite())
        {
            return true;
        }
    }
    return false;
}

bool PieceModel::canComeOn(bool side)
{
    for (int i = 0; i < moves().size(); i++)
    {
        QList<QVariant> pieces = getPieces(side ? moves().at(i).toInt() :  25 - moves().at(i).toInt());
        if (pieces.size() < 2)
        {
            return true;
        }
    }
    return false;
}

bool PieceModel::canMove(bool white)
{
    if (hasPieceOff())
    {
        QList<QVariant> offPieces = getOffPieces();
        for (int i = 0; i< offPieces.size(); i++)
        {
            for (int j = 0; j < moves().size(); j++)
            {
                if (canMovePiece(offPieces.at(i).value<Piece*>(), isWhite() ? moves().at(j).toInt() : 25 - moves().at(j).toInt()))
                {
                    return true;
                }
            }
        }
    } else {
        for (int i = 0; i< _positions.size(); i++)
        {
            if (_positions.at(i)->isWhite() == white)
            {
                if (hasPieceOff() && _positions.at(i)->isOff())
                {
                    for (int j = 0; j < moves().size(); j++)
                    {
                        if (canMovePiece(_positions.at(i), isWhite() ? 25 - moves().at(j).toInt() : moves().at(j).toInt()))
                        {
                            return true;
                        }
                    }
                } else if (!hasPieceOff()) {
                    for (int j = 0; j < moves().size(); j++)
                    {
                        if (canMovePiece(_positions.at(i), isWhite() ? _positions.at(i)->position() + moves().at(j).toInt() : _positions.at(i)->position() - moves().at(j).toInt()))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool PieceModel::canMovePiece(Piece* piece, int toPosition)
{
    if ((toPosition < 1) || (toPosition > 24))
    {
        //
        // need exception for if all pieces off, multiple in home, one can go off because it is say at position 2 and the dice move is 3, but there is a piece at position 4 and position 1 is blocked, therefore cannot move
        if (!allInHome())
        {
            return false;
        } else if (toPosition == 0 || toPosition == 25) {
            return true;
        } else if (isHighestPiece(piece->isWhite(), piece->position())) {
            return true;
        } else {
            return false;
        }
    }

    QList<QVariant> piecesAtPosition = getPieces(toPosition);

    if (piecesAtPosition.size() < 2 || piecesAtPosition.at(0).value<Piece*>()->isWhite() == piece->isWhite())
    {
        return true;
    }
    return false;
}

bool PieceModel::allInHome()
{
    return allInHome(isWhite());
}
bool PieceModel::allInHome(bool isWhite)
{
    for (int i = 0; i < _positions.size(); i++)
    {
        if ((isWhite == _positions.at(i)->isWhite()) && !(_positions.at(i)->inHome() || _positions.at(i)->inRack()))
        {
            return false;
        }
    }
    return true;
}

bool PieceModel::allOff()
{
    for (int i = 0; i < _positions.size(); i++)
    {
        if (!_positions.at(i)->inRack() && _positions.at(i)->isWhite() == isWhite())
        {
            return false;
        }
    }
    return true;
}

PuzzlePieceModel::PuzzlePieceModel()
{
    _moveTimer = new QTimer();
    _moveTimer->setInterval(500);
    connect(_moveTimer, &QTimer::timeout, this, &PuzzlePieceModel::handleComputerTurn);
    _started = true;
}

void PuzzlePieceModel::setPuzzle(QVariant puzzle)
{
    _puzzle = puzzle.value<Puzzle*>();
    setupStep();
}

void PuzzlePieceModel::userFinishedTurn(bool addAction)
{
    endTurn();
}

bool PuzzlePieceModel::myTurn()
{
    return _myTurn;
}

void PuzzlePieceModel::setupStep()
{
    _currentMoves = 0;

    if (_puzzle->steps() + 1 > _stepIndex)
    {
        int dice1 = -1, dice2 = -1;
        bool isWhite = false;
        QList<PuzzleMove> moves = _puzzle->movesAtStep(_stepIndex);

        for (int i = 0; i < moves.size(); i++)
        {
            isWhite = moves.at(i).isWhite();
            if (dice1 == -1)
                dice1 = moves.at(i).roll();
            else
                dice2 = moves.at(i).roll();
        }
        setDice(isWhite, dice1, dice2, moves.at(0).isWhite() == _puzzle->isWhite() ? _puzzle->isWhite() : !_puzzle->isWhite());
        if (moves.at(0).isWhite() != _puzzle->isWhite())
        {
            _moveTimer->start();
        }
    } else {
        emit puzzleFinished();
        // puzzle finished
    }
}

void PuzzlePieceModel::addScore(int value)
{
    _score += value;
    emit scoreChanged();
}

int PuzzlePieceModel::tempMove(int fromPos, bool myTurn, bool fromServer, int isWhite)
{
    for (int i = 0; i < moves().size(); i++)
    {
        if (_puzzle->checkMoveOnStep(_stepIndex, moves().at(0).toInt(), fromPos, isWhite))
        {
            _currentMoves++;
            if (myTurn)
            {
                addScore(100);
                emit correctPuzzleMove();
            }
            PieceModel::tempMove(fromPos, myTurn, fromServer, isWhite);
            return 1;
        } else if (myTurn) {
            if (myTurn)
            {
                addScore(-100);
                emit incorrectPuzzleMove();
            }
        }
    }
    return 0;
}

void PuzzlePieceModel::endTurn()
{
    _myTurn = false;
    emit myTurnChanged();
    setTurnFinished(false);
    _stepIndex++;
    setupStep();
}

void PuzzlePieceModel::handleComputerTurn()
{
    QList<PuzzleMove> moves = _puzzle->movesAtStep(_stepIndex);
    if (_computerCurrentMove < moves.size())
    {
        tempMove(moves.at(_computerCurrentMove).from(), false, false, !_puzzle->isWhite());
        _computerCurrentMove++;
    } else {
        _moveTimer->stop();
        _computerCurrentMove = 0;
        _stepIndex++;
        setupStep();
        emit myTurnChanged();
        setTurnFinished(false);
    }
}

//void PuzzlePieceModel::checkMoves(bool isMyTurn)
//{
//    if (isMyTurn)
//    {
//    if (moves().size() == 0)
//    {

//        _turnFinished = true;
//        emit turnFinishedChanged();
//        emit turnFinished(isMyTurn);
//    } else
//        if (!canMove(isWhite()))
//        {
//            _turnFinished = true;
//            emit turnFinishedChanged();
//            emit noMoves();
//        }
//    }
//}

void PieceModel::saveSgf(int gameId)
{

    QFile* saveLocation = new QFile(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/computergame.sgf");
    saveLocation->open(QFile::ReadWrite);
    //    QFile
    QString sgf = getSgf(1, "Human", "Computer", QDateTime::currentDateTime().toString(), 1, 1, _actionDb->actions(gameId));
    saveLocation->write(sgf.toStdString().c_str(),sgf.length());
    saveLocation->close();
}

QString PieceModel::getSgf(int gameId, QString player1, QString player2, QString date, int toScore, int numMatches, std::list<Action> actions)
{
    QString sgfString = "";

    int moveIndex = 1;
    int dice1 = 0, dice2 = 0;
    QList<Action*> movesMap = QList<Action*>(); //fromPos = 0, toPos = 0;
    QList<int> takesList = QList<int>();
    bool firstTurn = true;
    bool doubled = false;
    int doubleVal = 1;
    int runningScoreWhite = 0;
    int runningScoreBlack = 0;
    sgfString += "[Site \"Connor's Backgammon App\"] \r\n";
    sgfString += "[Match ID \"" + QString::number(gameId) + "\"] \r\n";
    sgfString += "[Event \"Computer Match\"] \r\n";
    sgfString += "[Round \"0\"] \r\n";
    sgfString += "[Player 1 \"" + player1 +"\"] \r\n";
    sgfString += "[Player 2 \"" + player2 +"\"] \r\n";
    sgfString += "[Event Date  \" " + date + "\"] \r\n";
    sgfString += "[Event Time  \" 00:00\"] \r\n";
    sgfString += "[Variation  \"Backgammon\"] \r\n";
    sgfString += "[Unrated  \"On\"] \r\n";
    sgfString += "[Crawford  \"Off\"] \r\n";
    sgfString += "[CubeLimit  \"1024\"] \r\n";
    sgfString += "[ClockType  \"Off\"] \r\n";
    sgfString += "\r\n";
    sgfString += toScore + " point match \r\n";
    sgfString += "\r\n";
    sgfString += "\r\n";

    for (int matchNumber = 0; matchNumber < numMatches; matchNumber++)
    {
        QStringList split;
        if (matchNumber == 0)
        {
            sgfString +="Game " + QString::number(matchNumber+1) + "\r\n";
            QString stringFrag = player1 + " : " + QString::number(runningScoreWhite);

            if (stringFrag.length() < 50)
            {
                for (int j = stringFrag.length(); j < 50; j++)
                {
                    stringFrag += " ";
                }
            }
            sgfString+= stringFrag;
            sgfString += player2 + " : " + QString::number(runningScoreBlack);
            sgfString += "\r\n";
        }
        int i = 0;
        std::list<Action>::iterator it = actions.begin();
        while (it != actions.end())
        {
            it++;
            switch (it->action())
            {
            case ActionTypes::None:
            {
                break;
            }
            case ActionTypes::StartRoll:
            {
                break;
            }
            case ActionTypes::FirstTurn:
            {
                // mark first who went first?
                break;
            }
            case ActionTypes::TurnFinish:
            {
                if (doubled)
                {
                    sgfString += "Takes ";
                    doubled = false;
                    split = sgfString.split("\r\n");
                    if (split.at(split.length()-1).toInt() < 50)
                    {
                        for (int j = split.at(split.length()-1).length(); j < 50; j++)
                        {
                            sgfString += " ";
                        }
                    }
                } else {
                    if (firstTurn)
                        sgfString += QString::number(moveIndex) + ") ";

                    sgfString += " " + QString::number(dice1) + "" + QString::number(dice2) + ":";
                    if (movesMap.size() > 0 || i == 0)
                    {
                        for (int j = 0; j < movesMap.length(); j++)
                        {
                            sgfString += " " + QString::number(movesMap.at(j)->dice1()) + "/";
                            if ((movesMap.at(j)->dice2() > 24 || movesMap.at(j)->dice2() < 1))
                                sgfString += "Off";
                            else
                                sgfString += QString::number(movesMap.at(j)->dice2());

                            //                            if (movesMap[j].matchId > 1)
                            //                            {
                            //                                sgfString +="(1)";
                            //                            }
                            //                            if (takesList[j] == 1)
                            //                            {
                            //                                sgfString += "*";
                            //                            }
                        }
                    } else {
                        sgfString += " No Move";
                    }
                    split = sgfString.split("\r\n");
                    if (split.at(split.length()-1).toInt() < 50)
                    {
                        for (int j = split.at(split.length()-1).length(); j < 50; j++)
                        {
                            sgfString += " ";
                        }
                    }
                }

                movesMap.clear();
                takesList.clear();

                // add string with saved variables

                if (!firstTurn)
                {
                    sgfString += "\r\n";
                    moveIndex++;
                }
                firstTurn = !firstTurn;
                break;
            }
            case ActionTypes::FirstTurnReroll:
            {
                break;
            }
            case ActionTypes::Roll:
            {
                dice1 = it->dice1();
                dice2 = it->dice2();
                // set dice1, dice2
                break;
            }
            case ActionTypes::Undo:
            {
                movesMap.clear();
                takesList.clear();
                break;
            }
            case ActionTypes::Move:
            {
                int matchingMoves = 0;

                for (int nextActionIndex = i + 1; nextActionIndex < i + 4 && it->action() == ActionTypes::Move && nextActionIndex < actions.size(); nextActionIndex++)
                {

                    std::list<Action>::iterator nextActionIt = actions.begin();
                    //                    nextActionIt + nextActionIndex;
                    for(int k = 0; k < nextActionIndex; k++)
                    {
                        nextActionIt++;
                    }
                    if (it->dice1() == nextActionIt->dice1() && it->dice2() == nextActionIt->dice2())
                    {
                        matchingMoves++;
                        i++;
                    }
                }
                movesMap.append(new Action(0,1,1,it->dice1(), it->dice2(), false, ActionTypes::Move, QDateTime::currentDateTime(), it->context()));
                takesList.append(it->context());
                break;
            }
            case ActionTypes::Won:
            {
                moveIndex = 0;
                sgfString+= "Wins " + QString::number(doubleVal) + " points\r\n \r\n";

                sgfString +="Game " + QString::number(matchNumber) + "\r\n";

                if (it->isWhite() == 1)
                {
                    runningScoreWhite += doubleVal;
                } else {
                    runningScoreBlack += doubleVal;
                }

                QString stringFrag = player1 + " : " + QString::number(runningScoreWhite);

                if (stringFrag.length() < 50)
                {
                    for (int j = stringFrag.length(); j < 50; j++)
                    {
                        stringFrag += " ";
                    }
                }
                sgfString+= stringFrag;
                sgfString += player2 + " : " + QString::number(runningScoreBlack);
                sgfString += "\r\n";
                doubleVal = 1;
                break;
            }
            case ActionTypes::Double:
            {
                doubleVal=doubleVal*2;
                QString doubleFrag = "";
                doubleFrag += "Doubles => ";
                doubleFrag += ("" +doubleVal);
                if (doubleFrag.length() < 50)
                {
                    for (int j = doubleFrag.length(); j < 50; j++)
                    {
                        doubleFrag += " ";
                    }
                }
                sgfString += doubleFrag;

                if (!firstTurn)
                    sgfString += "\r\n";
                else {

                }
                firstTurn = !firstTurn;
                doubled = true;
                // add double text
                break;
            }
            case ActionTypes::Finished:
            {
                // finish loop
                break;
            }
            }
        }
        sgfString += "\r\n";
        sgfString += "\r\n";
        sgfString += "\r\n";
        it++;
    }
    return sgfString;
}

ComputerPieceModel::ComputerPieceModel()
{
    resetPositions();
    _moveTimer = new QTimer();
    _analysis = new Analysis(this);
    _moveTimer->setInterval(1000);
    _isComputer = true;
    connect(_moveTimer, &QTimer::timeout, this, &ComputerPieceModel::handleComputerTurn);

}

void ComputerPieceModel::startGame()
{
    generateDice(true);
    setCurrentSide(true);
    emit positionsChanged();
}

bool ComputerPieceModel::myTurn()
{
    return _myTurn;
}

int ComputerPieceModel::tempMove(int fromPos, bool myTurn, bool fromServer, int isWhite)
{
    PieceModel::tempMove(fromPos, myTurn, true, isWhite);
    if (moves().size() == 0)
    {
        setTurnFinished(true);
    }
}

void ComputerPieceModel::generateDice(bool humanTurn, bool addAction)
{
    _usedMoves.clear();
    int rand1 = QRandomGenerator::global()->bounded(6) + 1;
    int rand2 = QRandomGenerator::global()->bounded(6) + 1;
    setDice(humanTurn, rand1, rand2, true, true, addAction);
}

void ComputerPieceModel::userFinishedTurn(bool addAction)
{
    endTurn();
    PieceModel::userFinishedTurn(addAction);
    _moveTimer->stop();
    generateDice(false);
    setCurrentSide(false);
    _currentPosition = getPieces();
    _analysisSequences = _analysis->allSequences(getPieces(), 0, QVariant::fromValue(_lastRoll));
    _computerNextPosition = _analysis->getBestAnalysisMoves(getPieces(), 0, QVariant::fromValue(_lastRoll));
    if (_computerNextPosition != nullptr)
    {
        _myTurn = false;
        emit myTurnChanged();
    } else {
        emit noMoves(true);
    }
}

void ComputerPieceModel::startComputerMoves()
{
    _moveTimer->start();
}

void ComputerPieceModel::checkSequence(int sequenceIndex)
{
    loadPieces(_analysisSequences.at(sequenceIndex)->positions().at(0)->getPieces());
}

QList<int> ComputerPieceModel::sequenceScores()
{
    QList<int> retVal;
    for (int i = 0; i < _analysisSequences.size();i++)
    {
        retVal.append(_analysisSequences.at(i)->score());
    }
    return retVal;
}

bool ComputerPieceModel::loadMoves(int gameId)
{
    bool foundMove = false;
    std::list<Action> actions = _actionDb->actions(gameId);
    std::list<Action>::iterator it = actions.begin();
    QList<AnalysisMove*> moves;
    while (it != actions.end())
    {
        if (it->action() == 7)
        {
            moves.append(new AnalysisMove(it->dice1(), it->dice1() + it->dice2()));
            //            qDebug() << "move!";
        } else if (it->action() == 5)
        {
            endTurn();

            for (int i = 0; i < moves.size(); i++)
            {
                //                qDebug() << "Load move: " << moves.at(i)->from() << " move: " << moves.at(i)->move();
                loadMove(getPiece(moves.at(i)->from()), moves.at(i)->to());
            }
            moves.clear();
        } else if (it->action() == 6)
        {
            moves.clear();
        } else if (it->action() == 4)
        {
            setDice(it->isWhite(), it->dice1(), it->dice2(), true, false, false);
            setCurrentSide(it->isWhite());
            moves.clear();
        }
        it++;
        foundMove = true;
    }

    if (!_whiteTurn && moves.size() > 0)
    {
        _currentPosition = getPieces();
        _analysisSequences = _analysis->allSequences(getPieces(), 0, QVariant::fromValue(_lastRoll));
        _computerNextPosition = _analysis->getBestAnalysisMoves(getPieces(), 0, QVariant::fromValue(_lastRoll));
        if (_computerNextPosition != nullptr)
        {
            _myTurn = false;
            emit myTurnChanged();
        } else {
            emit noMoves(true);
        }
    }
    emit positionsChanged();

    return foundMove;
}

bool PieceModel::loadPreviousMoves(int game, QVariant positionModel)
{
    bool foundMove = false;
    std::list<Action> actions = _actionDb->actions(game);
    std::list<Action>::iterator it = actions.begin();
    QList<AnalysisMove*> moves;
    positionModel.value<PositionModel*>()->addPosition(Position(_positions, moves, true, 0));
    while (it != actions.end())
    {
        if (it->action() == 7)
        {
            moves.append(new AnalysisMove(it->dice1(), it->dice1() + it->dice2()));
            //            qDebug() << "move!";
        } else if (it->action() == 5)
        {
            endTurn();

            for (int i = 0; i < moves.size(); i++)
            {
                //                qDebug() << "Load move: " << moves.at(i)->from() << " move: " << moves.at(i)->move();
                loadMove(getPiece(moves.at(i)->from()), moves.at(i)->to());
            }
            positionModel.value<PositionModel*>()->addPosition(Position(_positions, moves, true, it->id()));
            moves.clear();
        } else if (it->action() == 6)
        {
            moves.clear();
        } else if (it->action() == 4)
        {
            setDice(it->isWhite(), it->dice1(), it->dice2(), true, false, false);
            moves.clear();
        }
        it++;
        foundMove = true;
    }

    //    positionModel.value<PositionModel*>()->sort(4, Qt::DescendingOrder);
    emit positionsChanged();
}

void PieceModel::reloadPosition(int id, int lastId)
{
    bool foundMove = false;
    std::list<Action> actions = _actionDb->actionsTo(id, lastId);
    std::list<Action>::iterator it = actions.begin();
    //    positionModel.value<PositionModel*>()->addPosition(Position(_positions, true));
    QList<AnalysisMove*> moves;
    while (it != actions.end())
    {
        if (it->action() == 7)
        {
            moves.append(new AnalysisMove(it->dice1(), it->dice1() + it->dice2()));
            //            qDebug() << "move!";
        } else if (it->action() == 5)
        {
            endTurn();

            for (int i = 0; i < moves.size(); i++)
            {
                //                qDebug() << "Load move: " << moves.at(i)->from() << " move: " << moves.at(i)->move();
                loadMove(getPiece(moves.at(i)->from()), moves.at(i)->to());
            }
            moves.clear();
        } else if (it->action() == 6)
        {
            moves.clear();
        } else if (it->action() == 4)
        {
            setDice(it->isWhite(), it->dice1(), it->dice2(), true, false, false);
            moves.clear();
        }
        it++;
        foundMove = true;
    }

    emit positionsChanged();
}

void ComputerPieceModel::endTurn()
{
    _moveTimer->stop();
    for (int i = 0; i < _positions.size(); i++)
    {
        _positions.at(i)->confirmMove();
    }

    checkFinished();
    emit positionsChanged();
}

void ComputerPieceModel::computerTurnFinish(bool addAction)
{
    _computerCurrentMove = 0;
    endTurn();
    _myTurn = true;
    if (addAction)
        _actionDb->insertAction(_matchId, _gameId, 0, 0, false, ActionTypes::TurnFinish, 0, _isComputer);
    setCurrentSide(true);
    generateDice(true, addAction);
    emit myTurnChanged();
    setTurnFinished(false);
    _moveTimer->stop();
}

void ComputerPieceModel::handleComputerTurn()
{
    if (_computerNextPosition)
    {
        if (_computerNextPosition->moves().size() > _computerCurrentMove)
        {
            tempMove(_computerNextPosition->moves().at(_computerCurrentMove)->from(), false, true, false);
            _computerCurrentMove++;
        } else {
            computerTurnFinish();
        }
    }
}

bool ComputerPieceModel::undoMoveAction()
{
    if (_actionDb != nullptr)
        _actionDb->insertAction(_matchId, _gameId, 0,0,1,ActionTypes::Undo,0, _isComputer);
    return undoFunction();
}
