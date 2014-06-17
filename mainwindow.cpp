#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "startdialog.h"
#include "vertex.h"
#include "edge.h"

#include <QMessageBox>
#include <QFileDialog>
#include <qmath.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_ui(new Ui::MainWindow),
	m_scene(new GraphScene(this)),
	m_startVertex(0),
	m_stopVertex(0)
{
	m_ui->setupUi(this);

	m_modeMap.insert(m_ui->selectAction, GraphScene::Select);
	m_modeMap.insert(m_ui->addVerticleAction, GraphScene::AddVertex);
	m_modeMap.insert(m_ui->removeAction, GraphScene::Remove);

	m_ui->graphicsView->setScene(m_scene);
	connect(m_scene, &GraphScene::vertexChanged, this, &MainWindow::updateVertex);

	connect(&m_tickTimer, &QTimer::timeout, this, &MainWindow::tick);
	m_tickTimer.setInterval(100);

	clear();
}

MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::setMode()
{
	m_ui->selectAction->setChecked(false);
	m_ui->addVerticleAction->setChecked(false);
	m_ui->removeAction->setChecked(false);

	if(QAction *action = qobject_cast<QAction*>(sender())) {
		action->setChecked(true);
		m_scene->setMode(m_modeMap.value(action));
	}
}
void MainWindow::startAlgorithm()
{
	StartDialog dlg(this);
	dlg.setDiagonalNeighbours(m_diagonalNeighbours);
	dlg.setAnimationTime(m_tickTimer.interval());
	if(!dlg.exec())
		return;

	m_ui->graphicsView->setEnabled(false);
	m_ui->loadAction->setEnabled(false);
	m_ui->saveAction->setEnabled(false);
	m_ui->startAction->setEnabled(false);
	m_ui->resetAction->setEnabled(true);

	m_diagonalNeighbours = dlg.diagonalNeighbours();
	m_openList << m_startVertex;
	m_startVertex->opened = true;

	m_tickTimer.setInterval(dlg.animationTime());

	tick();
	m_tickTimer.start();
}

void MainWindow::resetAlgorithm()
{
	m_tickTimer.stop();

	foreach(QGraphicsItem *item, m_scene->items()) {
		if(item->type() == QGraphicsLineItem::Type) {
			delete item;
			continue;
		}

		if(item->type() != Vertex::Type)
			continue;

		Vertex *vertex = qgraphicsitem_cast<Vertex*>(item);
		vertex->prev = 0;
		vertex->f = INT_MAX;
		vertex->g = INT_MAX;
		vertex->h = INT_MAX;
		vertex->opened = false;
		vertex->closed = false;

		if(vertex->subType() == Vertex::Opened || vertex->subType() == Vertex::Closed)
			vertex->setSubType(Vertex::Empty);
	}

	m_openList.clear();

	m_ui->graphicsView->setEnabled(true);
	m_ui->loadAction->setEnabled(true);
	m_ui->saveAction->setEnabled(true);
	m_ui->startAction->setEnabled(true);
	m_ui->resetAction->setEnabled(false);
}

void MainWindow::tick()
{
	if(!m_openList.count()) {
		m_tickTimer.stop();

		QMessageBox::information(this, "Buuu :'(", "Po przeszukaniu całego grafu stwierdzam, że nie da się znaleźć żądanej ścieżki.");
		return;
	}

	qSort(m_openList.begin(), m_openList.end(), [](Vertex *a, Vertex *b) { return( a->f < b->f); });

	Vertex *vertex = m_openList.takeAt(0);
	vertex->closed = true;

	if(vertex == m_stopVertex) {
		m_tickTimer.stop();

		while(vertex->prev) {
			m_scene->addLine(QLineF(vertex->pos(), vertex->prev->pos()), QPen(Qt::yellow, 2.0))->setZValue(1000);

			vertex = vertex->prev;
		}

		return;
	}

	if(vertex != m_startVertex)
		vertex->setSubType(Vertex::Closed);

	auto neighbours = vertex->neighbours(m_diagonalNeighbours);
	for(int i = 0; i < neighbours.size(); ++i) {
		Vertex *neighbour = neighbours[i];

		if(neighbour->closed)
			continue;

		const double ng = vertex->g + ((neighbour->x() - vertex->x() == 0 || neighbour->y() - vertex->y() == 0) ? 1 : M_SQRT2);
		if(!neighbour->opened || ng < neighbour->g) {
			neighbour->g = ng;
			neighbour->h = abs(neighbour->x() - m_stopVertex->x()) + abs(neighbour->y() - m_stopVertex->y());
			neighbour->f = neighbour->g + neighbour->h;
			neighbour->prev = vertex;

			if(!neighbour->opened) {
				m_openList << neighbour;
				neighbour->opened = true;

				if(neighbour != m_stopVertex)
					neighbour->setSubType(Vertex::Opened);
			}
		}
	}
}

void MainWindow::save()
{
	const QString fileName = QFileDialog::getSaveFileName(this, QString(), QString(), "Skompresowane pliki json (*.dat)");
	if(fileName.isEmpty())
		return;

	m_scene->saveAs(fileName);
}

void MainWindow::load()
{
	const QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(), "Skompresowane pliki json (*.dat)");
	if(fileName.isEmpty())
		return;

	m_scene->load(fileName);
}

void MainWindow::clear()
{
	m_scene->clear();

	for(int x = -2032; x <= 2032; x += 32) {
		for(int y = -2032; y <= 2032; y += 32) {
			m_scene->addItem(new Vertex(Vertex::Empty, QPoint(x, y)));
		}
	}

	m_startVertex = qgraphicsitem_cast<Vertex*>(m_scene->itemAt(QPoint(-176,-16), QTransform()));
	m_startVertex->setSubType(Vertex::Start);

	m_stopVertex = qgraphicsitem_cast<Vertex*>(m_scene->itemAt(QPoint(144,-16), QTransform()));
	m_stopVertex->setSubType(Vertex::Stop);
}

void MainWindow::updateVertex(Vertex *vertex)
{
	if(vertex->subType() == Vertex::Start) {
		m_startVertex = vertex;
	} else if(vertex->subType() == Vertex::Stop) {
		m_stopVertex = vertex;
	}
}
