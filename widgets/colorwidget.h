#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QQueue>
#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>

#include <widgets/colorbutton.h>

/* 
 * This is needed for lazy implementation of Least Recently Used cache.
 */
typedef struct ColorEntry
{
	uint8_t Timeout;	/* Remove this entry if it's the greatest. */
	QColor Color;		/* The color that belongs in this slot. */
}ColorEntry;

class ColorWidget : public QWidget
{

Q_OBJECT

public:
	ColorWidget(QWidget *Parent = nullptr);
	~ColorWidget();
	
public slots:
	void SetMaximumColors(uint8_t NewMax);
	void InsertColor(QColor Color);
	
signals:
	void ColorCachePicked(QColor NewColor);
	
private:
	void ResetButtons();
	QButtonGroup ColorGroup;
		
	uint8_t MaxColors;
	QVector<ColorEntry> RecentColors;

private slots:
	void InternalReEmitColorPickedSignal(QColor Color);
};

#endif