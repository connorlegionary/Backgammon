#include "game.h"

#include <QDebug>

GameData::GameData(int id, QString name, int toScore, int score1, int score2, int doubleVal, int match)
{
    _id = id;
    _name = name;
    _toScore = toScore;
    _score1 = score1;
    _score2 = score2;
    _doubleVal = doubleVal;
    _matchId = match;
}

Game::Game(QObject* parent, int id, QString name, bool joined, int turn, int toScore) : QObject(parent)
{
    _id = id;
    _name = name;
    _joined = joined;
    _turn = turn;
    _toScore = toScore;
}

Game::Game()
{

}

void Game::setId(int id)
{
    _id = id;
    emit idChanged();
}

void Game::setName(QString name)
{
    _name = name;
    emit nameChanged();
}

void Game::setJoined(bool joined)
{
    _joined = joined;
    emit joinedChanged();
}

void Game::setTurn(int turn)
{
    _turn = turn;
    emit turnChanged();
}

void Game::setIsWhite(bool isWhite)
{
    _isWhite = isWhite;
    emit isWhiteChanged();
}

void Game::setToScore(int toScore)
{
    _toScore = toScore;
    emit toScoreChanged();
}

void Game::setDouble(int doubleVal)
{
    _doubleVal = doubleVal;
    emit doubleChanged();
}

void Game::setMatchId(int matchId)
{
    _matchId = matchId;
    emit matchIdChanged();
}

void Game::setScore1(int score)
{
    _score1 = score;
    emit scoreChanged();
}

void Game::setScore2(int score)
{
    _score2 = score;
    emit scoreChanged();
}
