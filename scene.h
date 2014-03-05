#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include "item.h"
#include "libstructure.h"
#include "textitem.h"

 class QGraphicsSceneMouseEvent;
 class QMenu;
 class QPointF;
 class QGraphicsLineItem;
 class QFont;
 class QGraphicsTextItem;
 class QColor;

 class Scene : public QGraphicsScene
 {
     Q_OBJECT

 public:
     enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

     Scene(QMenu *itemMenu, QObject *parent = 0);
     QFont font() const { return myFont; }
     void setFont(const QFont &font);

 public slots:
     void setMode(Mode mode);
     void setItemType(Item::ItemType type);
     void editorLostFocus(TextItem *item);

 signals:
     void itemInserted(LibStructure *item);
     void textInserted(QGraphicsTextItem *item);
     void itemSelected(QGraphicsItem *item);

 protected:
     void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

 private:
     bool isItemChange(int type);

     QPointF location;
     Item::ItemType myItemType;
     QMenu *myItemMenu;
     Mode myMode;
     bool leftButtonDown;
     QPointF startPoint;
     QGraphicsLineItem *line;
     QFont myFont;
     TextItem *textItem;
     LibStructure item;
 };

#endif // SCENE_H
