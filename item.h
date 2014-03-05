#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsPixmapItem>
#include <QList>

class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;

class Line;

class Item : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    enum ItemType { Basic, In3out1};                //typy itemů

    Item(ItemType filterType, QMenu *contextMenu,QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void removeLine(Line *line);
    void removeLines();
    ItemType filterType() const { return myfilterType; }
    QPolygonF polygon() const { return myPolygon; }
    void addLine(Line *line);

    int type() const { return Type;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    ItemType myfilterType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<Line *> lines;
};

#endif // ITEM_H
