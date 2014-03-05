#include "gridlines.h"
#include <QColor>

GridLines::GridLines( int w, int h, int s) : QGraphicsItem(),
            gridwidth(w), gridheight(h), gridspace(s)
{

}

void GridLines::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c (200,200,255,125);

    painter->setPen(c);

    for (int y= 0; y < gridheight; y+=gridspace)            //horizontální čáry
    {
        painter->drawLine(0,y, gridwidth, y);
    }

    for (int x= 0; x < gridwidth; x+=gridspace)             //vertikální čáry
    {
        painter->drawLine(x,0,x, gridheight);
    }

}

QRectF GridLines::boundingRect() const
{
    return QRectF ( static_cast<qreal>(0), static_cast<qreal>(0), static_cast<qreal>(gridwidth), static_cast<qreal>(gridheight));
}

void GridLines::handleWindowSizeChanged(int w, int h)
{
    gridwidth = w;
    gridheight = h;
}

/*
void GridLines::mouseMoveEvent(QGraphicsSceneDragDropEvent *)
{
}

void GridLines::mousePressEvent(QGraphicsSceneDragDropEvent *)
{
}

void GridLines::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
}

void GridLines::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
}
*/
