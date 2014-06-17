#include "startdialog.h"
#include "ui_startdialog.h"

#include <QMessageBox>

StartDialog::StartDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::StartDialog)
{
	m_ui->setupUi(this);
}

StartDialog::~StartDialog()
{
	delete m_ui;
}

void StartDialog::setAnimationTime(const int &ms)
{
	m_ui->timeSpinBox->setValue(ms);
}

int StartDialog::animationTime() const
{
	return m_ui->timeSpinBox->value();
}

void StartDialog::setDiagonalNeighbours(bool diagonal)
{
	m_ui->checkBox->setChecked(diagonal);
}

bool StartDialog::diagonalNeighbours() const
{
	return m_ui->checkBox->isChecked();
}
