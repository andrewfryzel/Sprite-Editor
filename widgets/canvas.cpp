/*
 *	canvas.cpp
 *	The core class for management of the canvas.
 *	Author:
 *		Natalia Soto (Oct 30, 2019)
 */

#include "widgets/canvas.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QImage>
#include <QRgb>

#include <iostream>

/* Setup an empty drawing canvas */
Canvas::Canvas(QWidget *parent) : QFrame(parent)
{
	this->CurCanvas = new QImage(128, 128, 
		QImage::Format_ARGB32_Premultiplied
	);
	
	this->LayerIndex = 0;

	/* No brush yet. */
	this->CurBrush = nullptr;
	this->IsBrushEnabled = false;
	this->CurBrushColor = QColor(Qt::black);
	this->ScaleAmount = 4.0f;
	this->Clear();
}

/* Deletes stuff no longer in use. */
Canvas::~Canvas()
{
	delete this->CurCanvas;
}


void Canvas::SetCurrentBrush(Brush *CurBrush)
{
	this->CurBrush = CurBrush;
}

/* Handles the repainting of the widget. */
void Canvas::paintEvent(QPaintEvent *Evt)
{
	/* We need a graphics context to draw with. */
	QPainter Painter(this);
	
	/* 
	 * The image on screen is *NOT* the same as the backing buffer.
	 * It's a scaled representation of it. However, we want to draw
	 * the scaled version, and leave the backing buffer alone.
	 */
	int ScaledWidth = static_cast<int>(this->CurCanvas->width() 
		* this->ScaleAmount);
	int ScaledHeight = static_cast<int>(this->CurCanvas->height() 
		* this->ScaleAmount);
	/* QPixmap (nicely) by default uses nearest-neighbor. */
	QPixmap ScaledMap = QPixmap::fromImage(
		this->CurCanvas->scaled(ScaledWidth, ScaledHeight)
	);
	
	/* To place the canvas in the middle of the screen. */
	int XPos = (this->width() / 2) - (ScaledMap.width() / 2);
	int YPos = (this->height() / 2) - (ScaledMap.height() / 2);
	
	/* 
	 * Draw a nice black box around it (in native size), to clearly
	 * show where the drawable area is. 
	 */
	QRect BoundingBox = ScaledMap.rect();
	BoundingBox.setX(XPos - 1);
	BoundingBox.setY(YPos - 1);
	/* No addition here since width/height is 1 more than the last pixel. */
	BoundingBox.setWidth(XPos + BoundingBox.width());
	BoundingBox.setHeight(YPos + BoundingBox.height());
	Painter.setPen(Qt::black);
	Painter.drawRect(BoundingBox);
	
	/* Make sure it's always possible to scroll to everything. */
	this->setMinimumSize(BoundingBox.size() * 1.1);
	
	/* Now draw the scaled up image! */
	Painter.drawPixmap(XPos, YPos, ScaledMap);
	Evt->accept();
}

/* Initiation of drawing stuff. */
void Canvas::mousePressEvent(QMouseEvent *Evt)
{
	this->IsBrushEnabled = true;
	this->CurBeginDrawPosition = Evt->pos();
	this->mouseMoveEvent(Evt);
	Evt->accept();
}


/* An internal-only helper function to check if we're in
 * the mapped bounds of a rectangle. 
 */
bool InternalCheckIfPointValid(QPoint Point, QRect Rect)
{
	if (Point.x() >= Rect.x() && Point.y() >= Rect.y())
	{
		if (Point.x() <= Rect.x() + Rect.width() 
			&& Point.y() <= Rect.y() + Rect.width())
		{
			return true;
		}
	}
	return false;
}

/* Sets the scale to render at. */
void Canvas::SetScale(float NewScale)
{
	this->ScaleAmount = NewScale;
}

/* Replaces the backing pixmap (ie, layer change event) */
void Canvas::SetCurrentPixmap(QPixmap NewMap, int LayerIndex)
{
	QPixmap RealCurrent = QPixmap::fromImage(*this->CurCanvas);
	emit this->LayerChanged(RealCurrent,
		static_cast<uint32_t>(this->LayerIndex)
	);
	this->LayerIndex = LayerIndex;
	/* Update accordingly. */
	this->CurPixmap = NewMap;
	this->LayerIndex = LayerIndex;
	delete this->CurCanvas;
	this->CurCanvas = new QImage(this->CurPixmap.toImage());
	this->repaint();
}

/* A helper function to get the sign of a number.
 * This should be used over left shift on a signed
 * integer to avoid undefined behavior.
 */
int InternalGetSign(int Val)
{
	if (Val < 0)
	{
		return -1;
	}
	return 1;
}

/* Gets the next position on a line between the current, and the end. */
QPoint InteralGetNextPoint(QPoint Cur, QPoint End)
{
	QPoint Diff(0, 0);
	if (Cur.x() < End.x())
	{
		Diff.setX(1);
	} else if (Cur.x() > End.x())
	{
		Diff.setX(-1);
	} /* Do nothing if equal. */
	
	if (Cur.y() < End.y())
	{
		Diff.setY(1);
	} else if (Cur.y() > End.y())
	{
		Diff.setY(-1);
	} /* Do nothing if equal. */
	return Cur + Diff;
}

/* When the mouse moves, the user could be trying to draw. */
void Canvas::mouseMoveEvent(QMouseEvent *Evt)
{
	if(!IsBrushEnabled) 
	{
		Evt->accept();
		return;
	}
	/* 
	 * We want to store the current moving position of the mouse 
	 * as the "end" position 
	 */
	 
	this->CurEndDrawPosition = Evt->pos();
	/*	Before anything happens, we want to "re-center", that is, 
	 *	correct the position since we draw at the center of the widget, 
	 *	instead of the top left corner.
	 * 
	 *	In graphics terminology, we're in model space. We want world
	 *	space, except we don't have to fight with an MVP matrix.
	 */
	QPoint RealStartPos = this->CurBeginDrawPosition;
	QPoint RealEndPos = this->CurEndDrawPosition;
	
	QSize Bounds = this->CurCanvas->size() 
		* static_cast<double>(this->ScaleAmount);
	
	/* And now convert to world space. */
	int XPos = (this->width() / 2);
	int YPos = (this->height() / 2);
	/* To make arithmetic easier, create a dummy point to subtract with. */
	QPoint RemapPoint(XPos, YPos);
	/* We want to map into world space. That starts at (0, 0).
	 * Right now, we're in some place in the middle of the canvas widget,
	 * where the top left corner (to map to (0, 0)), is somewhere in
	 * the middle of it.
	 */
	RealStartPos -= RemapPoint;
	RealEndPos -= RemapPoint;
	/* For scaling, now bring it down to the appropriate size. */
	QPoint ScalePoint(Bounds.width(), Bounds.height());	
	RealStartPos /= static_cast<double>(this->ScaleAmount);
	RealEndPos /= static_cast<double>(this->ScaleAmount);
	/* And this is done *after* to try and improve point-perfect accuracy.*/
	double DoubleScaleAmount = static_cast<double>(this->ScaleAmount * 2);
	RealStartPos = RealStartPos + (ScalePoint / DoubleScaleAmount);
	RealEndPos = RealEndPos + (ScalePoint / DoubleScaleAmount);

	
	/* Setup the pen and brush for the QPainter Brush */
	QPainter Painter(this->CurCanvas);
	QPen CurPen(this->CurBrushColor);
	Painter.setPen(CurPen);
	if (this->CurBrush == nullptr)
	{
		return;
	}
	uint32_t MaxWidth = this->CurBrush->Width;
	uint32_t MaxIndex = this->CurBrush->Width * this->CurBrush->Height;
	for (uint32_t Idx = 0; Idx < MaxIndex; ++Idx)
	{
		uint8_t SetAlpha = this->CurBrush->Map[Idx];
		int CurAlpha = this->CurBrushColor.alpha();
		uint32_t X = Idx % MaxWidth;
		uint32_t Y = Idx / MaxWidth;
		QPoint Shifting(Y, X);
		QColor DrawyColor = this->CurBrushColor;
		uint8_t NewAlpha = static_cast<uint8_t>((CurAlpha / 255.0) 
			* (SetAlpha));
		DrawyColor.setAlpha(NewAlpha);
		if (this->CurBrush->IsEraser)
		{
			QPoint DrawSpot = RealStartPos;
			while (DrawSpot != RealEndPos)
			{
				DrawyColor.setAlpha(0xFF - DrawyColor.alpha());
				this->CurCanvas->setPixel(DrawSpot.x(), 
					DrawSpot.y(), 0
				);
				DrawSpot = InteralGetNextPoint(DrawSpot, 
					RealEndPos
				);
			}
		} else {
			Painter.setPen(DrawyColor);
			Painter.drawLine(RealStartPos - Shifting, 
				RealEndPos - Shifting);
		}
	}
	this->CurBeginDrawPosition = this->CurEndDrawPosition;
	/* 
	 * When MainWindow is updated, paintEvent() is called and the image 
	 * is drawn.
	 */
	this->update();
	Evt->accept();
}

/* The user is no longer drawing. */
void Canvas::mouseReleaseEvent(QMouseEvent *Evt)
{
	this->IsBrushEnabled = false;
	this->CurEndDrawPosition = Evt->pos();
	
	Evt->accept();
	std::cout << "GETTING CALLED!" << std::endl;
	QPixmap RealCurrent = QPixmap::fromImage(*this->CurCanvas);
	emit LayerChanged(RealCurrent,
		static_cast<uint32_t>(this->LayerIndex));
	
}

/* Wipes out the image on this canvas. */
void Canvas::Clear()
{
	*(this->CurCanvas) = QImage(128, 128,
		QImage::Format_ARGB32_Premultiplied
	);
	this->CurCanvas->fill(QColor(Qt::transparent));
	this->update();
}

/* Sets the color to draw to the screen with. */
void Canvas::SetBrushColor(QColor color)
{
	this->CurBrushColor = color;
}

/* The color to be used. */
QColor Canvas::GetBrushColor()
{
	return this->CurBrushColor;
}

/* Gets the width of the **backing buffer** for this canvas. */
uint32_t Canvas::GetContentWidth()
{
    return static_cast<uint32_t>(this->CurCanvas->width());
}


/* Gets the height of the **backing buffer** for this canvas. */
uint32_t Canvas::GetContentHeight()
{
    return static_cast<uint32_t>(this->CurCanvas->height());
}

int Canvas::GetLayerIndex()
{
	return this->LayerIndex;
}
