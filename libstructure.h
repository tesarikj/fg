#ifndef LIBSTRUCTURE_H
#define LIBSTRUCTURE_H

#include <QtGui>
#include <QObject>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QBrush>

#include <string>
#include <vector>
#include <map>

using namespace std;

class QGraphicsSceneContextMenuEvent;

class QGraphicsPathItem;
class LibStructure;
class Port;
class Connection;

class LibStructure : public QGraphicsPathItem {
    public:
    LibStructure(QGraphicsItem *parent = 0,
                 QGraphicsScene *scene = 0,
                 QMenu *contextMenu=0,
                 QPointF location=QPointF (0,0));

        QGraphicsTextItem text;
        QMenu *myContextMenu;

        enum { Type = QGraphicsItem::UserType + 3 };
        Port* addPort(const QString &name, bool isOutput, int flags = 0, int ptr = 0);
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
        QVector<Port*> ports();

        int type() const { return Type; }


    private:
        int width;
        int height;
        int space;

        int valueDev;
        string libraryDev;
        string device;
        string name;

        QPen borderPen;
        QPointF location;
        QPointF dragStart;
        QColor colorPen;
        QVector<Connection*> m_connections;

        void sizePart();                //nastaví rozměry do proměných widht a height
        void createPart();              //z nastavených rozměrů vytvoří polygon
        void createPort();              //vytvoří porty

        virtual QRectF boundingRect() const; ///< must be re-implemented in this class to provide the diminsions of the box to the QGraphicsView
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
        virtual void mousePressEvent (QGraphicsSceneMouseEvent * event );
        virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event );

    protected:
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};


#endif // LIBSTRUCTURE_H
