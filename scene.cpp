#include <QtGui>

#include "scene.h"
#include "mainwindow.h"
#include "line.h"


Scene::Scene(QMenu *itemMenu, QObject *parent): QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    //myItemType = Item::Basic;
    line = 0;
    textItem = 0;
}

void Scene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(TextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<TextItem *>(selectedItems().first());
        if (item)
            item->setFont(myFont);
    }
}

void Scene::setMode(Mode mode)
{
    myMode = mode;
}

/*void Scene::setItemType(Item::ItemType type)
{
    myItemType = type;
}*/
void Scene::editorLostFocus(TextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    LibStructure *item;
    switch (myMode) {
        case InsertItem:
            location= mouseEvent->scenePos();               //zjistí se aktuální pozice myši
            location.setX(qRound(location.x()/10)*10);
            location.setY(qRound(location.y()/10)*10);
            item = new LibStructure(0,0,myItemMenu, location);
            addItem(item);
            item->setPos(location);           //umístí item na pozici myši
            emit itemInserted(item);
            break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            line->setPen(QPen(Qt::black, 2));
            addItem(line);
            break;
        case InsertText:
            textItem = new TextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(TextItem*)),this, SLOT(editorLostFocus(TextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
            default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    /*if (line != 0 && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 && startItems.first()->type() == Item::Type &&
            endItems.first()->type() == Item::Type && startItems.first() != endItems.first()) {
            Item *startItem = qgraphicsitem_cast<Item *>(startItems.first());
            Item *endItem = qgraphicsitem_cast<Item *>(endItems.first());
            Line *line = new Line(startItem, endItem);
            startItem->addLine(line);
            endItem->addLine(line);
            line->setZValue(-1000.0);
            addItem(line);
            line->updatePosition();
        //}
    }
    line = 0;*/

    QGraphicsScene::mouseReleaseEvent(mouseEvent);

}

bool Scene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

