#include "libstructure.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"

#include "port.h"
#include <string>
#include <QVector>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include <math.h>

LibStructure::LibStructure(QGraphicsItem *parent, QGraphicsScene *scene, QMenu *contextMenu, QPointF location):
    QGraphicsPathItem(parent, scene),
    dragStart(50,50),
    colorPen(Qt::black),
    space(10),                 //rozměr mřížky
    location(location)
{
//zjistí se jméno objektu, hodnota, knihovna a druh objektu
    name="NOT";
    valueDev=1;
    libraryDev="Basic";
    device="not01";

//--------------------------------------------------------------------
    sizePart();                                     //nastaví rozměry do proměných widht a height
    createPart();                                   //z nastavených rozměrů vytvoří polygon
    createPort();                                   //vytvoří porty

    myContextMenu = contextMenu;                    //nastavení kontextového menu pro objekty

    setFlag(QGraphicsItem::ItemIsMovable, true);    //povolení pohybovaní s objekty
    setFlag(QGraphicsItem::ItemIsSelectable, true); //povolení vybírání objektů
}

//------------PODLE KNIHOVNY A DEVICE NASTAVÍ ROZMĚRY OBJEKTU-----------
void LibStructure::sizePart()
{
    width=100;                           // zjištěná šířka zařízení
    height=100;                          // zjištěná výška zařízení

}

//------------ VYTVOŘENÍ OBJEKTU PODLE ROZMĚRŮ--------------------------
void LibStructure::createPart()
{
    QPoint topLeft (0,0);
    QPoint topRight (width,0);
    QPoint bottomRight (width,height);
    QPoint bottomLeft (0, height);

    QPolygon polygon;
    polygon  << topLeft << topRight << bottomRight << bottomLeft << topLeft;

    QPainterPath p;
    p.addPolygon(polygon);
    setPath(p);

    text.setPlainText(QString::fromStdString(name));   //vysází text
    text.setParentItem(this);                          //přiřadí text k objektu
    text.setPos(width/4,height/20);                    //nastavení pozice názvu objektu

}
//------------VYTVOŘÍ PORTY PRO PŘIPOJENÍ----------------------------------
void LibStructure::createPort()
{
    int number_ports=3;                                 //načte počet portů

    QList<QString> names;                               //načte jména portů
    names << "X" << "Y" << "Z";

    QList<QString> direction;                           //načte směrovost portů
    direction << "IN" << "IN" << "OUT";

    for (int x = 0; x < number_ports; x++)
    {
        if (direction[x]=="IN"){
            addPort(names[x],false);
        }
        else if(direction[x]=="OUT"){
            addPort(names[x],true);
        }
    }
}

Port* LibStructure::addPort(const QString &name, bool isOutput, int flags, int ptr)
{
    Port *port = new Port(this);
    port->setName(name);
    port->setIsOutput(isOutput);
    port->setNEBlock(this);
    port->setPortFlags(flags);
    port->setPtr(ptr);

    //QFontMetrics fm(scene()->font());
    //int h = fm.height();

    int y1 = -height / 2 + height + port->radius();
    int y2 = -height / 2 + height + port->radius();
    foreach(QGraphicsItem *port_, children()) {
        if (port_->type() != Port::Type)
            continue;

        Port *port = (Port*) port_;
        if (port->isOutput()){

            port->setPos(width + port->radius(), y1-port->radius());
            y1 += 20;
        }
        else{
            port->setPos(-port->radius(), y2-port->radius());
            y2 += 20;
        }
    }
    return port;
}

void LibStructure::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    this->setPos(location);


    if(isSelected()){
        setSelected(false);
        //setSelected(true);
        borderPen.setStyle(Qt::SolidLine);

    }else {
        setSelected(true);
        //setSelected(false);
        borderPen.setStyle(Qt::DashLine);
    }
}

void LibStructure::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    event->setAccepted(true);
    dragStart = event->pos();               //načte se pozice, kde byla nad objektem stisknuta myš
}

void LibStructure::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    QPointF newPos = event->pos() ;
    location += (newPos - dragStart);

    setSelected(false);

    /*int x;
    int y;
    x=qRound(location.x()/space)*space;           //zaokrouhlení pozice na mřížku
    y=qRound(location.y()/space)*space;
    location.setX(x);
    location.setY(y);*/

    location.setX(qRound(location.x()/space)*space);
    location.setY(qRound(location.y()/space)*space);
    this->setPos(location);
}

// boundingRect must be re-implemented from the base class to provide the scene with
// size info about this custom GraphicsItem
QRectF LibStructure::boundingRect() const
{
    return QRectF(0,0,width,height);
}

void LibStructure::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant LibStructure::itemChange(GraphicsItemChange change, const QVariant &value)
{
    /*if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Line *line, lines) {
            line->updatePosition();
        }
    }*/
    if (change == ItemScenePositionHasChanged)
    {
        foreach(Connection *conn, m_connections)
        {
            conn->updatePosFromPorts();
            conn->updatePath();
        }
    }
    return value;
}

QVector<Port*> LibStructure::ports()
{
    QVector<Port*> res;
    foreach(QGraphicsItem *port_, childItems())
    {
        if (port_->type() == Port::Type)
            res.append((Port*) port_);
    }
    return res;
}

void LibStructure::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{    
    borderPen.setColor(colorPen);

    painter->setPen(borderPen);
    painter->drawPath(path());
}







