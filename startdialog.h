#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

namespace Ui
{
	class StartDialog;
}

class StartDialog : public QDialog
{
	Q_OBJECT
		
	public:
		explicit StartDialog(QWidget *parent = 0);
		~StartDialog();

		void setAnimationTime(const int &ms);
		int animationTime() const;

		void setDiagonalNeighbours(bool diagonal);
		bool diagonalNeighbours() const;

	private:
		Ui::StartDialog *m_ui;
};

#endif // STARTDIALOG_H
