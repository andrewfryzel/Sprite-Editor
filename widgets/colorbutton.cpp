#include "colorbutton.h"

#include <QPainter>
#include <QSizePolicy>

#include <QPaintEvent>

/* Creates a new color button, where the user can click and select a color. */
ColorButton::ColorButton(QWidget *parent) : QPushButton(parent)
{
	this->connect(this, &ColorButton::pressed, 
		this, &ColorButton::InternalProxyButtonPressed);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/* Sets the color to use. */
void ColorButton::SetColor(QColor Color)
{
	this->Color = Color;
}

/* Shows this button, but clearly only the color it has. */
void ColorButton::paintEvent(QPaintEvent *Event)
{
	QPainter CurPainter(this);
	CurPainter.fillRect(this->rect(), QBrush(this->Color));
	Event->accept();
}

/* A proxy slot to re-emit to the parent with the color. */
void ColorButton::InternalProxyButtonPressed()
{
	emit this->ButtonClicked(this->Color);
}
