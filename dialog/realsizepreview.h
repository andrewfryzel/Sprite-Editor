#ifndef REALSIZEPREVIEW_H
#define REALSIZEPREVIEW_H

#include <QDialog>

namespace Ui {
class RealSizePreview;
}

class RealSizePreview : public QDialog
{
	Q_OBJECT
	
public:
	explicit RealSizePreview(QWidget *parent = nullptr);
	~RealSizePreview();
	
	void SetWidget(QWidget *Widget);
	
private:
	Ui::RealSizePreview *ui;
};

#endif