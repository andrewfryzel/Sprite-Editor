#include "brushbutton.h"
#include <QPushButton>

#include <pluginmanager.h>
#include <stdlib.h>
#include <iostream>

/* 
 * Creates a new BrushButton, which holds the state of the brush
 * as well as other details for which this brush is useful for.
 */
BrushButton::BrushButton(QWidget *Parent) : QPushButton(Parent)
{
	this->AttachedBrush = new Brush();
	this->AttachedBrush->Map = static_cast<Strength*>(malloc(4096));
	memset(this->AttachedBrush->Map, 0, 4096);
	
	this->connect(this, &BrushButton::clicked,
		this, &BrushButton::InternalProxyClickedSignal);
}

/* Makes an attempt to clean up the attached brush if the plugin didn't. */
BrushButton::~BrushButton()
{
	free(this->AttachedBrush->Map);
	delete this->AttachedBrush;
}

void BrushButton::RequestResize(float Scaling)
{
	BrushResizeObject BrRzObj;
	BrRzObj.NewSize = Scaling;
	BrRzObj.CurBrush = this->AttachedBrush;
	emit this->ResizeRequest(this->text().toStdString(),
		PLUGIN_DISPATCH_REASON_BRUSH_DESELECTED, 
		&BrRzObj
	);
}

/* Proxies the pressing signal to the callback mechanism for the plugins. */
void BrushButton::InternalProxyClickedSignal(bool Input)
{
	std::cout << " BRUSH FIRED! " << std::endl;
	if (Input)
	{
		emit this->Pressed(this->text().toStdString(),
			PLUGIN_DISPATCH_REASON_BRUSH_SELECTED, 
			this->AttachedBrush
		);
	} else {
		emit this->Pressed(this->text().toStdString(),
			PLUGIN_DISPATCH_REASON_BRUSH_DESELECTED, 
			this->AttachedBrush
		);
	}
}
