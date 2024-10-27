#ifndef DICEROLL_H
#define DICEROLL_H

#include <QObject>

class DiceRoll : public QObject
{
    Q_OBJECT

public:
    DiceRoll(int dice1, int dice2);

    int dice1() { return _dice1; }
    int dice2() { return _dice2; }
    void reverseDice();

private:
    int _dice1;
    int _dice2;
};


#endif // DICEROLL_H
