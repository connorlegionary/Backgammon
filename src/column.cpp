#include "column.h"

#include <QPainterPath>
#include <QPainter>

Column::Column(QObject *parent) : QQuickPaintedItem()
{
    _lightColor = QColor(92,125,180);
    _darkColor = QColor(52,86,172);
    _pen.setColor(_darkColor);
    _pen.setWidth(2);
    _pen.setBrush(QBrush(_darkColor));
}

void Column::paint(QPainter* painter)
{
    painter->setPen(_pen);
    QPainterPath path = QPainterPath();
    path.moveTo(width()*0.2, 0);
    path.lineTo(width()/2, height());
    path.lineTo(width()*0.8, 0);
    path.closeSubpath();
    painter->fillPath(path, QBrush(_even ? _darkColor : _lightColor));
}
