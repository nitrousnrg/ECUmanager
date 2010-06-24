/***************************************************************************
 *   Copyright (C) 2007 by marcos chaparro   *
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

#include "plotter.h"

PlotArea::PlotArea(QWidget *parent)     : QWidget(parent)
{

	LambdaBox  = new QCheckBox(tr("Lambda"));
	RPMBox          = new QCheckBox(tr("RPM"));
	VEBox      = new QCheckBox(tr("VE"));
	MAPBox          = new QCheckBox(tr("MAP"));
	airTempBox = new QCheckBox(tr("Air Temp"));
	waterTempBox   = new QCheckBox(tr("Water Temp"));
	ignAdvanceBox  = new QCheckBox(tr("Ignition Advance"));
	fuelAdvBox = new QCheckBox(tr("Fuel Advance"));
	dutyBox    = new QCheckBox(tr("Duty %"));
	injTimeBox = new QCheckBox(tr("Inj Time"));
	throttleBox    = new QCheckBox(tr("Throttle pos"));

	readSettings();
	setBackgroundRole(QPalette::Base);
	setMinimumSize(100,100);
	setAutoFillBackground(true);
	BackColor.setRgb(255, 255, 255);
	penColor.setRgb(150, 150, 150);
	channel1Color.setRgb(0, 0, 0);
	//backBrush.setColor(BackColor);

	palette.setColor(backgroundRole(), BackColor);
	setPalette(palette);

	points = new QPoint[1100];
	const int base = height();	 //out of range
	for(int i=0;i<1100;++i)		 //width()-15;++i)
	{
		(points+i)->setX(i+25);
		(points+i)->setY(base);
	}
	channel = 3;
	reversed = true;
	scaleFactorX = 1;
	scaleFactorY = 1;
	rows = 9;
	columns = 15;
	xSize=300;
	ySize=200;
	pointsReceived = 0;

	for(int i=0;i<rows;++i)
	{
		scaleY[i] = (i) * scaleFactorY;
	}

	menu = new QMenu;

	selectChannelAct = new QAction(tr("Select channels"), this);
	connect(selectChannelAct, SIGNAL(triggered()), this, SLOT(chooseDialog()));

	menu->addAction(selectChannelAct);
	colors = menu->addMenu("Colors");
	backColorAct = colors->addAction(tr("Background"));
	fontColorAct = colors->addAction(tr("Font Color"));
	chan1ColorAct = colors->addAction(tr("Channel 1"));
	chan2ColorAct = colors->addAction(tr("Channel 2"));
	chan3ColorAct = colors->addAction(tr("Channel 3"));

	connect(backColorAct, SIGNAL(triggered()), this, SLOT(setBackColorSlot()));
	connect(fontColorAct, SIGNAL(triggered()), this, SLOT(setFontColorSlot()));
	connect(chan1ColorAct, SIGNAL(triggered()), this, SLOT(setChannel1ColorSlot()));
	connect(chan2ColorAct, SIGNAL(triggered()), this, SLOT(setBackColorSlot()));
	connect(chan3ColorAct, SIGNAL(triggered()), this, SLOT(setBackColorSlot()));

}


QSize PlotArea::minimumSizeHint() const
{
	return QSize(180, 180);
}


QSize PlotArea::sizeHint() const
{
	return QSize(xSize, ySize);
}


void PlotArea::setSize(int x, int y)
{
	xSize = x;
	ySize = y;
}


void PlotArea::desplazar(float newValue)
{
	newValue = height() - 15 - newValue;
	for(int i=0;i<(myWidth-25);++i)
	{
		(points+i)->setY(points[i+1].y());
	}
	(points+(myWidth-25))->setY((int)newValue);
	update();					 //20,0,300,146);
	return;
}


int PlotArea::getChannel()
{
	return channel;
}


void PlotArea::setBackColorSlot()
{								 //puts an invalid color
	setBackColor(QColor());
}


void PlotArea::setBackColor(QColor color = NULL)
{
	if (color.isValid())
	{
		palette.setColor(backgroundRole(), color);
		setPalette(palette);
	}
	else
	{
		color = QColorDialog::getColor(Qt::white, this);
		if (color.isValid())
		{
			palette.setColor(backgroundRole(), color);
			setPalette(palette);
		}
	}

	update();

}


void PlotArea::setFontColorSlot()
{								 //invalid
	setFontColor(QColor());
}


void PlotArea::clear()
{
	const int base = height();	 //out of range
	for(int i=0;i<1100;++i)		 //width()-15;++i)
	{
		(points+i)->setX(i+25);
		(points+i)->setY(base);
	}
}


void PlotArea::setFontColor(QColor color)
{
	if (color.isValid())
		penColor = color;
	else
	{
		color = QColorDialog::getColor(penColor, this);
		if (color.isValid())
			penColor = color;
	}
	update();
}


void PlotArea::setChannel1ColorSlot()
{
	setChannel1Color(QColor());
}


void PlotArea::setChannel1Color(QColor color)
{
	if (color.isValid())
		channel1Color = color;
	else
	{
		color = QColorDialog::getColor(channel1Color, this);
		if (color.isValid())
			channel1Color = color;
	}
	update();
}


QColor PlotArea::getBackgroundColor()
{   return palette.color(backgroundRole()); }

QColor PlotArea::getFontColor()
{   return penColor;    }

QColor PlotArea::getChannelColor(int atChannel)
{
	atChannel=1;
	return channel1Color;
}


void PlotArea::setPoints(float newValue,int size)
{
	newValue = height() - 15 - newValue;

	int teToca = size/(myWidth-25);

	if(pointsReceived >= myWidth-25)
	{
		if( (pointsReceived % teToca) == 0)
		{
			for(int i=0;i<(myWidth-25);++i)
			{
				(points+i)->setY(points[i+1].y());
			}
			(points+(myWidth-25))->setY((int)newValue);
			update();			 //20,0,300,146);
		}
	}
	++pointsReceived;
	return;
}


void PlotArea::setStyle(int tipo)
{
	style = tipo;
}


void PlotArea::setRows(int ROW)
{
	if(ROW < 20)
		rows=ROW;
}


void PlotArea::setColumns(int COLUMN)
{
	columns = COLUMN;
}


void PlotArea::setScaleFactorX(float scale,bool reverse)
{
	scaleFactorX = scale;
	reversed = reverse;
}


void PlotArea::setScaleFactorY(float scale)
{
	scaleFactorY = scale;
	for(int i=0;i<rows;++i)
	{
		scaleY[i] = i * scaleFactorY;
	}

}


void PlotArea::paintEvent(QPaintEvent * /* event */)
{

	QPen pen;
	//	backBrush;
	QPainter painter(this);

	painter.setPen(penColor);
	QFont newFont = font();
	newFont.setPixelSize(9);
	painter.setFont(newFont);

	int step = (height()-15)/rows;
	int colStep = (width()-25)/columns;
	for(int i=25;i<width();i += colStep)
	{
		painter.drawLine(i,height()-15-step*rows,i,height()-15);
		if(reversed)
			num.setNum(( (width()-i-1)/colStep )*scaleFactorX);
		else
			num.setNum(i/colStep*scaleFactorX);
		painter.drawText(i-5, height()-4, num);
		myWidth=i;
	}
	int j=rows;
	for(int i=height()-15; i>=0 ;i -= step,--j)
	{
		num.setNum((float)scaleY[j],'f',1);
		painter.drawText(0, height()-15-i-3, num);

		painter.drawLine(25,i,myWidth,i);

	}

	// mark which channels it is plotting
	if( MAPBox->isChecked() )
		painter.drawText(width()-30, 14, QString(tr("MAP")));
	if( LambdaBox->isChecked() )
		painter.drawText(width()-30, 24, QString("Lambda"));
	if( RPMBox->isChecked() )
		painter.drawText(width()-30, 34, QString("RPM"));
	if( VEBox->isChecked() )
		painter.drawText(width()-30, 44, QString("VE"));
	if( airTempBox->isChecked() )
		painter.drawText(width()-30, 54, QString(tr("Air Temp")));
	if( waterTempBox->isChecked() )
		painter.drawText(width()-30, 64, QString(tr("Water Temp")));
	if( ignAdvanceBox->isChecked() )
		painter.drawText(width()-30, 74, QString(tr("Ign Adv")));
	if( fuelAdvBox->isChecked() )
		painter.drawText(width()-30, 84, QString(tr("Fuel Adv")));
	if( dutyBox->isChecked() )
		painter.drawText(width()-30, 84, QString(tr("Duty")));
	if( injTimeBox->isChecked() )
		painter.drawText(width()-30, 84, QString(tr("Inj Time")));
	if( throttleBox->isChecked() )
		painter.drawText(width()-30, 84, QString(tr("Throttle")));

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(channel1Color);

	painter.drawPolyline(points, (myWidth-25));
}


void PlotArea::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		menu->popup(QPoint(event->globalX(),event->globalY()), chooseAct);
	}
}


void PlotArea::readSettings()
{
	QSettings settings("FreeEMS", "Moving plot settings");
	VEBox->         setChecked(settings.value("Show VE",false).toBool());
	MAPBox->        setChecked(settings.value("Show MAP",true).toBool());
	airTempBox->    setChecked(settings.value("Show Air Temp",false).toBool());
	waterTempBox->  setChecked(settings.value("Show Water Temp",false).toBool());
	LambdaBox->     setChecked(settings.value("Show Air/fuel",false).toBool());
	RPMBox->        setChecked(settings.value("Show RPM",false).toBool());
	ignAdvanceBox-> setChecked(settings.value("Show Ign advance",false).toBool());
	fuelAdvBox->    setChecked(settings.value("Show Fuel advance",false).toBool());
	dutyBox->       setChecked(settings.value("Show Duty Cycle",false).toBool());
	injTimeBox->    setChecked(settings.value("Show Injection Time",false).toBool());
	throttleBox->   setChecked(settings.value("Show throttle",false).toBool());
}


void PlotArea::writeSettings()
{
	QSettings settings("FreeEMS", "Moving plot settings");
	settings.setValue("Show VE",            VEBox->isChecked());
	settings.setValue("Show MAP",           MAPBox->isChecked());
	settings.setValue("Show Air Temp",      airTempBox->isChecked());
	settings.setValue("Show Water Temp",    waterTempBox->isChecked());
	settings.setValue("Show Air/fuel",      LambdaBox->isChecked());
	settings.setValue("Show RPM",           RPMBox->isChecked());
	settings.setValue("Show Ign advance",   ignAdvanceBox->isChecked());
	settings.setValue("Show Fuel advance",  fuelAdvBox->isChecked());
	settings.setValue("Show Duty Cycle",    dutyBox->isChecked());
	settings.setValue("Show Injection Time",injTimeBox->isChecked());
	settings.setValue("Show throttle",      throttleBox->isChecked());
}


void PlotArea::chooseDialog()
{

	dialog = new QDialog();
	dialog->setModal(true);
	dialog->setSizeGripEnabled(false);
	dialog->resize(300,360);
	dialog->setWindowTitle(tr("Select source channel"));

	QGridLayout *grid = new QGridLayout;

	switch(channel)
	{
		case 0:
			LambdaBox->setChecked(true);
			break;
		case 1:
			RPMBox->setChecked(true);
			break;
		case 2:
			VEBox->setChecked(true);
			break;
		case 3:
			MAPBox->setChecked(true);
			break;
		case 4:
			airTempBox->setChecked(true);
			break;
		case 5:
			waterTempBox->setChecked(true);
			break;
		case 6:
			ignAdvanceBox->setChecked(true);
			break;
		case 7:
			fuelAdvBox->setChecked(true);
			break;
		case 8:
			dutyBox->setChecked(true);
			break;
		case 9:
			injTimeBox->setChecked(true);
			break;
		case 10:
			throttleBox->setChecked(true);
			break;

	}

	QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
	connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));

	QGroupBox *groupBox1 = new QGroupBox(tr("Channels:"));
	QGridLayout *vbox = new QGridLayout;

	vbox->addWidget(LambdaBox,1,0);
	vbox->addWidget(RPMBox,2,0);
	vbox->addWidget(VEBox,3,0);
	vbox->addWidget(MAPBox,4,0);
	vbox->addWidget(airTempBox,5,0);
	vbox->addWidget(waterTempBox,6,0);
	vbox->addWidget(ignAdvanceBox,7,0);
	vbox->addWidget(fuelAdvBox,8,0);
	vbox->addWidget(dutyBox,9,0);
	vbox->addWidget(injTimeBox,10,0);
	vbox->addWidget(throttleBox,11,0);
	groupBox1->setLayout(vbox);

	grid->addWidget(groupBox1,1,0,1,2);
	grid->addWidget(accept,3,1);

	dialog->setLayout(grid);
	dialog->show();

}


void PlotArea::acceptDialog()
{
	if( LambdaBox->isChecked() )        channel = 0;
	if( RPMBox->isChecked() )           channel = 1;
	if( VEBox->isChecked() )            channel = 2;
	if( MAPBox->isChecked() )           channel = 3;
	if( airTempBox->isChecked() )       channel = 4;
	if( waterTempBox->isChecked() )     channel = 5;
	if( ignAdvanceBox->isChecked() )    channel = 6;
	if( fuelAdvBox->isChecked() )       channel = 7;
	if( dutyBox->isChecked() )          channel = 8;
	if( injTimeBox->isChecked() )       channel = 9;
	if( throttleBox->isChecked() )      channel = 10;

	writeSettings();
	update();
	dialog->accept();
	clear();
}


void PlotArea::rejectDialog()
{
	dialog->reject();
}
