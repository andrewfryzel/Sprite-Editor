#ifndef BRUSHBUTTON_H
#define BRUSHBUTTON_H

#include <tools.h>

#include <QPushButton>

#include <pluginsdk/pluginsdk.h>

class BrushButton : public QPushButton
{
Q_OBJECT
public:
	BrushButton(QWidget *Parent = nullptr);
	
	virtual ~BrushButton();
	
signals:
	void Pressed(std::string Name, PluginDispatchReason Input, 
		Brush *AttachedBrush
	);
	
	void ResizeRequest(std::string Name, PluginDispatchReason Input, 
		BrushResizeObject *AttachedBrRzObj
	);

public slots:
	void RequestResize(float Scaling);

private slots:
	void InternalProxyClickedSignal(bool Input);
	
private:
	Brush *AttachedBrush;
};

#endif