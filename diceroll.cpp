#include "diceroll.h"

DiceRoll::DiceRoll(int dice1, int dice2)
{
    _dice1 = dice1;
    _dice2 = dice2;
}

void DiceRoll::reverseDice()
{
    int temp = _dice2;
    _dice2 = _dice1;
    _dice1 = temp;
}
