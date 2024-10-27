#include "analysisthread.h"

#include "positionmodel.h"

AnalysisThread::AnalysisThread(QObject *parent)
    : QObject{parent}
{
    startAnalysisEngine();
}

void AnalysisThread::startAnalysisEngine()
{
    _thread = new QThread();
    _handler = new Analysis();
    _handler->moveToThread(_thread);
    QObject::connect(_handler, SIGNAL(finished()), _thread, SLOT(quit()));
    QObject::connect(_handler, SIGNAL(finished()), _handler, SLOT(deleteLater()));
    QObject::connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));
    QObject::connect(this, &AnalysisThread::getBestMoves, _handler, &Analysis::getBestMoves);
    QObject::connect(this, &AnalysisThread::analiseMove, _handler, &Analysis::analiseMoves);
    QObject::connect(_handler, &Analysis::positionAnalised, this, &AnalysisThread::receviedPositionAnalysis);

    _thread->start();
}

QVariant AnalysisThread::model() { return QVariant::fromValue(_model); }

void AnalysisThread::setModel(QVariant model)
{
    _model = model.value<PositionModel*>();
}

void AnalysisThread::triggerGetMoves()
{
    _model->clearPositions();
}

void AnalysisThread::receviedPositionAnalysis(QList<QVariant> moves, int score, int whitePointsLeft, int blackPointsLeft)
{
    _model->addPosition(Position({}, moves, score, whitePointsLeft, blackPointsLeft));
}
