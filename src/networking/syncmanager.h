#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

//#include "animation.h"
#include "../database/logindatabase.h"
#include "../database/gamesdatabase.h"
#include "../database/movesdatabase.h"
#include "../database/settingsdatabase.h"
#include "../database/puzzledatabase.h"

class GameModel;

class SyncManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString username READ username NOTIFY usernameChanged)
    Q_PROPERTY(bool turnIsWhite READ isWhite NOTIFY isWhiteChanged)
    Q_PROPERTY(bool needRoll READ needRoll NOTIFY needRollChanged)
    Q_PROPERTY(QVariant gamesDatabase READ gamesDatabase WRITE setGamesDatabase NOTIFY gamesDatabaseChanged)
    Q_PROPERTY(QVariant movesDatabase READ movesDatabase WRITE setMovesDatabase NOTIFY movesDatabaseChanged)
    Q_PROPERTY(QVariant settingsDatabase READ settingsDatabase WRITE setSettingsDatabase NOTIFY settingsDatabaseChanged)
    Q_PROPERTY(QVariant puzzlesDatabase READ puzzleDatabase WRITE setPuzzleDatabase NOTIFY puzzleDatabaseChanged)

public:
    explicit SyncManager(QObject *parent = nullptr);

    Q_INVOKABLE void createUser(QString user, QString password);
    Q_INVOKABLE void createGame(QString user, int score, int seconds);
    Q_INVOKABLE void checkLogin();
    Q_INVOKABLE void login(QString user, QString password);
    Q_INVOKABLE void downloadGames(int lastId);
    Q_INVOKABLE void downloadMoves(int gameId);
    Q_INVOKABLE void rollDice(int gameId);
    Q_INVOKABLE void turnFinished(int gameId);
    Q_INVOKABLE void undoMove(int gameId);
    Q_INVOKABLE void addMove(int id, int gameId, int fromPos, int toPos, int action, bool isWhite, bool took);
    Q_INVOKABLE void sendWonGame(int gameId, int score);
    Q_INVOKABLE void sendDouble(int gameId);
    Q_INVOKABLE void resignMatch(int gameId);
    Q_INVOKABLE void resignGame(int gameId);
    Q_INVOKABLE void doubleResponse(int gameId, bool accepted);
    Q_INVOKABLE void joinMatchmaking(int type);
    Q_INVOKABLE void leaveMatchmaking();
    Q_INVOKABLE void setGameModel(QVariant gameModel);
    Q_INVOKABLE void downloadPuzzles();

    Q_INVOKABLE void logout();

    bool isWhite() { return _turnIsWhite; };
    int downloadMoves(QJsonArray moves);
    Q_INVOKABLE bool checkTurn(int isWhite, bool isFirstTurn, int gameId);
    void downloadActions(QJsonArray actions, int gameId, bool hadMoves);
    void downloadGames(QJsonArray games);
    void downloadPuzzles(QJsonArray puzzles);
    void addAuth(QJsonObject& obj);

    QString username() { return _loginDatabase->username(); }

    QVariant gamesDatabase() { return QVariant::fromValue(_gamesDatabase); }
    void setGamesDatabase(QVariant gamesDb);

    Q_INVOKABLE void setFinished(int gameId);

    QVariant movesDatabase() { return QVariant::fromValue(_moveDatabase); }
    void setMovesDatabase(QVariant movesDb);

    QVariant settingsDatabase() { return QVariant::fromValue(_settingsDatabase); }
    void setSettingsDatabase(QVariant settingsDb);

    QVariant puzzleDatabase() { return QVariant::fromValue(_puzzleDatabase); }
    void setPuzzleDatabase(QVariant puzzleDb);

    void setNeedRoll(bool needRoll);
    bool needRoll() { return _needRoll; }

private:
    QNetworkAccessManager *_manager = nullptr;
    QNetworkRequest _request;
    LoginDatabase* _loginDatabase = nullptr;
    GamesDatabase* _gamesDatabase = nullptr;
    MovesDatabase* _moveDatabase = nullptr;
    PuzzleDatabase* _puzzleDatabase = nullptr;
    SettingsDatabase* _settingsDatabase = nullptr;
    GameModel* _gameModel = nullptr;
    QString _urlString;
    bool _skipGet = false;
    bool _turnIsWhite = true;
//    int _rolled = 0;
    bool _undid = false;
    bool _needRoll;
    \
    bool _awaitingResponse = false;


signals:
    void animationAdded(int id);
    void gamesDatabaseChanged();
    void movesDatabaseChanged();
    void settingsDatabaseChanged();
    void requestSuccess(int messageType);
    void requestFailed(int messageType);
    void usernameChanged();
    void movesChanged(int firstId);
    void newMove(int fromPos, int move, bool isMyTurn); // add temporary move instead of saving immediately so we can undo easier.
    void diceChanged(int turn, int dice1, int dice2, bool currentAction);
    void startRoll(bool type);
    void needRoll(bool reroll);
    void showDice(int dice1, int dice2, bool mixed); // dice 1 is always whites dice
    void isWhiteChanged();
    void wonGame(bool isWhite, bool current);
    void opponentDoubled();
    void wonMatch(bool isWhite);
    void flashUndo();
    void confirmMoves();
    void clearTemp();
    void needRollChanged();
    void currentMatch(int matchId);
    void currentStats(int whiteScore, int blackScore, int whiteTime, int blackTime, int doubleVal, int whoseDouble);
    void puzzlesChanged(int foundPuzzle);
    void puzzleDatabaseChanged();
};

#endif // SYNCMANAGER_H
