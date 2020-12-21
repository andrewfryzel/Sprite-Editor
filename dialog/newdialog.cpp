#include "newdialog.h"
#include "ui_newdialog.h"

NewDialog::NewDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::NewDialog)
{
	ui->setupUi(this);
}

NewDialog::~NewDialog()
{
	delete ui;
}


int NewDialog::GetWidth()
{
	QString CurWidth = this->ui->width->text();
	bool Okay = false;
	int RealVal = CurWidth.toInt(&Okay);
	if (!Okay)
	{
		return 32;
	}
	if (RealVal >= 256)
	{
		return 256;
	}
	return RealVal;
}

int NewDialog::GetHeight()
{
	QString CurHeight = this->ui->height->text();
	bool Okay = false;
	int RealVal = CurHeight.toInt(&Okay);
	if (!Okay)
	{
		return 32;
	}
	if (RealVal >= 256)
	{
		return 256;
	}
	return RealVal;
}