#ifndef PORT_H
#define PORT_H

#include <QGraphicsPathItem>

class LibStructure;
class Connection;

class Port : public QGraphicsPathItem
{
public:
    enum { Type = QGraphicsItem::UserType + 1 };
    enum { NamePort = 1, TypePort = 2 };

    Port(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~Port();

    void setNEBlock(LibStructure *);
    void setName(const QString &n);
    void setIsOutput(bool o);
    int radius();
    bool isOutput();
    QVector<Connection*>& connections();
    void setPortFlags(int);

    const QString& portName() const { return name; }
    int portFlags() const { return m_portFlags; }

    int type() const { return Type; }

    LibStructure* block() const;

    quint64 ptr();
    void setPtr(quint64);

    bool isConnected(Port*);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    LibStructure *m_block;
    QString name;
    bool isOutput_;
    QGraphicsTextItem *label;
    int radius_;
    int margin;
    QVector<Connection*> m_connections;
    int m_portFlags;
    quint64 m_ptr;
};

#endif // PORT_H
