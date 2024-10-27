#ifndef PLAYBACKMODEL_H
#define PLAYBACKMODEL_H

#include <QObject>
#include <QVariant>

#include "src/piecemodel.h"

class PlaybackModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QVariant> moves READ moves WRITE setMoves NOTIFY movesChanged)
    Q_PROPERTY(QVariant pieceModel READ pieceModel WRITE setPieceModel NOTIFY pieceModelChanged)
    Q_PROPERTY(int currentMove READ currentMove NOTIFY currentMoveChanged)
public:
    explicit PlaybackModel(QObject *parent = nullptr);

    QVariant pieceModel() { return QVariant::fromValue(_pieceModel); }
    void setPieceModel(QVariant model);

    QList<QVariant> moves() { return _moves; }
    void setMoves(QList<QVariant> moves);

    Q_INVOKABLE void previousMove();
    Q_INVOKABLE void nextMove();

    void resetPosition();

    int currentMove() { return _currentMove;}
private:
    QList<QVariant> _moves;
    PieceModel* _pieceModel;
    int _currentMove = 0;

signals:
    void currentMoveChanged();
    void movesChanged();
    void pieceModelChanged();

};

#endif // PLAYBACKMODEL_H
