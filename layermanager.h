#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QWidget>
#include <QScrollArea>

#include <layer.h>


class LayerManager : public QWidget
{
Q_OBJECT

public:
	explicit LayerManager(QWidget *Parent = nullptr);
	~LayerManager() override;
	
	QVector<QPixmap> GetLayers();
	void SetLayers(QVector<QPixmap> NewLayers);
	
	void InsertWithPixmap(QPixmap Map);
	QPixmap GetCurrLayer();

signals:
	void LayerChanged(QPixmap Map, int Index);
	
	void InternalDemandRepaint();
	void SendingLayers(QVector<QPixmap> Items);

public slots:
	void RequestNewLayer(int Width, int Height);
	QPixmap SelectLayer(int Index);
	void RecieveLayer(QPixmap NewMap, int NewIndex);

private:
	QScrollArea *HoldingArea;
	QVector<QPixmap> Map;
	int CurrIndex;

};

#endif
