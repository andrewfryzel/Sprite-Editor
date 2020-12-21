#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

#include "exportmanager.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit ExportDialog(QWidget *parent = nullptr);
	~ExportDialog();

private:
	Ui::ExportDialog *ui;
};

#endif
