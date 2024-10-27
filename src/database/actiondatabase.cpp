#include "actiondatabase.h"

#include <QDebug>

#include <QSqlQuery>
#include <QSqlError>

#include <QVariant>
#include <QDateTime>

ActionDatabase::ActionDatabase(QObject *parent)
    : Database("actions", parent)
{

}


void ActionDatabase::insertAction(int matchId, int gameId, int dice1, int dice2, bool isWhite, int action, int context, bool isComputer)
{
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("INSERT INTO actions (matchId, gameId, dice1, dice2, isWhite, action, time, context, computer) VALUES (?,?,?,?,?,?,?,?,?)");
    query.addBindValue(matchId);
    query.addBindValue(gameId);
    query.addBindValue(dice1);
    query.addBindValue(dice2);
    query.addBindValue(isWhite);
    query.addBindValue(action);
    query.addBindValue(QDateTime::currentDateTime());
    query.addBindValue(context);
    query.addBindValue(isComputer);
    if (!query.exec())
    {
        qDebug() << "add action error: " << query.lastError().text();
    }
}

std::list<Action> ActionDatabase::actions(int gameId)
{
    std::list<Action> actions = std::list<Action>();
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT id, matchId, gameId, dice1, dice2, isWhite, action, time, context FROM actions WHERE gameId = ?");
    query.addBindValue(gameId);
    if (query.exec())
    {
        while (query.next())
        {
            qDebug() << "got action";
            Action action = Action();
            action.setId(query.value(0).toInt());
            action.setMatchId(query.value(1).toInt());
            action.setGameId(query.value(2).toInt());
            action.setDice1(query.value(3).toInt());
            action.setDice2(query.value(4).toInt());
            action.setIsWhite(query.value(5).toInt());
            action.setAction(query.value(6).toInt());
            action.setTime(query.value(7).toDateTime());
            action.setContext(query.value(8).toInt());
            actions.push_back(action);
        }
    }
    qDebug() <<"actions: " << actions.size();
    return actions;
}

std::list<Action> ActionDatabase::actionsTo(int gameId, int maxId)
{
    bool deleted = false;
    std::list<Action> retVal;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("DELETE FROM actions WHERE gameId = ? AND id > ?");
    query.addBindValue(gameId);
    query.addBindValue(maxId);
    if (query.exec())
    {
        deleted = true;
    }
    if (deleted)
    {
        retVal = actions(gameId);
    }
    return retVal;
//    std::list<Action> actions = std::list<Action>();
//    QSqlQuery query = QSqlQuery(_db);
//    query.prepare("SELECT id, matchId, gameId, dice1, dice2, isWhite, action, time, context FROM actions WHERE gameId = ? AND id < ?");
//    query.addBindValue(gameId);
//    query.addBindValue(maxId);
//    if (query.exec())
//    {
//        while (query.next())
//        {
//            qDebug() << "got action";
//            Action action = Action();
//            action.setMatchId(query.value(1).toInt());
//            action.setGameId(query.value(2).toInt());
//            action.setDice1(query.value(3).toInt());
//            action.setDice2(query.value(4).toInt());
//            action.setIsWhite(query.value(5).toInt());
//            action.setAction(query.value(6).toInt());
//            action.setTime(query.value(7).toDateTime());
//            action.setContext(query.value(8).toInt());
//            actions.push_back(action);
//        }
//    }
//    qDebug() <<"actions: " << actions.size();
//    return actions;
}

bool ActionDatabase::lastTurnColor(int id)
{
    bool lastTurnColor = true;
    QSqlQuery query = QSqlQuery(_db);
    query.prepare("SELECT MAX(id), isWhite FROM actions WHERE gameId = ? AND action = 5");
    query.addBindValue(id);
    if (query.exec())
    {
        if (query.next())
        {
            lastTurnColor = query.value(1).toBool();
        }
    }
    return lastTurnColor;
}
