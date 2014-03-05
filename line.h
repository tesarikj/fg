#ifndef LINE_H
#define LINE_H

#include <QGraphicsLineItem>

#include "item.h"

class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;

class Line : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Line(Item *startItem, Item *endItem, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const { return Type; }
    QRectF boundingRect() const;

    Item *startItem() const { return myStartItem; }
    Item *endItem() const { return myEndItem; }

public slots:
   void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    Item *myStartItem;
    Item *myEndItem;
};

#endif

