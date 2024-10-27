#ifndef PIECEMODEL_H
#define PIECEMODEL_H

#include <QObject>
#include <QVariant>
#include <QRandomGenerator>
#include <QTimer>

#include "src/piece.h"
#include "puzzles/puzzledata.h"
#include "diceroll.h"
#include "analysis/analtypes.h"

#include "database/actiondatabase.h"

class Analysis;
class Position;
//class PositionModel;

class UndoMove {
public:
    UndoMove(int piece, int taken, int move) { _pieceId = piece; _takenId = taken; _move = move; }
    int pieceId() { return _pieceId; }
    int takenPieceId() { return _takenId; }
    int move() const { return _move; }

private:
    int _pieceId;
    int _takenId;
    int _move;
};

class GameMove : public QObject
{
    Q_OBJECT

public:
    GameMove(Piece* piece, int diceVal, int fromPosition, bool isWhite, int turn);

    Piece* piece() { return _piece; }
    int diceVal() { return _diceVal; }
    int fromPosition() { return _fromPosition; }
    bool removedPiece() { return _removedPiece; }
    bool isWhite() { return _isWhite; }
    int turn() { return _turn; }

private:
    Piece* _piece;
    int _diceVal;
    int _fromPosition;
    bool _removedPiece = false;
    bool _isWhite = false;
    bool _turn = false;
};

class PieceModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isWhite READ isWhite WRITE setCurrentSide NOTIFY currentSideChanged)
    Q_PROPERTY(bool started READ started NOTIFY startedChanged)
    Q_PROPERTY(int whoseDouble READ whoseDouble WRITE setWhoseDouble NOTIFY whoseDoubleChanged) // 0 == noone double, 1 == white doubled last, 2 == black doubled last
    Q_PROPERTY(bool isTurnFinished READ isTurnFinished WRITE setTurnFinished NOTIFY turnFinishedChanged)
    Q_PROPERTY(QList<QVariant> moves READ moves NOTIFY movesChanged)
    Q_PROPERTY(QList<QVariant> displayDice READ displayDice NOTIFY movesChanged)
    Q_PROPERTY(QList<QVariant> pieces READ getPieces NOTIFY piecesChanged)

    Q_PROPERTY(QVariant actionDatabase READ actionDatabase WRITE setActionDatabase NOTIFY actionDatabaseChanged)

public:
    explicit PieceModel(QObject *parent = nullptr);

    void setPositions(QList<Piece*> pieces) { _positions = pieces; }

    bool isWhite() { return _whiteTurn; }
    bool started() { return _started; }
    bool isTurnFinished() { return _turnFinished; }
    void setTurnFinished(bool finished);
    void setStarted(bool started);

    void setCurrentSide(bool currentSide);

    int whoseDouble() { return _whoseDouble; }
    void setWhoseDouble(int whoseDouble);

    // analysisFunctions
    int calculateTakeRisk(int piecePosition, bool isWhite);
    int calculatePotential(int piecePosition, bool isWhite);
    // end analysisFunctions

    QVariant actionDatabase() { return QVariant::fromValue(_actionDb); }
    void setActionDatabase(QVariant actionDatabase);

    Q_INVOKABLE void saveSgf(int id);

    QList<QVariant> getPieces();
    QList<Piece*> pieces() { return _positions; }
    QList<QVariant> moves();
    QList<QVariant> displayDice();
    Q_INVOKABLE bool loadPreviousMoves(int game, QVariant positionModel);
    Q_INVOKABLE void reloadPosition(int id, int lastId);
    Q_INVOKABLE virtual void userFinishedTurn(bool addAction = true);
    Q_INVOKABLE void setDisplayDice(int dice1, int dice2);
    Q_INVOKABLE void confirmMoves();
    Q_INVOKABLE bool checkTook();
    Q_INVOKABLE virtual void endTurn();
    Q_INVOKABLE QVariant getLastRoll() { return QVariant::fromValue(_lastRoll);}
    Q_INVOKABLE int pips(bool side);

    bool isBlocked(int isWhite, int position);
    bool isBlot(int isWhite, int position);
    bool isHighestPiece(bool isWhite, int position);

    Q_INVOKABLE void loadPieces(QList<QVariant> pieces);
    Q_INVOKABLE void loadMoves(QList<QVariant> moves);
    Q_INVOKABLE bool loadMove(QVariant pieceVar, int to);
    Q_INVOKABLE virtual int tempMove(int fromPos, bool myTurn, bool fromServer, int isWhite = 2);
    Q_INVOKABLE int analysisMove(int fromPos, int move);
    int checkTake(QList<QVariant> piecesAtPosition, Piece* piece, int sideAtPosition, bool temp);
//    Q_INVOKABLE void movePiece(int gameId, QVariant piece, int move, bool removeDice = true);
    bool loadUndo(QVariant pieceVar, int move, int context);
    Q_INVOKABLE virtual bool undoMoveAction();
    Q_INVOKABLE virtual bool undoLastMoveAction();

    bool undoFunction()
    {
        //    qDebug() << "undoing moves";
        _usedMoves.clear();
        PieceModel::endTurn();
        _turnFinished = false;
        emit turnFinishedChanged();
        emit positionsChanged();
        emit movesChanged();
        emit undid();
        return true;
    }
    Q_INVOKABLE bool clearMoves();
    Q_INVOKABLE void removePiece(int fromPosition);
    Q_INVOKABLE QList<QVariant> getPieces(int position, int color = 2);
    Q_INVOKABLE virtual QList<QVariant> getOffPieces(int color = 2);
    Q_INVOKABLE QVariant getPiece(int position, bool onlyUnmoved = true, int color = 2);
    Q_INVOKABLE void resetPositions();
    Q_INVOKABLE void setDice(bool isWhite, int dice1, int dice2, bool currentAction, bool forceCalc = false, bool addAction = true);
    Q_INVOKABLE void reverseDice();
    Q_INVOKABLE int calculateToPosition(bool isPieceWhite, bool isUserWhite, int fromPosition, int move);

    bool inHome(int position, int color);
    int numPiecesOnBoard(int color);
    bool allInHome();
    bool allInHome(bool isWhite);

    void printPositions();

    Q_INVOKABLE void setMatchId(int id) { _matchId = id; }
    Q_INVOKABLE void setGameId(int id) { _gameId = id; }

protected:
    QList<Piece*> _positions; // -1 for taken off the baord by opponent, 1-24 board position, otherwise taken off by player

    bool _whiteTurn = false;
    bool _started = false;
    bool _turnFinished = false;

    int _whoseDouble = 0;
    int _currentTurn = 0;
    int _gameId = 0;
    int _matchId = 0;

    bool _isComputer = false;

    QRandomGenerator _generator;

    DiceRoll* _lastRoll = nullptr;

    QList<int> _moves;
    QList<int> _displayDice;
    QList<UndoMove> _usedMoves;
    QList<GameMove*> _movesThisGame; // store moves in case of undo (old position, dice val)

    void removeDice(int piece, int diceVal, int fromPos, int taken = -1000);
    void checkMoves(bool isMyTurn);
    bool hasPieceOff();
    bool canComeOn(bool side);
    bool canMove(bool side);
    bool canMovePiece(Piece* piece, int toPosition);

    bool allOff();
    void checkFinished();

    ActionDatabase* _actionDb = nullptr;

    QString getSgf(int gameId, QString player1, QString player2, QString date, int toScore, int numMatches, std::list<Action> moves);

signals:
    void currentSideChanged();
    void movesChanged();
    void noMoves(bool isComputer = false);
    void undoMove(int gameId, int currentPos);
    void undid();
    void turnFinishedChanged();
    void movedPiece(int gameId, int position, int move);
    void positionsChanged();
    void piecesChanged();
    void startedChanged();
    void turnFinished(bool isMyTurn);
    void wonGame(bool isWhite);
    void whoseDoubleChanged();
    void sendMove(int fromPos, int toPos, bool isWhite, bool took);
    void boardMove(int fromPos, bool myTurn, bool fromServer, bool isWhite);
    void syncTurnFinished();

    void actionDatabaseChanged();
};

class PuzzlePieceModel : public PieceModel {
    Q_OBJECT

    Q_PROPERTY(QVariant puzzle READ puzzle WRITE setPuzzle NOTIFY puzzleChanged)
    Q_PROPERTY(bool myTurn READ myTurn NOTIFY myTurnChanged)
    Q_PROPERTY(int score READ score NOTIFY scoreChanged)

public:
    PuzzlePieceModel();

    Q_INVOKABLE virtual int tempMove(int fromPos, bool myTurn, bool fromServer, int isWhite = 2) override;

    QVariant puzzle() { return QVariant::fromValue(_puzzle); }
    void setPuzzle(QVariant puzzle);

    bool myTurn();
    virtual void userFinishedTurn(bool addAction = true) override;
    void setupStep();

    int score() { return _score; }
    void addScore(int value);

    virtual void endTurn() override;

private:
    int _stepIndex = 0;
    Puzzle* _puzzle = nullptr;
    int _currentMoves = 0;
    int _computerCurrentMove = 0;
    QTimer* _moveTimer;
    bool _myTurn = true;
    int _score = 0;

signals:
    void puzzleChanged();
    void correctPuzzleMove();
    void incorrectPuzzleMove();
    void myTurnChanged();
    void scoreChanged();
    void puzzleFinished();

private slots:
    void handleComputerTurn();
};

class ComputerPieceModel : public PieceModel {
    Q_OBJECT

    Q_PROPERTY(bool myTurn READ myTurn NOTIFY myTurnChanged)
public:
    ComputerPieceModel();

    bool myTurn();
    Q_INVOKABLE virtual int tempMove(int fromPos, bool myTurn, bool fromServer, int isWhite = 2) override;

    Q_INVOKABLE void userFinishedTurn(bool addAction = true) override;
    Q_INVOKABLE void startComputerMoves();

    Q_INVOKABLE void startGame();

    Q_INVOKABLE void generateDice(bool humanTurn, bool addAction = true);
    virtual void endTurn() override;

    Q_INVOKABLE void computerTurnFinish(bool addAction = true);
//    Q_INVOKABLE virtual QList<QVariant> getOffPieces();
    Q_INVOKABLE bool undoMoveAction() override;
//    bool undoLastMoveAction(int fromPos);

    Q_INVOKABLE void checkSequence(int sequenceIndex);
    Q_INVOKABLE QList<int> sequenceScores();

    Q_INVOKABLE bool loadMoves(int game);

    Q_INVOKABLE int lastTurnColor(int id) { return _actionDb->lastTurnColor(id); }
private:
    QTimer* _moveTimer = nullptr;
    Analysis* _analysis = nullptr;
    Position* _computerNextPosition = nullptr;
    QList<QVariant> _currentPosition;
    int _computerCurrentMove = 0;
    bool _myTurn = true;
    QList<MoveSequence*> _analysisSequences;

signals:
    void myTurnChanged();
private slots:
    void handleComputerTurn();
};

#endif // PIECEMODEL_H
