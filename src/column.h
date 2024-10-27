#ifndef COLUMN_H
#define COLUMN_H

#include <QQuickPaintedItem>
#include <QPen>
#include <QColor>

class Column : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(bool even READ even WRITE setEven NOTIFY evenChanged)
public:
    explicit Column(QObject *parent = nullptr);

    void paint(QPainter* painter) override;

    bool even() { return _even; }
    void setEven(bool even) { _even = even; emit evenChanged(); }

private:
    QColor _lightColor;
    QColor _darkColor;
    QPen _pen;
    bool _even = true;

signals:
    void evenChanged();
};

#endif // COLUMN_H
