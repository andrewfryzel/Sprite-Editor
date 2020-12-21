/*
 *	widgets/colorwidget.cpp
 *	Implementation of recently used colors.
 *	The colors are backed by a cache which uses
 *	an implementation of the Least Recently Used
 *	cache algorithm to update colors quickly available.
 * 
 *	Author:
 *		Brian Schnepp (Nov 2, 2019)
 */

#include <widgets/colorwidget.h>

#include <QGridLayout>

#include <math.h>

/* Creates the ColorWidget. */
ColorWidget::ColorWidget(QWidget *Parent) : QWidget(Parent)
{
	this->MaxColors = 9;
	this->setLayout(new QGridLayout(this));
	this->layout()->setSpacing(0);
	this->layout()->setMargin(0);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->ColorGroup.setExclusive(true);
}

/* Empty destructor. */
ColorWidget::~ColorWidget()
{

}

/* Updates the maximum colors to use. */
void ColorWidget::SetMaximumColors(uint8_t NewMax)
{
	this->MaxColors = NewMax;
}

/* Proxies the call upwards. */
void ColorWidget::InternalReEmitColorPickedSignal(QColor Color)
{
	/* 
	 * "re-insert" it so that it's not going to get evicted from
	 * cache, when it would otherwise be it's turn.
	 */
	this->InsertColor(Color);
	emit this->ColorCachePicked(Color);
}

/* Replaces the current grouping of buttons with new ones.
 * This is done when a new color is added, so it's made available to click on.
 */
void ColorWidget::ResetButtons()
{
	int ColCount = static_cast<int>(sqrt(this->RecentColors.length()));
	int CurCol = 0;
	int CurRow = 0;
	/* Delete the old stuff. */
	for (QAbstractButton *Button : this->ColorGroup.buttons())
	{
		delete Button;
	}
	/* Re-insert them. */
	for (int Idx = 0; Idx < this->RecentColors.length(); ++Idx)
	{
		ColorButton *NewButton = new ColorButton(this);
		NewButton->SetColor(this->RecentColors.at(Idx).Color);
		/* 
		 * Need this event to cause the main window to eventually 
		 * be invoked. 
		 */
		NewButton->connect(NewButton, &ColorButton::ButtonClicked,
			this, &ColorWidget::InternalReEmitColorPickedSignal);
		
		this->ColorGroup.addButton(NewButton);
		QGridLayout *CurLayout = 
			static_cast<QGridLayout*>(this->layout()
		);
		/* Some nice formatting magic. Just makes things 
		 * kind of square-ish, instead of a column of colors. */
		if (CurRow == ColCount)
		{
			CurRow = 0;
			CurCol++;
		}
		CurLayout->addWidget(NewButton, CurRow++, CurCol);
	}
}

/* 
 * Adds a new color to the cache. If already filled,
 * the least recently used color is evicted from the cache, and
 * replaced with this new color.
 */
void ColorWidget::InsertColor(QColor Color)
{
	ColorEntry NewEntry = {0, Color};
	/* First check if it's already present. If so, great! We just quit. */
	for (int Idx = 0; Idx < this->RecentColors.length(); ++Idx)
	{
		if (this->RecentColors.at(Idx).Color == Color)
		{
			/* Reset time since last use. */
			this->RecentColors.replace(Idx, NewEntry);
			return;
		}
	}
	if (this->RecentColors.length() == this->MaxColors)
	{
		/* Least Recently Used. */
		uint8_t IndexGreatest = 0;
		for (int Idx = 0; Idx < this->RecentColors.length(); ++Idx)
		{
			ColorEntry Entry = this->RecentColors.at(Idx);
			if (Entry.Timeout > IndexGreatest)
			{
				IndexGreatest = static_cast<uint8_t>(Idx);
			}
			Entry.Timeout++;
			this->RecentColors.replace(Idx, Entry);
		}
		
		/* Evict the least recently used one. */
		this->RecentColors.replace(IndexGreatest, NewEntry);
	} else {
		/* Populate the cache normally. */	
		this->RecentColors.append(NewEntry);
	}
	/* Re-render and update. */
	this->ResetButtons();
}