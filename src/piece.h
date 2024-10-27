#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QDebug>

class Piece : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isWhite READ isWhite NOTIFY positionChanged)
    Q_PROPERTY(bool isOff READ isOff NOTIFY positionChanged)
    Q_PROPERTY(int position READ position NOTIFY positionChanged)
    Q_PROPERTY(int rawPosition READ rawPosition NOTIFY positionChanged)

public:
    Piece(int id, int position, bool isWhite, QObject* parent = nullptr);
    Piece(int id, int position, int tempDiff, bool isWhite, QObject* parent = nullptr);

    int id() { return _id; }
    int position() { return (isOff() ? 0 : _position) + _tempDiff; }
    int tempDiff() { return _tempDiff; }
    Q_INVOKABLE int rawPosition() { return _position; }
    void setPosition(int position) { if (position != 0) {
            _position = position; _tempDiff = 0; _tempMoves.clear(); emit positionChanged();
        }
    }

    bool containsTempMove(int move);
    void undo() { _tempDiff = 0; _tempMoves.clear(); emit positionChanged();}
    void tempMove(int move) { _tempDiff += move;  _tempMoves.append(move); emit positionChanged();  } // qDebug() << "moving from: " << _position << " to: " << move;
    void confirmMove() {  setPosition(position()); }
    QList<int> tempMoves() { return _tempMoves; }
    int lastPosition() { if (_tempMoves.size() > 0) { return position() - _tempMoves.last(); } else {return position();} }
    bool isOff() { return _position == -1 && _tempDiff == 0; }
    bool inHome() { return _isWhite ? (position() > 18 && position() < 25) : (position() > 0 && position() < 7); }
    bool inRack() { return (position() < -1 || position() > 24 || position() == 0); }
    bool isWhite() { return _isWhite; }
    bool exactMoveToRack(int move) {
        if (isWhite())
        {
            return 25 - (position() - move) == 0;
        } else {
            return position() + move == 0;
        }
    }

    void undoLastMove() {
        qDebug() << "undoing last move: " << _tempMoves.last() << " from position: " << position();
        _tempDiff -= _tempMoves.last();
        _tempMoves.removeLast();
        qDebug() << "new diff: " << _tempDiff << " now at position: " << position();
    }
private:
    int _id;
    int _position;
    int _tempDiff = 0;
    bool _isWhite;

    QList<int> _tempMoves; // add a way of telling if piece was taken during move so can undo both moves
signals:
    void positionChanged();
};

#endif // PIECE_H
