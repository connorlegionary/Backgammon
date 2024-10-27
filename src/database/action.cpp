#include "action.h"

Action::Action()
{

}

Action::Action(int id, int matchId, int gameId, int dice1, int dice2, int isWhite, int action, QDateTime time, int context)
{
    _id = id;
    _matchId = matchId;
    _gameId = gameId;
    _dice1 = dice1;
    _dice2 = dice2;
    _isWhite = isWhite;
    _action = action;
    _time = time;
    _context = context;
}

void Action::setId(int id)
{
    _id = id;
}

void Action::setMatchId(int matchId)
{
    _matchId = matchId;
}

void Action::setGameId(int gameId)
{
    _gameId = gameId;
}
void Action::setDice1(int dice1)
{
    _dice1 = dice1;
}

void Action::setDice2(int dice2)
{
    _dice2 = dice2;
}

void Action::setIsWhite(bool isWhite)
{
    _isWhite = isWhite;
}

void Action::setAction(int action)
{
    _action = action;
}

void Action::setTime(QDateTime time)
{
    _time = time;
}

void Action::setContext(int context)
{
    _context = context;
}
