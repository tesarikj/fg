#include <QtGui>

#include "item.h"
//#include "line.h"

Item::Item(ItemType Type, QMenu *contextMenu, QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsPolygonItem(parent, scene)
{
    myfilterType = Type;
    myContextMenu = contextMenu;

    switch (myfilterType) {
       case Basic:                                                  //dva vstupy
            myPolygon << QPointF(-25, -25) << QPointF(25, -25)
                      << QPointF(25, 25) << QPointF(-25, 25)
                      << QPointF(-25, -25);
            break;
        case In3out1:                                               //tři vstupy
            myPolygon << QPointF(-25, -40) << QPointF(25, -40)
                      << QPointF(25, 40) << QPointF(-25, 40)
                      << QPointF(-25, -40);
            break;
        default:
            myPolygon << QPointF(-25, -25) << QPointF(25, -25)
                      << QPointF(25, 25) << QPointF(-25, 25)
                      << QPointF(-25, -25);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);           //povolení pohybovaní s objekty
    setFlag(QGraphicsItem::ItemIsSelectable, true);      //povolení vybírání objektů
}

void Item::removeLine(Line *line)
{
    int index = lines.indexOf(line);

    if (index != -1)
        lines.removeAt(index);
}

void Item::removeLines()
{
    foreach (Line *line, lines) {
        line->startItem()->removeLine(line);
        line->endItem()->removeLine(line);
        scene()->removeItem(line);
        delete line;
    }
}

void Item::addLine(Line *line)
{
    lines.append(line);
}

void Item::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant Item::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Line *line, lines) {
            line->updatePosition();
        }
    }

    return value;
}

