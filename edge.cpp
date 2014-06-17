#include "edge.h"

#include "vertex.h"

#include <QDebug>
#include <QPen>

Edge::Edge(Vertex *start, Vertex *end) :
	m_start(start),
	m_end(end)
{
	setPen(QPen(QColor(100, 140, 220), 2.0));

	connect(start, &Vertex::posChanged, this, &Edge::updatePosition);
	connect(end, &Vertex::posChanged, this, &Edge::updatePosition);

	updatePosition();
}

Vertex *Edge::start() const
{
	return m_start;
}

Vertex *Edge::end() const
{
	return m_end;
}

void Edge::updatePosition()
{
	setLine(QLineF(m_start->pos(), m_end->pos()));
}
