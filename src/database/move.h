#ifndef MOVE_H
#define MOVE_H

#include <QObject>
#include <QDateTime>

class PieceMove : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int move READ move NOTIFY moveChanged)
    Q_PROPERTY(int fromPosition READ fromPos NOTIFY fromPosChanged)

public:
    explicit PieceMove(int id, int gameId, int fromPos, int move, int action, int context, bool isWhite, QDateTime time, int matchId, QObject *parent = nullptr);
    PieceMove();

    int id() { return _id; }
    int gameId() { return _gameId; }
    int matchId() { return _matchId; }
    int fromPos() { return _fromPos; }
    int move() { return _move; }
    int action() { return _action; }
    int context() { return _context; }
    bool isWhite() { return _isWhite; }
    QDateTime time() { return _time; }

    void setId(int id);
    void setGameId(int gameId);
    void setMatchId(int gameId);
    void setFromPos(int fromPos);
    void setMove(int move);
    void setAction(int action);
    void setIsWhite(bool isWhite);
    void setTime(QDateTime time);

private:
    int _id;
    int _gameId;
    int _matchId;
    int _fromPos;
    int _move;
    int _context;
    int _action;
    bool _isWhite;
    QDateTime _time;

signals:
    void moveChanged();
    void fromPosChanged();

};

#endif // MOVE_H
