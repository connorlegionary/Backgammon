#include "computergame.h"

ComputerGame::ComputerGame(QObject *parent, int computer, int id, int toScore, int timeLimit, QDateTime date)
    : QObject{parent}
{
    _computer = computer;
    _id = id;
    _toScore = toScore;
    _timeLimit = timeLimit;
    _date = date;
}
