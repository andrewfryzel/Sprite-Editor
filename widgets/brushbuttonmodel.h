#include <QObject>

#include <tools.h>
#include <widgets/brushbutton.h>

#ifndef BRUSHBUTTONMODEL_H
#define BRUSHBUTTONMODEL_H


class BrushButtonModel : public QObject
{

Q_OBJECT

public:
	BrushButtonModel(QWidget *Parent, Brush *brush);
};

#endif // BRUSHBUTTONMODEL_H