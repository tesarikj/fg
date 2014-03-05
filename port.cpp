#include "port.h"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "connection.h"

Port::Port(QGraphicsItem *parent, QGraphicsScene *scene):
    QGraphicsPathItem(parent, scene)
{
    label = new QGraphicsTextItem(this);

    radius_ = 5;
    margin = 2;

    QPainterPath p;
    p.addRect(-5,-5,10,10);                 //pozice a velikost portu
    setPath(p);

    setPen(QPen(Qt::black));
    setBrush(Qt::gray);

    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    m_portFlags = 0;
}

Port::~Port()
{
    foreach(Connection *conn, m_connections)
        delete conn;
}

void Port::setNEBlock(LibStructure *b)
{
    m_block = b;
}

void Port::setName(const QString &n)
{
    //name = n;
    label->setPlainText(n);
}

void Port::setIsOutput(bool o)
{
    isOutput_ = o;

    //QFontMetrics fm(scene()->font());
    //QRect r = fm.boundingRect(name);
    //pozice je oproti portům...
    if (isOutput_)
        label->setPos(-4 - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
    else
        label->setPos(radius_ + margin, -label->boundingRect().height()/2);
}

int Port::radius()
{
    return radius_;
}

bool Port::isOutput()
{
    return isOutput_;
}

QVector<Connection*>& Port::connections()
{
    return m_connections;
}

void Port::setPortFlags(int f)
{
    m_portFlags = f;

    if (m_portFlags & TypePort)
    {
        QFont font(scene()->font());
        font.setItalic(true);
        label->setFont(font);
        setPath(QPainterPath());
    } else if (m_portFlags & NamePort)
    {
        QFont font(scene()->font());
        font.setBold(true);
        label->setFont(font);
        setPath(QPainterPath());
    }
}

LibStructure* Port::block() const
{
    return m_block;
}

quint64 Port::ptr()
{
    return m_ptr;
}

void Port::setPtr(quint64 p)
{
    m_ptr = p;
}

bool Port::isConnected(Port *other)
{
    foreach(Connection *conn, m_connections)
        if (conn->port1() == other || conn->port2() == other)
            return true;

    return false;
}

QVariant Port::itemChange(GraphicsItemChange change, const QVariant &value)
{
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
