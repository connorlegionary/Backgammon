#include "syncmanager.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkReply>
#include <QObject>
#include <QTimer>

#include "src/gamemodel.h"
#include "../database/move.h"
#include <QDebug>

#include "src/database/action.h"

enum MessageType {
    GetGamesResponse = 0,
    AddGameResponse,
    LoginResponse,
    CreateUserResponse,
    DeleteGameResponse,
    JoinGameResponse,
    AddMoveResponse,
    GetMovesResponse,
    GetPuzzlesResponse,
};

SyncManager::SyncManager(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_IOS
    //    _urlString = "http://192.168.2.227:5001/";
    _urlString = "http://192.168.2.154:5001/";
#elif defined(Q_OS_ANDROID)
    _urlString = "http://192.168.0.16:5001/";
#else
    _urlString = "http://127.0.0.1:5001/";
#endif
    _loginDatabase = new LoginDatabase();
    _manager = new QNetworkAccessManager();
    connect(_manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *reply) {
        if (reply->error()) {
            qDebug() << "Server Error: " << reply->errorString();
            emit requestFailed(-1);
            return;
        }

        _skipGet = true;
        QString answer = reply->readAll();
//        qDebug() << "answer: " << answer;
        QJsonDocument document = QJsonDocument::fromJson(answer.toStdString().c_str());
        QJsonObject data = document.object();
        QJsonValue messageType = data.value("messageType");
        switch ((MessageType)messageType.toInt())
        {
        case GetGamesResponse:
            if (data.value("success").toBool())
            {
                downloadGames(data.value("games").toArray());
            }
            break;
        case GetMovesResponse:
        {
//            if (!_awaitingResponse)
//            {
//                _gamesDatabase->setCurrentMatchId();
//            qDebug() << "Answer: " << answer;
                emit currentMatch(data.value("matchId").toInt());
                downloadMoves(data.value("moves").toArray());
                emit currentStats(data.value("whiteScore").toInt(), data.value("blackScore").toInt(), data.value("whiteTime").toInt(), data.value("blackTime").toInt(), data.value("doubleVal").toInt(), data.value("whoseDouble").toInt());
                downloadActions(data.value("actions").toArray(), data.value("game_id").toInt(), data.value("moves").toArray().size() > 0);
//            }

            break;
        }
        case AddGameResponse:
            if (data.value("success").toBool())
            {
                downloadGames(_gamesDatabase->lastId());
            }
            break;
        case LoginResponse:
            if (data.value("success").toBool())
            {
                _loginDatabase->setAuthToken(data.value("auth_token").toString());
                _loginDatabase->setId(data.value("user").toInt());
            }
            break;
        case CreateUserResponse:
            break;
        case DeleteGameResponse: {
            if (data.value("success").toBool())
            {
            }
            break;
        }
        case JoinGameResponse: {
            if (data.value("success").toBool())
            {
            }
            break;
        }
        case AddMoveResponse: {
            downloadMoves(data.value("game_id").toInt());
            break;
        }
        case GetPuzzlesResponse: {
            downloadPuzzles(data.value("puzzles").toArray());
            break;
        }
        }

        if (data.value("success").toBool())
        {
            emit requestSuccess(messageType.toInt());
        } else {
            emit requestFailed(messageType.toInt());
        }
    }
    );
}

void SyncManager::createUser(QString user, QString password)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "user/create"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    body.insert("username", user.toStdString().c_str());
    body.insert("password", password.toStdString().c_str());
    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::createGame(QString user, int score, int seconds)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "game/create"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    body.insert("opponent", user);
    body.insert("toScore", score);
    body.insert("seconds", seconds);
    addAuth(body);
    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::checkLogin()
{
    if (!_loginDatabase->authToken().isEmpty())
    {
        _request.setUrl(QUrl(_settingsDatabase->ip() + "user/checklogin"));
        _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject body = QJsonObject();
        addAuth(body);
        QJsonDocument doc(body);
        _awaitingResponse = true;
        _manager->post(_request, doc.toJson());
    }
}

void SyncManager::login(QString user, QString password)
{
    if (_loginDatabase->username() != user)
        _loginDatabase->clearData();
    qDebug() << "ip: " << _settingsDatabase->ip();
    _request.setUrl(QUrl(_settingsDatabase->ip() + "user/login"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
    _loginDatabase->setUsername(user);

    QJsonObject body = QJsonObject();
    body.insert("username", user.toStdString().c_str());
    body.insert("password", password.toStdString().c_str());
    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::downloadGames(int lastId)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "game/get"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    body.insert("lastId", lastId);
    addAuth(body);
    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::downloadPuzzles()
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "puzzle/get"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    body.insert("lastId", _puzzleDatabase->lastId());
    addAuth(body);
    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::downloadMoves(int gameId)
{
    if (_skipGet)
    {
        _skipGet = false;
        return;
    }
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/get"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

//    qDebug() << "Download moves after: " << _gamesDatabase->getLastActionId(gameId);
    QJsonObject body = QJsonObject();
    body.insert("game_id", gameId);
    body.insert("user_id", _loginDatabase->id());
    body.insert("lastId", _gamesDatabase->getLastMoveId(gameId));
    body.insert("lastActionId", _gamesDatabase->getLastActionId(gameId));
    body.insert("isWhite", isWhite());
    addAuth(body);
    QJsonDocument doc(body);
//    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::rollDice(int gameId)
{
//    _rolled = true;
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/roll"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    body.insert("game_id", gameId);
    body.insert("user_id", _loginDatabase->id());
    body.insert("lastId", _gamesDatabase->getLastMoveId(gameId));
    addAuth(body);
    QJsonDocument doc(body);
    _needRoll = false;
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::turnFinished(int gameId)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/finish"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    body.insert("game_id", gameId);
    body.insert("user_id", _loginDatabase->id());
    body.insert("isWhite", !_gamesDatabase->isWhite(gameId));
    body.insert("lastId", _gamesDatabase->getLastMoveId(gameId));

    _undid = false;
    emit confirmMoves();
    addAuth(body);
    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::downloadGames(QJsonArray games)
{
    for (int i = 0; i < games.size(); i++)
    {
        Game* game = new Game();
        QJsonObject gameVar = games.at(i).toObject();
        game->setId(gameVar.value("id").toInt());
        game->setName(gameVar.value("name").toString());
        game->setJoined(gameVar.value("joined").toBool());
        game->setTurn(gameVar.value("turn").toInt());
        game->setIsWhite(gameVar.value("isWhite").toBool());
        game->setToScore(gameVar.value("toScore").toInt());
        _gamesDatabase->addGame(game);
        _gameModel->addGame(GameData(gameVar.value("id").toInt(),gameVar.value("name").toString(),gameVar.value("toScore").toInt(),0,0,0,0));
    }
}

void SyncManager::undoMove(int gameId)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/undo"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("game_id", gameId);
    body.insert("user_id", _loginDatabase->id());
    body.insert("isWhite", _gamesDatabase->isWhite(gameId));
    body.insert("lastId", _gamesDatabase->getLastMoveId(gameId));
    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::addMove(int id, int gameId, int fromPos, int move, int action, bool isWhite, bool took)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/add"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("id", id);
    body.insert("gameId", gameId);
    body.insert("fromPos", fromPos);
    body.insert("move", move);
    body.insert("pieceAction", action);
    body.insert("isWhite", isWhite);
    body.insert("took", took);
    body.insert("user_id", _loginDatabase->id());
    body.insert("time", QDateTime::currentDateTime().toString());

    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::sendWonGame(int gameId, int score)
{
        _request.setUrl(QUrl(_settingsDatabase->ip() + "move/checkWin"));
        _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

        QJsonObject body = QJsonObject();
        addAuth(body);
        body.insert("game_id", gameId);
        body.insert("isWhite", isWhite());
//        body.insert("score", score);

        QJsonDocument doc(body);
        _manager->post(_request, doc.toJson());

}

void SyncManager::sendDouble(int gameId)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/double"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("game_id", gameId);
    body.insert("isWhite", isWhite());
    body.insert("time", QDateTime::currentDateTime().toString());

    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::resignMatch(int gameId)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/resignMatch"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("game_id", gameId);
    body.insert("isWhite", isWhite());
    body.insert("time", QDateTime::currentDateTime().toString());

    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::resignGame(int gameId)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/resignGame"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("game_id", gameId);
    body.insert("isWhite", isWhite());
    body.insert("time", QDateTime::currentDateTime().toString());

    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::doubleResponse(int gameId, bool accepted)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "move/doubleResponse"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("game_id", gameId);
    body.insert("isWhite", isWhite());
    body.insert("time", QDateTime::currentDateTime().toString());
    body.insert("accepted", accepted);

    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());
}

void SyncManager::joinMatchmaking(int type)
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "matchmaking/joinqueue"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("gameType", type);
    body.insert("user_id", _loginDatabase->id());

    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());

}

void SyncManager::leaveMatchmaking()
{
    _request.setUrl(QUrl(_settingsDatabase->ip() + "matchmaking/leaveQueue"));
    _request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

    QJsonObject body = QJsonObject();
    addAuth(body);
    body.insert("user_id", _loginDatabase->id());

    QJsonDocument doc(body);
    _awaitingResponse = true;
    _manager->post(_request, doc.toJson());

}

void SyncManager::setGameModel(QVariant gameModel)
{
    _gameModel = gameModel.value<GameModel*>();

    if (_gamesDatabase)
        _gameModel->setGames(_gamesDatabase->gameData());
}

void SyncManager::logout()
{
    _loginDatabase->clearData();
}

int SyncManager::downloadMoves(QJsonArray moves)
{
    int gameId = -1;
    int foundMove = -1;
    int foundMoveOnMatch = -1;

    int lastMove = _moveDatabase->lastMove();
    for (int i = 0 ; i < moves.size(); i++)
    {
        PieceMove* move = new PieceMove();
        QJsonObject moveVar = moves.at(i).toObject();
        if (moveVar.value("id").toInt() > lastMove)
        {
            gameId = moveVar.value("game_id").toInt();
            move->setId(moveVar.value("id").toInt());
            move->setGameId(moveVar.value("game_id").toInt());
            move->setMatchId(moveVar.value("matchId").toInt());
            move->setFromPos(moveVar.value("fromPos").toInt());
            move->setMove(moveVar.value("move").toInt());
            move->setAction(moveVar.value("action").toInt());
            move->setIsWhite(moveVar.value("isWhite").toBool());
            move->setTime(QDateTime::fromString(moveVar.value("time").toString()));
            qDebug() << "adding move to database: " << move->id();
            _moveDatabase->addMove(move->id(), move->fromPos(), move->move(), move->action(), move->isWhite(), move->matchId());
            if (foundMove == -1 || foundMoveOnMatch < move->matchId())
            {
                foundMove = move->id();
                foundMoveOnMatch = move->matchId();
            }
        }
    }
    if (foundMove != -1)
    {
        emit movesChanged(foundMove);
    }
    return gameId;
}

void SyncManager::downloadPuzzles(QJsonArray puzzles)
{
    int gameId = -1;
    int foundPuzzle = -1;

//    int lastMove = _puzzleDatabase->lastMove();
    for (int i = 0 ; i < puzzles.size(); i++)
    {
        // process puzzle data
        QJsonObject puzzleVar = puzzles.at(i).toObject();
        PuzzleData data = PuzzleData(puzzleVar.value("name").toString(), puzzleVar.value("id").toInt(), puzzleVar.value("difficulty").toInt(), puzzleVar.value("isWhite").toInt());

        QJsonArray moveArray = puzzleVar.value("moves").toArray();
        QJsonArray locationArray = puzzleVar.value("startPositions").toArray();

        for (int i = 0; i < moveArray.size(); i++)
        {
            QJsonObject move = moveArray.at(i).toObject();
            data.addPuzzleMove(new PuzzleMove(move.value("fromPos").toInt(), move.value("move").toInt(), move.value("roll").toInt(),move.value("isWhite").toBool(),move.value("step").toInt()));
        }

        for (int i = 0; i < locationArray.size(); i++)
        {
            QJsonObject location = locationArray.at(i).toObject();
            qDebug() << "parsing location: " << location.value("position").toInt();
            data.addPuzzleLocation(new PuzzleLocation(location.value("position").toInt(), location.value("isWhite").toBool()));
        }

        _puzzleDatabase->insertPuzzle(data);
    }

    if (foundPuzzle != -1)
    {
        emit puzzlesChanged(foundPuzzle);
    }
}

bool SyncManager::checkTurn(int isWhiteTurn, bool isFirstTurn, int gameId)
{
    if (isWhiteTurn == 2)
    {
        return true;
    } else if (isWhiteTurn == (isFirstTurn ? _gamesDatabase->isWhite(gameId) ? 0 : 1 : _gamesDatabase->isWhite(gameId) ? 1 : 0))
    {
        return true;
    }
    return false;
}

void SyncManager::downloadActions(QJsonArray actions, int gameId, bool hadMoves)
{
    for (int i = 0 ; i < actions.size(); i++)
    {
        bool currentAction = i == actions.size() - 1;
        QJsonObject data = actions.at(i).toObject();
        switch ((ActionTypes)data.value("action").toInt())
        {
        case None:
            break;
        case StartRoll:
            if (currentAction)
            {
                if (checkTurn(data.value("isWhite").toInt(), true, gameId))
                    emit startRoll(false);
                else
                    emit diceChanged(data.value("isWhite").toInt() == 1 ? true : false, data.value("dice1").toInt(),data.value("dice2").toInt(), currentAction);
            }
            break;
        case FirstTurnReroll:
            if (currentAction)
            {
                emit startRoll(true);
            }
            break;
        case FirstTurn:
            _turnIsWhite = data.value("isWhite").toInt() == 1 ? true : false;
            emit diceChanged(data.value("isWhite").toInt() == 1 ? true : false, data.value("dice1").toInt(),data.value("dice2").toInt(), currentAction);
            emit isWhiteChanged();
        case Roll:
            _turnIsWhite = data.value("isWhite").toInt() == 1 ? true : false;
            emit diceChanged(data.value("isWhite").toInt() == 1 ? true : false, data.value("dice1").toInt(),data.value("dice2").toInt(), currentAction);
            break;
        case TurnFinish:
            if (currentAction)
            {
                emit clearTemp();
                if (checkTurn(data.value("isWhite").toInt() == 1 ? true : false, false, gameId))
                {
                    emit needRoll(false);
                    _needRoll = true;
                }
            }
            break;
        case Undo:
            if (currentAction)
            {
                if (!_undid)
                {
                    emit flashUndo();
                }
            }
            break;
        case Move:
//            if (!hadMoves)
//            {
                qDebug() << "found move";
                if (!checkTurn(hadMoves ? 3 : data.value("isWhite").toInt() == 1 ? true : false, false, gameId)) // only show if other player moved, we already moved our piece (Cannot wait for sync as it delays moves)
                {
                    qDebug() << "Loading moves: ";
                    emit newMove(data.value("dice1").toInt(), data.value("dice2").toInt() - data.value("dice1").toInt(), checkTurn(data.value("isWhite").toInt() == 1 ? true : false, false, gameId));
                }
//            }
            break;
        case Won:
            _needRoll = true;

            if (currentAction)
            {
                emit wonGame(data.value("isWhite").toInt() == 1 ? true : false, currentAction);
                QTimer::singleShot(2000,[this] {
                   emit startRoll(false);
                });
            }
            break;
        case Double:
            // handle indicator and number
            if (currentAction)
            {
                if (checkTurn(data.value("isWhite").toInt() == 1 ? true : false, false, gameId))
                {
                    emit opponentDoubled();
                }
            }
            break;
        case Finished:
            qDebug() << "game finished";
            emit wonMatch(data.value("isWhite").toInt() == 1 ? true : false);
            break;
        }
        _gamesDatabase->updateLastActionId(gameId, data.value("turn").toInt());
    }
}

void SyncManager::setGamesDatabase(QVariant gamesDb)
{
    _gamesDatabase = gamesDb.value<GamesDatabase*>();

    if (_gameModel)
        _gameModel->setGames(_gamesDatabase->gameData());
}

void SyncManager::setFinished(int gameId)
{
    _gamesDatabase->setFinished(gameId);

}

void SyncManager::setMovesDatabase(QVariant movesDb)
{
    _moveDatabase = movesDb.value<MovesDatabase*>();
    qDebug() << "Set move database: " << movesDb;
}

void SyncManager::setPuzzleDatabase(QVariant puzzleDb)
{
    _puzzleDatabase = puzzleDb.value<PuzzleDatabase*>();
    qDebug() << "Set move database: " << puzzleDb;
}

void SyncManager::setSettingsDatabase(QVariant settingsDb)
{
    _settingsDatabase = settingsDb.value<SettingsDatabase*>();
    qDebug() << "Set move database: " << settingsDb;
}

void SyncManager::setNeedRoll(bool needRoll)
{
    _needRoll = needRoll;
    emit needRollChanged();
}

void SyncManager::addAuth(QJsonObject& obj)
{
    obj.insert("user", _loginDatabase->username());
    obj.insert("token", _loginDatabase->authToken());
}
