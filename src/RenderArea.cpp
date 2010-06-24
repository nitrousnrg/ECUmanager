/****************************************************************************
 **
 ** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://www.trolltech.com/products/qt/opensource.html
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://www.trolltech.com/products/qt/licensing.html or contact the
 ** sales department at sales@trolltech.com.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **

   Modified by Marcos Chaparro jojo
****************************************************************************/

#include <QtGui>
#include <math.h>
#include "renderarea.h"
#define pi_to_rad 0.017453292

RenderArea::RenderArea(QWidget *parent,int limit, int redLine, int initialAngle, int finalAngle)
: QWidget(parent)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(false);
	Limit = limit/1000;
	RedLine = redLine/1000;
	InitialAngle = initialAngle;
	FinalAngle = finalAngle;

	needleColor = QColor(0, 127, 127, 191);
	numbersColor = QColor(0, 127, 127, 191);
	scale1Color= QColor(127, 0, 127);
	scale2Color= QColor(0, 127, 127, 191);
	hourColor = QColor(127, 0, 127);
	redPen = QColor(255, 40, 40);

	setLimitAct = new QAction(tr("Limit"), this);
	connect(setLimitAct, SIGNAL(triggered()), this, SLOT(setLimitSlot()));

	setRedLineAct = new QAction(tr("Red Line"), this);
	connect(setRedLineAct, SIGNAL(triggered()), this, SLOT(setRedLineSlot()));

	setInitialAngleAct = new QAction(tr("Initial Angle"), this);
	connect(setInitialAngleAct, SIGNAL(triggered()), this, SLOT(setInitialAngleSlot()));

	setFinalAngleAct = new QAction(tr("Final Angle"), this);
	connect(setFinalAngleAct, SIGNAL(triggered()), this, SLOT(setFinalAngleSlot()));

	setNeedleColorAct = new QAction(tr("Needle Color"), this);
	connect(setNeedleColorAct, SIGNAL(triggered()), this, SLOT(setNeedleColorSlot()));

	setNumbersColorAct = new QAction(tr("Numbers Color"), this);
	connect(setNumbersColorAct, SIGNAL(triggered()), this, SLOT(setNumbersColorSlot()));

	setScale1ColorAct = new QAction(tr("Scale 1 Color"), this);
	connect(setScale1ColorAct, SIGNAL(triggered()), this, SLOT(setScale1ColorSlot()));

	setScale2ColorAct = new QAction(tr("Scale 2 Color"), this);
	connect(setScale2ColorAct, SIGNAL(triggered()), this, SLOT(setScale2ColorSlot()));

	menu = new QMenu;
	changeNumbersMenu = menu->addMenu("Settings");
	changeColorsMenu = menu->addMenu("Colors");
	changeNumbersMenu->addAction(setLimitAct);
	changeNumbersMenu->addAction(setRedLineAct);
	changeNumbersMenu->addAction(setInitialAngleAct);
	changeNumbersMenu->addAction(setFinalAngleAct);

	changeColorsMenu->addAction(setNeedleColorAct);
	changeColorsMenu->addAction(setNumbersColorAct);
	changeColorsMenu->addAction(setScale1ColorAct);
	changeColorsMenu->addAction(setScale2ColorAct);

}


QSize RenderArea::minimumSizeHint() const
{
	return QSize(100, 100);
}


QSize RenderArea::sizeHint() const
{
	return QSize(300, 200);
}


void RenderArea::setValue(float value)
{
	actualPosition = value;
	update();
}


void RenderArea::setLimitSlot()
{
	bool ok;
	int newLimit = QInputDialog::getInteger(this, tr("RPM Limit"),
		tr("Enter RPM:"), Limit*1000, 0, 20000, 1000, &ok);
	if(ok)
		setLimit(newLimit);
}


void RenderArea::setLimit(int newLimit)
{
	Limit = newLimit/1000;
	update();
}


void RenderArea::setRedLineSlot()
{
	bool ok;
	int newLimit = QInputDialog::getInteger(this, tr("RPM Limit"),
		tr("Enter RPM:"), RedLine*1000, 0, 20000, 1000, &ok);
	if(ok)
		RedLine = newLimit/1000;
	update();
}


void RenderArea::setInitialAngleSlot()
{
	bool ok;
	int newAngle = QInputDialog::getInteger(this, tr("Initial Angle"),
		tr("Enter Angle:"), InitialAngle, 0, FinalAngle, 1, &ok);
	if(ok)
		InitialAngle = newAngle;
	update();
}


void RenderArea::setFinalAngleSlot()
{
	bool ok;
	int newAngle = QInputDialog::getInteger(this, tr("Final Angle"),
		tr("Enter Angle:"), FinalAngle, InitialAngle, 360, 1, &ok);
	if(ok)
		FinalAngle = newAngle;
	update();
}


void RenderArea::setNeedleColorSlot()
{
	QColor color = QColorDialog::getColor(needleColor,this);
	if (color.isValid())
	{
		needleColor = color;
	}
	update();
}


void RenderArea::setNumbersColorSlot()
{
	QColor color = QColorDialog::getColor(numbersColor, this);
	if (color.isValid())
	{
		numbersColor = color;
	}
	update();
}


void RenderArea::setScale1ColorSlot()
{
	QColor color = QColorDialog::getColor(scale1Color, this);
	if (color.isValid())
	{
		scale1Color = color;
	}
	update();
}


void RenderArea::setScale2ColorSlot()
{
	QColor color = QColorDialog::getColor(scale2Color, this);
	if (color.isValid())
	{
		scale2Color = color;
	}
	update();
}


void RenderArea::paintEvent(QPaintEvent * /* event */)
{
	static const QPoint Needle[3] =
	{
		QPoint(7, 8),
		QPoint(-7, 8),
		QPoint(0, -85)
	};

	QFont newFont = font();
	newFont.setPixelSize(15);
	setFont(newFont);
	QFontMetrics fontMetrics(newFont);
	QRect xBoundingRect = fontMetrics.boundingRect(tr("x"));

	int side = qMin(width(), height());
	//     QTime time = QTime::currentTime();

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(width() / 2, height() / 2);
	painter.scale(side / 200.0, side / 200.0);

	//	painter.setPen(numbersColor);

	double step = (FinalAngle-InitialAngle)/Limit;

	painter.save();
	double angle=InitialAngle+90;

	QString num;
	QColor black(56,56,90);
	painter.setPen(black);
	painter.drawText(   30 + xBoundingRect.width() / 2,
		40 + xBoundingRect.height() / 2,
								 //poner scalefactor!
		num.setNum((int)(actualPosition*1000),10));

	painter.setPen(numbersColor);

	for (int j = 0 ; j <= (Limit); ++j)
	{
		painter.drawText( 74*cos(angle*pi_to_rad) - 10 + xBoundingRect.width() / 2,
			74*sin(angle*pi_to_rad) - 4 + xBoundingRect.height() / 2, num.setNum(j,10));
		angle += step;
	}

	painter.rotate(InitialAngle);
	painter.setPen(scale1Color);

	for (int i = 0; i < Limit+1; ++i)
	{
		if( i == RedLine)
			painter.setPen(redPen);
		painter.drawLine(0, 88, 0, 96);
		painter.rotate(step);
	}
	painter.restore();

	painter.setPen(Qt::NoPen);
	painter.setBrush(needleColor);

	painter.save();
	painter.rotate(actualPosition*step+180+InitialAngle);
	painter.drawConvexPolygon(Needle, 3);
	painter.restore();

	painter.setPen(scale2Color);
	painter.rotate(InitialAngle);

	for (int j = 0; j <= (Limit)*5; ++j)
	{
		if( j == RedLine*5)
			painter.setPen(redPen);

		if ((j % 5) != 0)
			painter.drawLine(0, 92, 0, 96);
		painter.rotate(step/5);
	}
}


void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
		menu->popup(QPoint(event->globalX(),event->globalY()), setLimitAct);
}
