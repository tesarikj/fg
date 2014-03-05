#include <QtGui>

#include "scene.h"
#include "mainwindow.h"

Scene::Scene(QMenu *itemMenu, QObject *parent): QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    //line = 0;
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
        /*case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            line->setPen(QPen(Qt::black, 2));
            addItem(line);
            break;*/
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
     if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

bool Scene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

