#include "realsizepreview.h"
#include "ui_realsizepreview.h"

#include <iostream>

RealSizePreview::RealSizePreview(QWidget *parent) :
QDialog(parent),
ui(new Ui::RealSizePreview)
{
	ui->setupUi(this);
}

RealSizePreview::~RealSizePreview()
{
	delete ui;
}


void RealSizePreview::SetWidget(QWidget *Widget)
{
	/* Consider checking if there was a widget there. */
	this->ui->Container->layout()->addWidget(Widget);
	std::cout << "THIS HAPPENED " << std::endl;
}