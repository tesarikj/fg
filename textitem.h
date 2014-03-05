#ifndef TEXTITEM_H
#define TEXTITEM_H


#include <QGraphicsTextItem>
#include <QPen>

 class QFocusEvent;
 class QGraphicsItem;
 class QGraphicsScene;
 class QGraphicsSceneMouseEvent;

 class TextItem : public QGraphicsTextItem
 {
     Q_OBJECT

 public:
     enum { Type = UserType + 3 };

     TextItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

     int type() const { return Type; }

 signals:
     void lostFocus(TextItem *item);
     void selectedChange(QGraphicsItem *item);

 protected:
     QVariant itemChange(GraphicsItemChange change, const QVariant &value);
     void focusOutEvent(QFocusEvent *event);
     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
 };

#endif // TEXTITEM_H
