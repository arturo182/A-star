#include "vertex.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPen>

Vertex::Vertex(const SubType &subType, const QPoint &pos) :
	QGraphicsRectItem(-16, -16, 32, 32),
	m_subType(subType),
	prev(0),
	f(INT_MAX),
	g(INT_MAX),
	h(INT_MAX),
	opened(false),
	closed(false)
	//m_id(id)
{
	setPos(pos);
	resetColors();
	setZValue(100);
}

void Vertex::resetColors()
{
	switch(m_subType) {
		case Start:
		{
			setPen(QPen(QColor(90, 130, 43), 2));
			setBrush(QColor(110, 170, 60));
		}
		break;

		case Stop:
		{
			setPen(QPen(QColor(153, 65, 55), 2));
			setBrush(QColor(173, 70, 55));
		}
		break;

		case Wall:
		{
			setPen(QPen(QColor(115, 115, 115), 2));
			setBrush(QColor(128, 128, 128));
		}
		break;

		case Opened:
		{
			setPen(QPen(QColor(141, 193, 193), 2));
			setBrush(QColor(175, 238, 238));
		}
		break;

		case Closed:
		{
			setPen(QPen(QColor(137, 226, 137), 2));
			setBrush(QColor(152, 251, 152));
		}
		break;

		case Empty:
		{
			setPen(QPen(QColor(207, 207, 207), 2));
			setBrush(QColor(255, 255, 255));
		}
		break;
	}
}

QList<Vertex*> Vertex::neighbours(bool diagonal) const
{
	QList<Vertex*> list;
	Vertex *v = 0;

	//left
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(-32,   0), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	//right
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(32,    0), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	//top
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(0,   -32), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	//bottom
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(0,    32), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	if(!diagonal)
		return list;

	//topLeft
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(-32, -32), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	//topRight
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(32,  -32), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	//bottomLeft
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(-32,  32), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	//bottomRight
	v = qgraphicsitem_cast<Vertex*>(scene()->itemAt(pos().toPoint() + QPoint(32,   32), QTransform()));
	if(v && (v->subType() != Vertex::Wall)) list << v;

	return list;
}

Vertex::SubType Vertex::subType() const
{
	return m_subType;
}

void Vertex::setSubType(const Vertex::SubType &subType)
{
	if(m_subType == subType)
		return;

	m_subType = subType;
	resetColors();

	update();
}

/*void Vertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsRectItem::paint(painter, option, widget);

	painter->save();

	painter->setPen(QPen(Qt::black, 1));
	painter->drawText(rect().adjusted(2, 2, -2, -2), QString::number(m_id), QTextOption(Qt::AlignCenter));

	painter->restore();
}*/
