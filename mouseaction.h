#ifndef MouseAction_H
#define MouseAction_H

#include <QObject>

class QGraphicsScene;
class Connection;
class QGraphicsItem;
class QPointF;

class MouseAction : public QObject
{
	Q_OBJECT
public:
    explicit MouseAction(QObject *parent = 0);

	void install(QGraphicsScene *scene);

	bool eventFilter(QObject *, QEvent *);

private:
	QGraphicsItem *itemAt(const QPointF&);

private:
    QGraphicsScene *scene;
    Connection *conn;
};

#endif // MouseAction_H
