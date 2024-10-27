#ifndef COMPUTERGAME_H
#define COMPUTERGAME_H

#include <QObject>
#include <QDateTime>

class ComputerGame : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int computer READ computer NOTIFY computerChanged)
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(int toScore READ toScore NOTIFY toScoreChanged)
    Q_PROPERTY(int timeLimit READ timeLimit NOTIFY timeLimitChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY dateChanged)

public:
    explicit ComputerGame(QObject *parent, int computer, int id, int toScore, int timeLimit, QDateTime date);

    int computer() const { return _computer; }
    int id() const { return _id; }
    int toScore() const { return _toScore; }
    int timeLimit() const { return _timeLimit; }
    QDateTime date() const { return _date; }

private:
    int _timeLimit;
    int _toScore;
    int _id;
    int _computer;
    QDateTime _date;

signals:
    void computerChanged();
    void idChanged();
    void toScoreChanged();
    void timeLimitChanged();
    void dateChanged();
};

#endif // COMPUTERGAME_H
