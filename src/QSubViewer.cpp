/***************************************************************************
 *   Copyright (C) 2007 by marcos chaparro                                 *
 *   nitrousnrg@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "QSubViewer.h"


QSubViewer::QSubViewer(QWidget *parent) : QWidget(parent)
{
//Display #1
	QGroupBox *groupBox = new QGroupBox(tr(""));
	QVBoxLayout *vLayout = new QVBoxLayout();

	QLabel *yAxis = new QLabel(tr("y axis:"));
	comboY = new QComboBox();
	comboY2 = new QComboBox();
	comboY3 = new QComboBox();

	QLabel *xAxis = new QLabel(tr("x axis:"));
	comboX = new QComboBox();

	comboY->addItem("Lambda");
	comboY->addItem("RPM");
	comboY->addItem("VE");
	comboY->addItem("MAP");
	comboY->addItem("Air Temp");
	comboY->addItem("WaterTemp");
	comboY->addItem("Ign Advance");
	comboY->addItem("Inj Advance");
	comboY->addItem("Duty %");
	comboY->addItem("Inj Time");
	comboY->addItem("Throttle pos");

	comboY2->addItem("Lambda");
	comboY2->addItem("RPM");
	comboY2->addItem("VE");
	comboY2->addItem("MAP");
	comboY2->addItem("Air Temp");
	comboY2->addItem("WaterTemp");
	comboY2->addItem("Ign Advance");
	comboY2->addItem("Inj Advance");
	comboY2->addItem("Duty %");
	comboY2->addItem("Inj Time");
	comboY2->addItem("Throttle pos");

	comboY3->addItem("Lambda");
	comboY3->addItem("RPM");
	comboY3->addItem("VE");
	comboY3->addItem("MAP");
	comboY3->addItem("Air Temp");
	comboY3->addItem("WaterTemp");
	comboY3->addItem("Ign Advance");
	comboY3->addItem("Inj Advance");
	comboY3->addItem("Duty %");
	comboY3->addItem("Inj Time");
	comboY3->addItem("Throttle pos");

	comboX->addItem("Time (sec)");
	comboX->addItem("Lambda");
	comboX->addItem("RPM");
	comboX->addItem("VE");
	comboX->addItem("MAP");
	comboX->addItem("Air Temp");
	comboX->addItem("WaterTemp");
	comboX->addItem("Ign Advance");
	comboX->addItem("Inj Advance");
	comboX->addItem("Duty %");
	comboX->addItem("Inj Time");
	comboY->addItem("Throttle pos");

	plot = new PlotArea();
	plot->setScaleFactorX(2,false);
	plot->setScaleFactorX(1.2,false);
	plot->setSize(1200,200);
	vLayout->addWidget(yAxis);
	vLayout->addWidget(comboY);
//	vLayout->addWidget(comboY2);
//	vLayout->addWidget(comboY3);
	vLayout->addStretch(40);
	vLayout->addWidget(xAxis);
	vLayout->addWidget(comboX);

	QGridLayout *grid = new QGridLayout();
	grid->addLayout(vLayout,0,0);
	grid->addWidget(plot,0,1);
	grid->setColumnMinimumWidth(1,440);
	grid->setColumnStretch(80,1);

	groupBox->setLayout(grid);
	
	QVBoxLayout *soyIgnorante = new QVBoxLayout();
	soyIgnorante->addWidget(groupBox);

	setLayout(soyIgnorante);

	connect(comboY, SIGNAL(currentIndexChanged(int)),this, SIGNAL(valueChanged(int)));
}

void QSubViewer::setPoints(float newPoint, int size)
{	plot->setPoints(newPoint,size);}

void QSubViewer::setChannel(int channel)
{	
	
	comboY->setCurrentIndex(channel);
}

void QSubViewer::clear()
{
	plot->clear();
}

int QSubViewer::getChannel()
{	return plot->getChannel();}

void QSubViewer::desplazar(float newPoint)
{	plot->desplazar(newPoint);}

void QSubViewer::setBackColor(QColor color)
{	plot->setBackColor(color);}

void QSubViewer::setFontColor(QColor color)
{	plot->setFontColor(color);}

void QSubViewer::setChannel1Color(QColor color)
{	plot->setChannel1Color(color);}

QColor QSubViewer::getBackgroundColor()
{	return plot->getBackgroundColor();}

QColor QSubViewer::getFontColor()
{	return plot->getFontColor();}

QColor QSubViewer::getChannelColor(int i)
{	return plot->getChannelColor(i);}

QSize QSubViewer::minimumSizeHint() const
{
        return QSize(100, 100);
}

QSize QSubViewer::sizeHint() const
{
        return QSize(150, 150);
}
