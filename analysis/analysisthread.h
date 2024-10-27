#ifndef ANALYSISTHREAD_H
#define ANALYSISTHREAD_H

#include <QObject>
#include <QThread>

#include "analysis.h"

class PositionModel;

class AnalysisThread : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)

public:
    explicit AnalysisThread(QObject *parent = nullptr);

    void startAnalysisEngine();

    Q_INVOKABLE void triggerGetMoves();

    QVariant model();
    void setModel(QVariant model);

private:
    QThread* _thread = nullptr;
    Analysis* _handler = nullptr;

    PositionModel* _model;

signals:
    void getBestMoves(QList<QVariant> positionVar, int color, QVariant diceRoll);
    void modelChanged();
    void analiseMove(QList<QVariant> positionVar, int color);

public slots:
    void receviedPositionAnalysis(QList<QVariant> moves, int score, int whitePointsLeft, int blackPointsLeft);
};

#endif // ANALYSISTHREAD_H
