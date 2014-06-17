#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>

class Vertex;

class Edge : public QObject, public QGraphicsLineItem
{
	Q_OBJECT

	public:
		Edge(Vertex *start, Vertex *end);

		enum { Type = UserType + 2 };
		int type() const { return Type; }

		Vertex *start() const;
		Vertex *end() const;

		void updatePosition();

	private:
		Vertex *m_start;
		Vertex *m_end;
};

#endif // EDGE_H
