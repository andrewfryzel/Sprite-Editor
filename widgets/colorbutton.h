#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

class ColorButton : public QPushButton
{
	Q_OBJECT
public:
	explicit ColorButton(QWidget *parent = nullptr);
	
	void SetColor(QColor Color);
	
	/* Replace the painting event Qt uses. */
	void paintEvent(QPaintEvent *Event) override;

signals:
	void ButtonClicked(QColor Color);

private:
	QColor Color;
	
private slots:
	void InternalProxyButtonPressed();
};

#endif