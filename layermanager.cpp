#include "layermanager.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <iostream>

LayerManager::LayerManager(QWidget *parent) : QWidget(parent)
{
	this->setLayout(new QVBoxLayout);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->HoldingArea = new QScrollArea(this);
	this->HoldingArea->setLayout(new QHBoxLayout());
	QFrame *InternalHoldingCell = new QFrame(this);
	InternalHoldingCell->setLayout(new QVBoxLayout());
	InternalHoldingCell->setSizePolicy(
		QSizePolicy::Expanding, QSizePolicy::Expanding
	);
	this->HoldingArea->setWidget(InternalHoldingCell);
	this->HoldingArea->setWidgetResizable(true);
	this->layout()->addWidget(this->HoldingArea);
	this->CurrIndex = 0;
}


LayerManager::~LayerManager()
{
	
}

QVector<QPixmap> LayerManager::GetLayers()
{
	return this->Map;
}

/* Updates the layers that's being set on, so that it's now something else. */
void LayerManager::SetLayers(QVector<QPixmap> NewLayers)
{
	/* Set the current index to 0 to be safe. */
	this->CurrIndex = 0;
	/* Remove what's already there. */
	while (QWidget *Item 
		= this->HoldingArea->widget()->findChild<QWidget*>()
		)
	{
		delete Item;
	}
	int Width = NewLayers.at(0).width();
	int Height = NewLayers.at(0).height();
	this->Map.clear();
	for (QPixmap Map : NewLayers)
	{
		this->InsertWithPixmap(Map);
	}
	/* And then claim we changed layers. */
	emit this->InternalDemandRepaint();
	emit this->SelectLayer(0);
}

void LayerManager::InsertWithPixmap(QPixmap Map)
{
	Layer *NewLayer = new Layer();
	this->Map.append(Map);
	NewLayer->SetCurPixmap(Map);
	NewLayer->SetIndex(this->Map.length() - 1);
	NewLayer->connect(NewLayer, &Layer::LayerPressed,
		this, 
		[this, NewLayer](int Index)
			{
				NewLayer->SetCurPixmap(this->Map.at(Index));
				this->SelectLayer(Index);
			}
		);
	NewLayer->connect(this, &LayerManager::InternalDemandRepaint,
		NewLayer, 
		[this, NewLayer]()
			{
				int CurIndex = NewLayer->GetIndex();
				if (this->Map.length() > CurIndex)
				{
					NewLayer->SetCurPixmap(
						this->Map.at(
							NewLayer->GetIndex()
						)
					);
				} else {
					QPixmap Blank;
					Blank.fill(Qt::transparent);
					NewLayer->SetCurPixmap(Blank);					
				}
				/* Force a redraw. */
				NewLayer->repaint();
			}
		);
	this->HoldingArea->widget()->layout()->addWidget(NewLayer);
}

void LayerManager::RequestNewLayer(int Width, int Height)
{
	QPixmap TempMap(Width, Height);
	TempMap.fill(Qt::transparent);
	Layer *NewLayer = new Layer();
	this->Map.append(TempMap);
	NewLayer->connect(NewLayer, &Layer::LayerPressed,
		this, 
		[this, NewLayer](int Index)
			{
				NewLayer->SetCurPixmap(this->Map.at(Index));
				this->SelectLayer(Index);
			}
		);
	this->SelectLayer(this->Map.length() - 1);
	NewLayer->SetIndex(this->Map.length() - 1);
	NewLayer->connect(this, &LayerManager::InternalDemandRepaint,
		NewLayer, 
		[this, NewLayer]()
			{
				int CurIndex = NewLayer->GetIndex();
				if (this->Map.length() > CurIndex)
				{
					NewLayer->SetCurPixmap(
						this->Map.at(
							NewLayer->GetIndex()
						)
					);
				} else {
					QPixmap Blank;
					Blank.fill(Qt::transparent);
					NewLayer->SetCurPixmap(Blank);					
				}
				/* Force a redraw. */
				NewLayer->repaint();
			}
		);
	this->HoldingArea->widget()->layout()->addWidget(NewLayer);
}

QPixmap LayerManager::GetCurrLayer() 
{
	return this->Map.at(this->CurrIndex);
}

QPixmap LayerManager::SelectLayer(int Index)
{
	QPixmap Curr = this->Map.at(Index);
	emit LayerChanged(Curr, Index);
	this->CurrIndex = Index;
	return Curr;
}


void LayerManager::RecieveLayer(QPixmap OldMap, int OldIndex)
{	
	this->Map.replace(OldIndex, OldMap);
	this->repaint();
	emit this->InternalDemandRepaint();
	emit this->SendingLayers(this->Map);
}