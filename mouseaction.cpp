#include "mouseaction.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include "port.h"
#include "connection.h"
#include "libstructure.h"

MouseAction::MouseAction(QObject *parent) :
    QObject(parent)
{
	conn = 0;
}

void MouseAction::install(QGraphicsScene *s)
{
	s->installEventFilter(this);
	scene = s;
}

QGraphicsItem* MouseAction::itemAt(const QPointF &pos)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}

bool MouseAction::eventFilter(QObject *o, QEvent *e)
{
    QGraphicsSceneMouseEvent *mouseEvent = (QGraphicsSceneMouseEvent*) e;

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
        switch ((int) mouseEvent->button())
        {
        case Qt::LeftButton:
		{
            QGraphicsItem *item = itemAt(mouseEvent->scenePos());
            if (conn == 0 && item && item->type() == Port::Type)
			{
                conn = new Connection(0, scene);
                conn->setPort1((Port*) item);
				conn->setPos1(item->scenePos());
                conn->setPos2(mouseEvent->scenePos());
                conn->updatePath();

                return true;
            }

            if (conn && mouseEvent->button() == Qt::LeftButton)
            {
                QGraphicsItem *item = itemAt(mouseEvent->scenePos());
                if (item && item->type() == Port::Type)
                {
                     Port *port1 = conn->port1();
                     Port *port2 = (Port*) item;

                     if (port1->block() != port2->block() &&         // nesmí se připojit vstup s výstupem stejného portu
                         port1->isOutput() != port2->isOutput() &&   // nesmí se k sobě připojit dva porty se stejnou směrovostí
                         !port1->isConnected(port2))                 // pokud je port1 připojen k portu2, tak je nelze připojit znovu
                         {
                         conn->setPos2(port2->scenePos());
                         conn->setPort2(port2);
                         conn->updatePath();
                         conn = 0;
                         return true;
                         }
                 }

                 conn->setPoint(mouseEvent->scenePos());
                 //conn->updatePath();
                 conn->addPoint(mouseEvent->scenePos());
                 //conn->updatePath();
                 //delete conn;
                 //conn = 0;
                 return true;
             }

            break;
        }
        case Qt::RightButton:
        {
            if(conn)
            {
                delete conn;
                conn = 0;
                return true;
            }
            break;
        }
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		if (conn)
		{
            //conn->setPos2(mouseEvent->scenePos());
            conn->addPoint(mouseEvent->scenePos());
            conn->updatePath();
            return true;
		}
		break;
	}
    case QEvent::GraphicsSceneMouseRelease:
    {
        /*if (conn && mouseEvent->button() == Qt::LeftButton)
        {
            QGraphicsItem *item = itemAt(mouseEvent->scenePos());
            if (item && item->type() == Port::Type)
			{
                Port *port1 = conn->port1();
                Port *port2 = (Port*) item;

                if (port1->block() != port2->block() &&         // nesmí se připojit vstup s výstupem stejného portu
                    port1->isOutput() != port2->isOutput() &&   // nesmí se k sobě připojit dva porty se stejnou směrovostí
                    !port1->isConnected(port2))                 // pokud je port1 připojen k portu2, tak je nelze připojit znovu
				{
                    conn->setPos2(port2->scenePos());
                    conn->setPort2(port2);
                    conn->updatePath();
                    conn = 0;
					return true;
				}
			}

            delete conn;
            conn = 0;
            return true;
		}
        break;
       /*if (conn && me->button() == Qt::LeftButton)
        {
            conn->setPos2(me->scenePos());
            conn->updatePath();
            return true;
        }
        break;*/
    }
	}
    }
	return QObject::eventFilter(o, e);
}


