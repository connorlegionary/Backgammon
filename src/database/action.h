#ifndef ACTION_H
#define ACTION_H

#include <QDateTime>

enum ActionTypes{
    None = 0,
    StartRoll,
    FirstTurn,
    FirstTurnReroll,
    Roll,
    TurnFinish,
    Undo,
    Move,
    Won,
    Double,
    Finished,
};


class Action
{
public:
    Action();
    Action(int id, int matchId, int gameId, int dice1, int dice2, int isWhite, int action, QDateTime time, int context);

    int id() { return _id; }
    int matchId() { return _matchId; }
    int gameId() { return _gameId; }
    int dice1() { return _dice1; }
    int dice2() { return _dice2; }
    bool isWhite() { return _isWhite; }
    int action() { return _action; }
    QDateTime time() { return _time; }
    int context() { return _context; }

    void setId(int id);
    void setMatchId(int matchId);
    void setGameId(int gameId);
    void setDice1(int dice1);
    void setDice2(int dice2);
    void setIsWhite(bool isWhite);
    void setAction(int action);
    void setTime(QDateTime time);
    void setContext(int context);

private:
    int _id = 0;
    int _matchId = 0;
    int _gameId = 0;
    int _dice1 = 0;
    int _dice2 = 0;
    bool _isWhite = false;
    int _action = 0;
    QDateTime _time;
    int _context;
};

#endif // ACTION_H
