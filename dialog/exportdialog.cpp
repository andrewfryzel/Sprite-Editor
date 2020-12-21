#include "exportdialog.h"
#include "ui_exportdialog.h"
#include <QFileDialog>

ExportDialog::ExportDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ExportDialog)
{
	ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
	delete ui;
}
