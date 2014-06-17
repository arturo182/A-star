#ifndef VERTEX_H
#define VERTEX_H

#include <QGraphicsRectItem>

class Edge;

class Vertex : public QObject, public QGraphicsRectItem
{
	Q_OBJECT

	public:
		enum SubType
		{
			Start = 0,
			Stop,
			Empty,
			Opened,
			Closed,
			Wall
		};

	public:
		Vertex(const SubType &subType, const QPoint &pos);

		Vertex *prev;
		double f;
		double g;
		double h;
		bool opened;
		bool closed;

		enum { Type = UserType + 1 };
		int type() const { return Type; }

		void resetColors();

		QList<Vertex*> neighbours(bool diagonal = true) const;

		SubType subType() const;
		void setSubType(const SubType &subType);

	signals:
		void posChanged();

	protected:
		//void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	private:
		QList<Edge*> m_edges;
		SubType m_subType;
};

#endif // VERTEX_H
