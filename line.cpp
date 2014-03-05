#include <QtGui>

#include "line.h"
#include <math.h>


Line::Line(Item *startItem, Item *endItem,QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF Line::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Line::updatePosition()
{
   QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
   setLine(line);
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))      //pokud se překrývají objekty, čára se nevykreslí
        return;

    QPen myPen = pen();
    myPen.setColor(Qt::black);
    painter->setPen(myPen);
    painter->setBrush(Qt::black);


    QLineF centerLine(myStartItem->pos(), myEndItem->pos());       //čára se souřadnicemi x, y
    QPolygonF endPolygon = myEndItem->polygon();
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;

    for (int i = 1; i < endPolygon.count(); ++i) {
       p2 = endPolygon.at(i) + myEndItem->pos();
       polyLine = QLineF(p1, p2);
       QLineF::IntersectType intersectType =polyLine.intersect(centerLine, &intersectPoint);
       if (intersectType == QLineF::BoundedIntersection)
              break;
              p1 = p2;
    }

    setLine(QLineF(intersectPoint, myStartItem->pos()));             //připojení čary k itemům

    painter->drawLine(line());

    if (isSelected()) {                                    //pokud je vybrána čára
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));   //okolo se zobrazí čárková čára
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}

