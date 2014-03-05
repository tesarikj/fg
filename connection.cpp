#include "connection.h"

#include "port.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include <QPainter>

Connection::Connection(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsPathItem(parent, scene)
{
    setPen(QPen(Qt::black, 1));         //barva a šířka čáry
    setBrush(Qt::NoBrush);              //nevybarvovat čáru
    setZValue(-1);
    m_port1 = 0;
    m_port2 = 0;
    numberPoint=0;

    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

Connection::~Connection()
{
    if (m_port1)
        m_port1->connections().remove(m_port1->connections().indexOf(this));
    if (m_port2)
        m_port2->connections().remove(m_port2->connections().indexOf(this));
}

void Connection::setPos1(const QPointF &p)
{
    pos1 = p;
    //arrayPoint.append(pos1);
}

void Connection::setPos2(const QPointF &p)
{
    pos2 = p;
}

void Connection::setPort1(Port *p)
{
    m_port1 = p;

    m_port1->connections().append(this);
}

void Connection::setPort2(Port *p)
{
    m_port2 = p;

    m_port2->connections().append(this);
}

void Connection::updatePosFromPorts()
{
    pos1 = m_port1->scenePos();
    pos2 = m_port2->scenePos();
}

void Connection::setPoint(const QPointF &p)
{
    //point = p;
}

void Connection::addPoint(const QPointF &p)
{
    //arrayPoint.append(p);
    point = p;
    numberPoint=+1;
    updatePath();
}

void Connection::updatePath()
{
    QPainterPath p;
    p.moveTo(pos1);             //umístí Painter do místa portu1

    if((point.x()<pos1.x()+10) && point.x()>pos1.x()-10)

    /*QPointF point1;
    point1.setX(point.x());
    point1.setY(pos2.y());
    QPointF point2 = QPointF(point.x(),pos2.y());*/
    p.quadTo(pos1,point);

    for(int x=0; x<numberPoint; x++){
        p.quadTo(point,pos2);       //vykreslí křivku z portu1 do portu 2
        QPointF oldPoint;
        oldPoint=point;
    }

    setPath(p);
}

Port* Connection::port1() const
{
    return m_port1;
}

Port* Connection::port2() const
{
    return m_port2;
}
