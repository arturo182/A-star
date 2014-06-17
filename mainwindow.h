#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphscene.h"

#include <QMainWindow>
#include <QTimer>
#include <QMap>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void setMode();
		void startAlgorithm();
		void resetAlgorithm();
		void tick();
		void save();
		void load();
		void clear();
		void updateVertex(Vertex *vertex);
		
	private:
		Ui::MainWindow *m_ui;
		GraphScene *m_scene;
		QMap<QAction*, GraphScene::Mode> m_modeMap;

		QList<Vertex*> m_openList;
		bool m_diagonalNeighbours;
		Vertex *m_startVertex;
		Vertex *m_stopVertex;
		QTimer m_tickTimer;
};

#endif // MAINWINDOW_H
