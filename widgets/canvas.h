/*
 *	canvas.h
 *	The core class for management of the canvas.
 *	Author:
 *		Natalia Soto (Oct 30, 2019)
 * 
 *	Reviewed:
 *		Brian Schnepp (October 31, 2019)
 *			Reformatted to fit style guide,
 *			corrected naming conventions.	
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QLabel>

#include <tools.h>

class Canvas : public QFrame
{

Q_OBJECT

public:
	Canvas(QWidget *Parent = nullptr);
	virtual ~Canvas() override;
	
	uint32_t GetContentWidth();
	uint32_t GetContentHeight();
	
signals:
	void LayerChanged(QPixmap OldPixmap, uint32_t OldIndex);

public slots:
	void Clear();
	
	QColor GetBrushColor();
	void SetBrushColor(QColor);
	
	void SetCurrentPixmap(QPixmap NewMap, int LayerIndex);
	void SetCurrentBrush(Brush *NewCur);
	void SetScale(float NewScale);
	int GetLayerIndex();

protected:
	void paintEvent(QPaintEvent *) override;
	void mousePressEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	/* void resizeEvent(QResizeEvent *); */

private:
	/* These should be appropriately renamed to fit the style guide. */
	Brush *CurBrush;
	
	QPixmap CurPixmap;
	QImage *CurCanvas;
	QColor CurBrushColor;
	QPoint CurBeginDrawPosition;
	QPoint CurEndDrawPosition;
	bool IsBrushEnabled;
	
	float ScaleAmount;
	int LayerIndex;
};

#endif