#ifndef CONNECTION_H
#define CONNECTION_H

#include <QGraphicsPathItem>

class Port;
class QPainter;

class Connection : public QGraphicsPathItem
{
public:
    enum { Type = QGraphicsItem::UserType + 2 };

    Connection(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~Connection();

    void setPos1(const QPointF &p);
    void setPos2(const QPointF &p);
    void setPort1(Port *p);
    void setPort2(Port *p);
    void updatePosFromPorts();
    void setPoint(const QPointF &p);
    void updatePath();
    void addPoint(const QPointF &p);
    Port* port1() const;
    Port* port2() const;

    int type() const { return Type; }

private:
    QVector<QPointF> arrayPoint;
    int numberPoint;
    QPointF pos1;
    QPointF pos2;
    Port *m_port1;
    Port *m_port2;
    QPointF point;
};

#endif // CONNECTION_H
