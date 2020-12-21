#ifndef LAYER_H
#define LAYER_H

#include <QAbstractButton>

class Layer : public QAbstractButton
{
Q_OBJECT

public:
	Layer(QWidget* parent = nullptr);
	~Layer() override;
	
	QPixmap GetPixmap();
	void SetIndex(int NewIndex);
	int GetIndex();
	
	/* Replace the painting event Qt uses. */
	void paintEvent(QPaintEvent *Event) override;
	
signals:
	void LayerPressed(int Layer);

public slots:
	void SetCurPixmap(QPixmap NewPixmap);
	
private:
	int CurIndex;
	QPixmap Pixmap;
};

#endif
