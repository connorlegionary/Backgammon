#ifndef ACTIONDATABASE_H
#define ACTIONDATABASE_H

#include <QObject>

#include "database.h"

#include "action.h"

class ActionDatabase : public Database
{
    Q_OBJECT
public:
    explicit ActionDatabase(QObject *parent = nullptr);

    void insertAction(int matchId, int gameId, int dice1, int dice2, bool isWhite, int action, int context, bool isComputer = false);
    std::list<Action> actions(int gameId);
    std::list<Action> actionsTo(int gameId, int maxId);

    bool lastTurnColor(int id);

signals:

};

#endif // ACTIONDATABASE_H
