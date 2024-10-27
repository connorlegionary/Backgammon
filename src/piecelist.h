#ifndef PIECELIST_H
#define PIECELIST_H

#include <QObject>
#include <QVariant>

class PieceList : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QVariant> pieces READ pieces NOTIFY piecesChanged)

public:
    PieceList(QList<QVariant> pieces);

    QList<QVariant> pieces() { return _pieces;}
private:
    QList<QVariant> _pieces;

signals:
    void piecesChanged();
};

#endif // PIECELIST_H
