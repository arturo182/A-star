#include "graphscene.h"

#include "vertex.h"
#include "edge.h"

#include <QGraphicsSceneMouseEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPainter>
#include <QDebug>
#include <QFile>

GraphScene::GraphScene(QObject *parent) :
	QGraphicsScene(parent),
	m_moveItem(0),
	m_line(0),
	m_maxId(0),
	m_mode(Select)
{
	setSceneRect(-2048, -2048, 4096, 4096);
}

void GraphScene::setMode(const GraphScene::Mode &mode)
{
	m_mode = mode;
}

void GraphScene::saveAs(const QString &fileName)
{
	QJsonArray vertices;
	/*foreach(QGraphicsItem *item, items()) {
		if(item->type() != Vertex::Type)
			continue;

		Vertex *vertex = qgraphicsitem_cast<Vertex*>(item);

		QJsonObject jsonObj;
		jsonObj.insert("x", vertex->pos().x());
		jsonObj.insert("y", vertex->pos().y());
		jsonObj.insert("id", vertex->id());
		vertices.append(jsonObj);
	}

	QJsonArray edges;
	foreach(QGraphicsItem *item, items()) {
		if(item->type() != Edge::Type)
			continue;

		Edge *edge = qgraphicsitem_cast<Edge*>(item);

		QJsonObject jsonObj;
		jsonObj.insert("start", edge->start()->id());
		jsonObj.insert("end", edge->end()->id());
		edges.append(jsonObj);
	}

	QJsonObject rootObj;
	rootObj.insert("vertices", vertices);
	rootObj.insert("edges", edges);

	QJsonDocument doc(rootObj);
	QFile file(fileName);
	if(!file.open(QFile::WriteOnly))
		return;

	file.write(qCompress(doc.toJson(), 9));
	file.close();*/
}

void GraphScene::load(const QString &fileName)
{
	clear();

	/*QFile file(fileName);
	if(!file.open(QFile::ReadOnly))
		return;

	QJsonDocument doc = QJsonDocument::fromJson(qUncompress(file.readAll()));
	file.close();

	QJsonObject rootObj = doc.object();

	QJsonArray vertices = rootObj.value("vertices").toArray();
	for(int i = 0; i < vertices.count(); ++i) {
		const QJsonObject vertexObj = vertices[i].toObject();
		const int id = vertexObj.value("id").toDouble();
		const int x = vertexObj.value("x").toDouble();
		const int y = vertexObj.value("y").toDouble();

		addItem(new Vertex(id, QPoint(x, y)));
		m_maxId = qMax(m_maxId, id);
	}

	if(m_maxId > 0)
		++m_maxId;

	QJsonArray edges = rootObj.value("edges").toArray();
	for(int i = 0; i < edges.count(); ++i) {
		const QJsonObject edgeObj = edges[i].toObject();

		Vertex *startVertex = vertexById(edgeObj.value("start").toDouble());
		Vertex *endVertex = vertexById(edgeObj.value("end").toDouble());

		if(!startVertex || !endVertex)
			continue;

		Edge *edge = new Edge(startVertex, endVertex);
		startVertex->addEdge(edge);
		endVertex->addEdge(edge);
		edge->updatePosition();
		addItem(edge);
	}

	update();*/
}

void GraphScene::clear()
{
	QGraphicsScene::clear();

	m_maxId = 0;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	switch(m_mode) {
		case Select:
		{
			foreach(QGraphicsItem *item, items(event->scenePos())) {
				if(item->type() != Vertex::Type)
					continue;

				Vertex *vertex = qgraphicsitem_cast<Vertex*>(item);
				if((vertex->subType() != Vertex::Start) && (vertex->subType() != Vertex::Stop))
					continue;

				m_moveItem = item;
				break;
			}
		}
		break;

		case AddVertex:
		{
			QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
			if(!item)
				break;

			Vertex *vertex = qgraphicsitem_cast<Vertex*>(item);
			if(vertex->subType() == Vertex::Empty)
				vertex->setSubType(Vertex::Wall);
		}
		break;

		case Remove:
		{
			QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
			if(!item)
				break;

			Vertex *vertex = qgraphicsitem_cast<Vertex*>(item);
			if(vertex->subType() == Vertex::Wall)
				vertex->setSubType(Vertex::Empty);
		}
		break;
	}

	QGraphicsScene::mousePressEvent(event);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	switch(m_mode) {
		case Select:
		{
			m_moveItem = 0;
		}
		break;

		default:
			break;
	}

	QGraphicsScene::mouseReleaseEvent(event);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	switch(m_mode) {
		case Select:
		{
			if(!m_moveItem)
				break;

			QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
			if(!item)
				break;

			Vertex *vertex = qgraphicsitem_cast<Vertex*>(item);
			if(vertex->subType() != Vertex::Empty)
				break;

			Vertex *moveVertex = qgraphicsitem_cast<Vertex*>(m_moveItem);

			vertex->setSubType(moveVertex->subType());
			moveVertex->setSubType(Vertex::Empty);

			emit vertexChanged(vertex);

			m_moveItem = item;
		}
		break;

		case Remove:
		case AddVertex:
		{
			mousePressEvent(event);
		}
		break;

		default:
			break;
	}

	QGraphicsScene::mouseMoveEvent(event);
}
