#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

class Vertex;

class GraphScene : public QGraphicsScene
{
	Q_OBJECT

	public:
		enum Mode
		{
			Select = 0,
			AddVertex,
			Remove
		};

	public:
		explicit GraphScene(QObject *parent = 0);

		void setMode(const Mode &mode);

		void saveAs(const QString &fileName);
		void load(const QString &fileName);
		void clear();

	signals:
		void vertexChanged(Vertex *vertex);

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	private:
		QGraphicsItem *m_moveItem;
		QGraphicsLineItem *m_line;
		int m_maxId;
		Mode m_mode;
};

#endif // GRAPHSCENE_H
