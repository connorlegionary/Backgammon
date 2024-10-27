#ifndef GAME_H
#define GAME_H

#include <QObject>

class GameData
{
public:
    GameData(int id, QString name, int toScore, int score1, int score2, int doubleVal, int match);

    QString name() const { return _name; }
    int id() const { return _id; }
    int toScore() const { return _toScore; }
    int score1() const { return _score1; }
    int score2() const { return _score2; }
    int doubleVal() const { return _doubleVal; }
    int matchId() const { return _matchId; }

private:
    int _score1 = 0;
    int _score2 = 0;
    int _doubleVal = 1;
    int _id;
    int _turn;
    int _toScore;
    int _matchId;
    QString _name;
};

class Game : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int toScore READ toScore WRITE setToScore NOTIFY toScoreChanged)
    Q_PROPERTY(int score1 READ score1 WRITE setScore1 NOTIFY scoreChanged)
    Q_PROPERTY(int score2 READ score2 WRITE setScore2 NOTIFY scoreChanged)
    Q_PROPERTY(int doubleVal READ doubleVal WRITE setDouble NOTIFY doubleChanged)
    Q_PROPERTY(int matchId READ matchId WRITE setMatchId NOTIFY matchIdChanged)

public:
    Game(QObject* parent, int id, QString name, bool joined, int turn, int toScore);
    Game();

    int id() { return _id; }
    QString name() { return _name; }
    bool joined() { return _joined; }
    bool isWhite() { return _isWhite; }
    int turn() { return _turn; }
    int toScore() { return _toScore; }
    int score1() { return _score1; }
    int score2() { return _score2; }
    int doubleVal() { return _doubleVal; }
    int matchId() { return _matchId; }

    void setId(int id);
    void setName(QString name);
    void setJoined(bool joined);
    void setTurn(int turn);
    void setIsWhite(bool isWhite);
    void setToScore(int toScore);
    void setDouble(int doubleVal);
    void setMatchId(int matchId);

    void setScore1(int score);
    void setScore2(int score);

private:
    int _id;
    int _turn;
    int _toScore;
    int _matchId;
    QString _name;
    bool _joined;
    bool _isWhite;

    int _score1 = 0;
    int _score2 = 0;
    int _doubleVal = 1;

signals:
    void idChanged();
    void nameChanged();
    void joinedChanged();
    void turnChanged();
    void matchIdChanged();
    void isWhiteChanged();
    void scoreChanged();
    void doubleChanged();
    void toScoreChanged();
};

#endif // GAME_H
