#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QObject>
#include <QVariant>

#include "src/piecemodel.h"
#include "diceroll.h"

#include "analtypes.h"

class PositionModel;

class Analysis : public QObject
{
    Q_OBJECT

//    Q_PROPERTY(QVariant game READ game WRITE setGame NOTIFY gameChanged)


public:
    explicit Analysis(QObject *parent = nullptr);

    bool AcceptDoubling();
    MoveSequence* ToLocalSequence(MoveSequence* sequence);
    QList<Piece*> DoSequence(MoveSequence* sequence);
    MoveSequence* GeneratePossiblePositions(Position* previousPosition, DiceRoll* roll, int color);
    Position* BestMove(Position* previousPosition, DiceRoll* roll, int color);
    void UndoSequence(MoveSequence* sequence, QList<Piece*> hits);
    double EvaluatePoints(int myColor, QList<Piece*> pieces);
    double pointsNotInHome(int myColor, QList<Piece*> pieces);
    double EvaluateCheckers(int myColor, QList<Piece*> pieces);
    double ProbabilityScore(int myColor);
    Position* GeneratePosition(Position* previousPosition, DiceRoll* roll);
    QList<MoveSequence*> generateAnalysis(QList<Piece*> startingPosition, int color, DiceRoll* startRoll = nullptr);
    bool positionAnalysis(MoveSequence* sequence, Position previousPosition, DiceRoll* roll, int color, QList<int> diceMoves, QList<AnalysisMove*> moves, int runningScore, int lastColumnIndex, bool base = false);
    MoveSequence* movePiece(int columnIndex, Position previousPosition, DiceRoll* roll, QList<int> diceMoves, QList<AnalysisMove*> moves, int runningScore, int color, bool base, MoveSequence* sequence);
    void GenerateMovesSequence(int myColor, QList<MoveSequence*> sequences, MoveSequence* moves, int diceIndex);
    double Evaluate(int color, QList<Piece*> pieces);
    bool isBlocked(QList<Piece*> pieces, int isWhite, int position);
    bool isBlot(QList<Piece*> pieces, int isWhite, int position);
    int pointsLeft(int color, QList<Piece*> pieces);

    Position* getBestAnalysisMoves(QList<QVariant> positionVar, int color, QVariant diceRoll);
//    void setGame(QVariant game);
    PieceModel* getGame() { return _game; }
    QVariant game() { return QVariant::fromValue(_game); }

private:
    PieceModel* _game;
    bool _first = true;
    int _depth = 0;

signals:
    void gameChanged();
    void positionAnalised(QList<QVariant> moves, int score, int whitePointsLeft, int blackPointsLeft);
    void finished();

public slots:
    QList<MoveSequence*> allSequences(QList<QVariant> positionVar, int color, QVariant diceRoll);
    Q_INVOKABLE QVariant getBestMoves(QList<QVariant> positionVar, int color, QVariant diceRoll);
    void analiseMoves(QList<QVariant> positionVar, int color);
};

#endif // ANALYSIS_H
