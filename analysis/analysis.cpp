 #include "analysis.h"

#include <QDebug>

Analysis::Analysis(QObject *parent)
    : QObject{parent}
{
    _game = new PieceModel(parent);
}
Position* Analysis::getBestAnalysisMoves(QList<QVariant> positionVar, int color, QVariant diceRoll)
{
    QList<Piece*> pieces;
    DiceRoll* roll = diceRoll.value<DiceRoll*>();
    for (int i = 0; i < positionVar.size(); i++)
    {
        pieces.append(positionVar.at(i).value<Piece*>());
    }
    Position* position = new Position(pieces);
    return BestMove(position, roll, color);
}

QList<MoveSequence*> Analysis::allSequences(QList<QVariant> positionVar, int color, QVariant diceRoll)
{
    QList<Piece*> pieces;
    DiceRoll* roll = diceRoll.value<DiceRoll*>();
    for (int i = 0; i < positionVar.size(); i++)
    {
        pieces.append(positionVar.at(i).value<Piece*>());
    }
    Position* position = new Position(pieces);

    return generateAnalysis(pieces, color, roll);
}

QVariant Analysis::getBestMoves(QList<QVariant> positionVar, int color, QVariant diceRoll)
{
    MoveSequence* bestMoveSequence = nullptr;
    int bestScore = 0;
    QList<Piece*> pieces;
    DiceRoll* roll = diceRoll.value<DiceRoll*>();
    for (int i = 0; i < positionVar.size(); i++)
    {
        pieces.append(positionVar.at(i).value<Piece*>());
    }
    Position* position = new Position(pieces);
    //    return BestMove(position, roll, color);
    QList<MoveSequence*> allSequences = generateAnalysis(position->pieces(), color, roll);

    for (int i =0; i <allSequences.size(); i++)
    {
        if (allSequences.at(i)->score() > bestScore)
        {
            bestScore = allSequences.at(i)->score();
            bestMoveSequence = allSequences.at(i);
        }
    }

    MoveSequence* retVal = new MoveSequence();
    retVal->addPosition(position);
    bestScore = 0;
    int bestScoreIndex = 0;

    if (bestMoveSequence)
    {
        for (int i = 0; i < bestMoveSequence->positions().size(); i++)
        {
            if (bestMoveSequence->positions().at(i)->score() > bestScore)
            {
                bestScore = bestMoveSequence->positions().at(i)->score();
                bestScoreIndex = i;
            }
        }
        retVal->addPosition(bestMoveSequence->positions().at(bestScoreIndex));
    }

    return QVariant::fromValue(retVal);
}

void Analysis::analiseMoves(QList<QVariant> positionVar, int color)
{
    QList<Piece*> pieces;
    for (int i = 0; i < positionVar.size(); i++)
    {
        pieces.append(positionVar.at(i).value<Piece*>());
    }
    _game->setPositions(pieces);
    Evaluate(color, _game->pieces());
    //    positionAnalysis();
}

int Analysis::pointsLeft(int color, QList<Piece*> pieces)
{
    int blackPointsLeft = 0;
    int whitePointsLeft = 0;
    for (int i = 0; i < pieces.size(); i++)
    {

        if (pieces.at(i)->isWhite())
        {
            whitePointsLeft += pieces.at(i)->position() == 0 ? 24 : 25 - pieces.at(i)->position();
        } else {
            blackPointsLeft += pieces.at(i)->position() == 0 ? 24 : pieces.at(i)->position();
        }
    }
    return color ? whitePointsLeft : blackPointsLeft;
}

double Analysis::EvaluatePoints(int myColor, QList<Piece*> pieces)
{
    int blackPointsLeft = pointsLeft(0, pieces);
    int whitePointsLeft = pointsLeft(1, pieces);
    if (myColor == 0)
    {
        //        qDebug() << "returning point score: " << (whitePointsLeft - blackPointsLeft);
        return whitePointsLeft - blackPointsLeft ;
    }
    else
    {
        //        qDebug() << "returning point score: " << (blackPointsLeft - whitePointsLeft);
        return blackPointsLeft - whitePointsLeft;
    }
}

double Analysis::pointsNotInHome(int myColor, QList<Piece*> pieces)
{
    int blackPointsLeft = 0;
    int whitePointsLeft = 0;
    for (int i = 0; i < pieces.size(); i++)
    {

        if (pieces.at(i)->isWhite())
        {
            if (!pieces.at(i)->inHome())
                whitePointsLeft += pieces.at(i)->position() == 0 ? 24 : 25 - pieces.at(i)->position();
        } else {
            if (!pieces.at(i)->inHome())
                blackPointsLeft += pieces.at(i)->position() == 0 ? 24 : pieces.at(i)->position();
        }
    }
    return myColor ? whitePointsLeft : blackPointsLeft;
}

double Analysis::EvaluateCheckers(int myColor, QList<Piece*> pieces)
{
    int score = 0;
    bool inBlock = false;
    int blockCount = 0; // consequtive blocks
    double bf = 1.5;//Blots Factor; // reducing this number below 1 increase score loss, increasing it reduces score loss
    double bfp = 2.5;//Blots Factor Passed; // reducing this number below 1 increase score loss, increasing it reduces score loss
    double bps = 3;//Blocked Point Score; // increasing this increases the amount of score for each 'blocked point'
    double cbf = 2;//Connected Blocks Factor; // blocked point score is increase by this power
    double opp = 50;//Off Piece Penalty // score to remove for off piece
    double runOrBlock = 1;
    //    BlotsFactor = 1,
    //    BlotsFactorPassed = 1,
    //    BlotsThreshold = 0,
    //    BlockedPointScore = 0,
    //    ConnectedBlocksFactor = 0,
    //    ProbablityScore = false,
    //    RunOrBlockFactor = 0

    int other = myColor == 0 ? 1 : 0;
    // Oponents checker closest to their bar. Relative to my point numbers.
    int opponentMax = 0;
    int opponentMin = 25;
    int myMin = 25;
    int myMax = 0;

    for (int i = 0; i < pieces.size(); i++)
    {
        if (pieces.at(i)->isWhite() == myColor)
        {
            if (myMin > pieces.at(i)->position())
                myMin = pieces.at(i)->position();
            if (myMax < pieces.at(i)->position())
                myMax = pieces.at(i)->position();
        } else {
            if (opponentMax < pieces.at(i)->position())
                opponentMax = pieces.at(i)->position();
            if (opponentMin > pieces.at(i)->position())
                opponentMin = pieces.at(i)->position();
        }
    }
    bool allPassed = true;
    int opponentOffPieces = _game->getOffPieces(other).size();
    int myOffPieces = _game->getOffPieces(myColor).size();

    score += opponentOffPieces*opp;
    score -= myOffPieces*opp;
    allPassed = (myColor == 1 ? myMin < opponentMax : opponentMin < myMax) && opponentOffPieces == 0;
    if (allPassed)
    {
        for (int i = 1; i < 25; i++)
        {
            // It is important to reverse looping for white.
            int piecePosition =  i;
            if (myColor == 0)
                piecePosition = 25 - i;


            // If all opponents checkers has passed this point, blots are not as bad.
            if (isBlocked(pieces, myColor, piecePosition))
            {
                if (inBlock)
                    blockCount++;
                else
                    blockCount = 1; // Start of blocks.
                inBlock = true;
            }
            else // not a blocked point
            {
                if (inBlock)
                {
                    score += pow(blockCount * bps, cbf);
                    blockCount = 0;
                }
                inBlock = false;
                if (isBlot(pieces, myColor, piecePosition))
                {
                    score -= _game->calculateTakeRisk(piecePosition, myColor) / (allPassed ? bfp : bf);
                    if (_game->inHome(piecePosition, myColor) && !allPassed)
                    {
                        score -= 5;
                    }

//                        if (difficulty > 5)
                        score += _game->calculatePotential(piecePosition, myColor);
                    // calculate potential next moves value, chances of creating new points etc and add extra score back on
                }
            }
        }

        if (inBlock) // the last point.
        {
            score += pow(blockCount * bps, cbf);
        }

        if (allPassed)
        {
            score += EvaluatePoints(myColor, pieces) * runOrBlock;
            score -= pointsNotInHome(myColor, pieces);
        }
    }
    else // once passed it is MOST important to get all pieces in home first
    {
        if (!_game->allInHome())
        {
            for (int i = 0; i < _game->pieces().size(); i++)
            {
                if (!_game->inHome(_game->pieces().at(i)->position(), myColor))
                {
                    score -= 50*(myColor == 0 ? _game->pieces().at(i)->position() : 25 - _game->pieces().at(i)->position());
                }
            }
        }

        score -= _game->numPiecesOnBoard(myColor) * 50;// favour moves off
    }

    return score;
}

bool Analysis::isBlocked(QList<Piece*> pieces, int isWhite, int position)
{
    int piecesAtPosition = 0;
    for (int i = 0; i < pieces.size(); i++)
    {
        if (pieces.at(i)->position() == position && pieces.at(i)->isWhite() == isWhite)
        {
            piecesAtPosition++;
        }
    }
    //    qDebug() << "pieces at position: " << piecesAtPosition << " position: " << position;
    return piecesAtPosition > 1;
}

bool Analysis::isBlot(QList<Piece*> pieces, int isWhite, int position)
{
    int piecesAtPosition = 0;
    for (int i = 0; i < pieces.size(); i++)
    {
        if (pieces.at(i)->position() == position && pieces.at(i)->isWhite() == isWhite)
        {
            piecesAtPosition++;
        }
    }
    return piecesAtPosition == 1;
}

//Get the average score for current player rolling all possible combinations
double Analysis::ProbabilityScore(int myColor)
{
    //        var allDiceRoll = AllRolls();
    //        var scores = new List<double>();
    //        var oponent = myColor == Player.Color.Black ? Player.Color.White : Player.Color.Black;
    //    foreach (var roll in allDiceRoll)
    //    {
    //        game.FakeRoll(roll.dice1, roll.dice2);
    //        var bestScore = double.MinValue;
    //        var seqs = GenerateMovesSequence(game);
    //        foreach (var s in seqs)
    //        {
    //            var hits = DoSequence(s, game);
    //            var score = EvaluatePoints(myColor, game) + EvaluateCheckers(myColor, game);
    //            score -= EvaluateCheckers(oponent, game);
    //            if (score > bestScore)
    //                bestScore = score;
    //            UndoSequence(s, hits, game);
    //        }
    //        int m = roll.dice1 == roll.dice2 ? 1 : 2; // dice roll with not same value on dices are twice as probable. 2 / 36, vs 1 / 36
    //        if (seqs.Any())
    //            scores.Add(bestScore * m);
    //        // Get best score of each roll, and make an average.
    //        // some rolls are more probable, multiply them
    //        // some rolls will be blocked or partially blocked
    //    }
    //    if (!scores.Any())
    //        return -100000; // If player cant move, shes blocked. Thats bad.
    //    return scores.Average();
    return 0;
}

QList<DiceRoll*> allRolls()
{
    QList<DiceRoll*> rolls;
    for (int d1 = 1; d1 < 7; d1++)
    {
        for (int d2 = 1; d2 < 7; d2++)
        {
            rolls.append(new DiceRoll(d1, d2));
        }
    }
    return rolls;
}

QList<int> getMoves(DiceRoll* roll)
{
    QList<int> diceMoves;
    if (roll->dice1() == roll->dice2())
    {
        diceMoves.append(roll->dice1());
        diceMoves.append(roll->dice1());
        diceMoves.append(roll->dice1());
        diceMoves.append(roll->dice1());
    } else {
        diceMoves.append(roll->dice1());
        diceMoves.append(roll->dice2());
    }
    return diceMoves;
}

MoveSequence* Analysis::GeneratePossiblePositions(Position* previousPosition, DiceRoll* roll, int color)
{

    _game->setPositions(previousPosition->pieces());
    MoveSequence* sequence = new MoveSequence();
    int runningScore = 0;
    Piece* lastPiece = nullptr;
    QList<int> diceMoves = getMoves(roll);

    QList<AnalysisMove*> moves;
    _first = true;
    positionAnalysis(sequence, *previousPosition, roll, color, diceMoves, moves, runningScore, 0, true);

    _game->undoMoveAction();
    sequence->setScore(runningScore);
    return sequence;
}

bool Analysis::positionAnalysis(MoveSequence* sequence, Position previousPosition, DiceRoll* roll, int color, QList<int> diceMoves, QList<AnalysisMove*> moves, int runningScore, int lastColumnIndex, bool base)
{
    QList<int> columnsWithMyPieces = previousPosition.positionsForColor(color);
    //    _game->setPositions(previousPosition.pieces());
    qDebug() << "columnsWithMyPieces: " << columnsWithMyPieces;

    if (columnsWithMyPieces.contains(-1)) // has off piece, move this first
    {
        qDebug() << "checking off piece <--------";
        QList<int> diceMovesCopy(diceMoves);
        Piece* piece = previousPosition.getOffPiece(color);
        if (piece)
        {
            qDebug() << "got off piece";
            int origPiecePositon = piece->position();

            for (int moveIndex = 0 ; moveIndex < diceMovesCopy.size(); moveIndex++)
            {
                if (_game->analysisMove(-1, diceMovesCopy.at(moveIndex)) > 0)
                {
                    moves.append(new AnalysisMove(origPiecePositon, diceMovesCopy.at(moveIndex)));
                    if (diceMovesCopy.size() > 0)
                    {
                        diceMovesCopy.removeAt(moveIndex);
                    }

                    int score = Evaluate(color, _game->pieces());
                    runningScore += score;

                    QList<QVariant> movesVar;
                    for (int moveIndex = 0 ; moveIndex < moves.size(); moveIndex++)
                    {
                        movesVar.append(QVariant::fromValue(moves.at(moveIndex)));
                    }

                    _depth++;
                    if (diceMovesCopy.size() == 0 && !base)
                    {
                        qDebug() << "no dice left at end of tree: " << moves.size() << " getMoves(roll).size(): " << getMoves(roll).size();
                        _game->printPositions();
                        if (moves.size() == getMoves(roll).size())
                        {
                            if (sequence->addPosition(new Position(_game->pieces(),moves, score)))
                                emit positionAnalised(movesVar, score, pointsLeft(1, _game->pieces()), pointsLeft(0, _game->pieces()));
                        }
                    } else {
                        if (!positionAnalysis(sequence, Position(_game->pieces(), moves, runningScore), roll, color, diceMovesCopy, moves, runningScore, -1))
                        {
                            if (moves.size() > 0 && sequence->positions().size() == 0)
                            {
                                //                                qDebug() << "Adding incomplete position because none other available: " << score << " moves: " << moves << " positions: " << sequence->positions();
                                _game->printPositions();
                                if (sequence->addPosition(new Position(_game->pieces(),moves, score)))
                                    emit positionAnalised(movesVar, score, pointsLeft(1, _game->pieces()), pointsLeft(0, _game->pieces()));
                            }
                        } else {
                            if (sequence->addPosition(new Position(_game->pieces(),moves, score)))
                                emit positionAnalised(movesVar, score, pointsLeft(1, _game->pieces()), pointsLeft(0, _game->pieces()));
                        }
                    }
                    _game->undoLastMoveAction();
                    moves.removeLast();
                }
            }

        }
    } else {
        qDebug() << "regular move";
        for (int columnIndex = 0; columnIndex < columnsWithMyPieces.size(); columnIndex++)
        {
            QList<int> diceMovesCopy(diceMoves);
            Piece* piece = previousPosition.pieceAt(columnsWithMyPieces.at(columnIndex));
            if (piece)
            {
                int origPiecePositon = piece->position();

                for (int moveIndex = 0 ; moveIndex < diceMovesCopy.size(); moveIndex++)
                {
                    if (_game->analysisMove(piece->position(), diceMovesCopy.at(moveIndex)) != 0)
                    {
                        moves.append(new AnalysisMove(origPiecePositon, diceMovesCopy.at(moveIndex)));
                        if (diceMovesCopy.size() > 0)
                        {
                            diceMovesCopy.removeAt(moveIndex);
                        }

                        int score = Evaluate(color, _game->pieces());
                        runningScore += score;

                        QList<QVariant> movesVar;
                        for (int moveIndexCopy = 0 ; moveIndexCopy < moves.size(); moveIndexCopy++)
                        {
                            movesVar.append(QVariant::fromValue(moves.at(moveIndexCopy)));
                        }


                        if (diceMovesCopy.size() == 0 && !base)
                        {
                            _game->printPositions();
                            if (moves.size() == getMoves(roll).size())
                            {
                                if (sequence->addPosition(new Position(_game->pieces(),moves, score)))
                                    emit positionAnalised(movesVar, score, pointsLeft(1, _game->pieces()), pointsLeft(0, _game->pieces()));
                            }
                        } else {
                            if (!positionAnalysis(sequence, Position(_game->pieces(), moves, runningScore), roll, color, diceMovesCopy, moves, runningScore, columnIndex))
                            {
                                if (moves.size() > 0 && sequence->positions().size() == 0)
                                {
                                    _game->printPositions();
                                    if (sequence->addPosition(new Position(_game->pieces(),moves, score)))
                                        emit positionAnalised(movesVar, score, pointsLeft(1, _game->pieces()), pointsLeft(0, _game->pieces()));
                                }
                            } else {
                                if (sequence->addPosition(new Position(_game->pieces(),moves, score)))
                                    emit positionAnalised(movesVar, score, pointsLeft(1, _game->pieces()), pointsLeft(0, _game->pieces()));
                            }
                        }

                        _game->undoLastMoveAction();
                        moves.removeLast();
                    }

                }
            }
        }
    }
    _depth--;
    //    _game->undoLastMoveAction();
    //    qDebug() << "Searched all columns";
    return false;
}

Position* Analysis::BestMove(Position* previousPosition, DiceRoll* roll, int color)
{
    int bestScoreIndex = 0;
    MoveSequence* sequence = GeneratePossiblePositions(previousPosition, roll, color);
    if (sequence->positions().size() > 0)
    {
        for (int  i = 1; i < sequence->positions().size(); i++)
        {
            //            qDebug() << "checking move all moves";
            //            for (int j = 0; j < sequence->positions().at(i)->getMoves().size(); j++)
            //            {
            //                qDebug() << "from: " << sequence->positions().at(i)->getMoves().at(j).value<AnalysisMove*>()->from() << " move: " << sequence->positions().at(i)->getMoves().at(j).value<AnalysisMove*>()->move();
            //            }
            //            qDebug() << "done checking moves: " << sequence->positions().at(i)->score();
            if (sequence->positions().at(i)->score() > sequence->positions().at(bestScoreIndex)->score())
            {
                bestScoreIndex = i;
            }
        }

        return sequence->positions().at(bestScoreIndex);
    }
    return nullptr;
}


Position* Analysis::GeneratePosition(Position* previousPosition, DiceRoll* roll)
{

}

QList<MoveSequence*> Analysis::generateAnalysis(QList<Piece*> startingPosition, int color, DiceRoll* startRoll)
{
    QList<MoveSequence*> moves;
    Position* pos = new Position(startingPosition, QList<AnalysisMove*>(),0);
    if (startRoll == nullptr)
    {
        QList<DiceRoll*> rolls = allRolls(); // check all possible dice rolls
        for (int i = 0; i < rolls.size(); i++)
        {
            moves.append(GeneratePossiblePositions(pos, rolls.at(i), color));
        }
    } else {
        moves.append(GeneratePossiblePositions(pos, startRoll, color));
    }
    return moves;
}

double Analysis::Evaluate(int color, QList<Piece*> pieces)
{
    //    qDebug() << "EvaluatePoints: " << EvaluatePoints(color, pieces) <<  " for color: " << color;
    return EvaluatePoints(color, pieces) + EvaluateCheckers(color, pieces);
}

bool Analysis::AcceptDoubling()
{
    //    if (!EngineGame.PlayersPassed())
    //        return true;

    return true;
    //    var myScore = Evaluate(EngineGame.CurrentPlayer, EngineGame);
    //    var oponent = EngineGame.CurrentPlayer == Player.Color.Black ? Player.Color.White : Player.Color.Black;
    //    var otherScore = Evaluate(oponent, EngineGame);

    //    var oppPips = EngineGame.CurrentPlayer == Player.Color.White ?
    //        EngineGame.BlackPlayer.PointsLeft :
    //        EngineGame.WhitePlayer.PointsLeft;

    //    var k = (myScore - otherScore) / oppPips;

    //    return k > -0.25; // Just my best guess.
}
