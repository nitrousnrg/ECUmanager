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
	const int base = height();	//out of range
	for(int i=0;i<1100;++i)//width()-15;++i)
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

	addAct = new QAction(tr("Add channel"), this);
	connect(addAct, SIGNAL(triggered()), this, SLOT(chooseDialog()));

	removeAct = new QAction(tr("Remove channel"), this);
//	connect(addAct, SIGNAL(triggered()), this, SLOT(chooseDialog()));

	menu->addAction(addAct);
	menu->addAction(removeAct);
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
{	newValue = height() - 15 - newValue;
	for(int i=0;i<(myWidth-25);++i)
	{
		(points+i)->setY(points[i+1].y());
	}
	(points+(myWidth-25))->setY((int)newValue);
	update();//20,0,300,146);
	return;
}

int PlotArea::getChannel()
{
	return channel;
}

void PlotArea::setBackColorSlot()
{	setBackColor(QColor());}	//puts an invalid color

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
{	setFontColor(QColor());	}	//invalid

void PlotArea::clear()
{
	const int base = height();	//out of range
	for(int i=0;i<1100;++i)//width()-15;++i)
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
{	return palette.color(backgroundRole());	}

QColor PlotArea::getFontColor()
{	return penColor;	}

QColor PlotArea::getChannelColor(int atChannel)
{
    atChannel=1;
    return channel1Color;	}

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
			update();//20,0,300,146);
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

void PlotArea::chooseDialog()
{

	dialog = new QDialog();
	dialog->setModal(true);
	dialog->setSizeGripEnabled(false);
	dialog->resize(300,360);
	dialog->setWindowTitle(tr("Select source channel"));

	QGridLayout *grid = new QGridLayout;

	LambdaRadio	= new QRadioButton(tr("Lambda"));
	RPMradio	= new QRadioButton(tr("RPM"));
	VEradio		= new QRadioButton(tr("VE"));
	MAPradio	= new QRadioButton(tr("MAP"));
	airTempRadio	= new QRadioButton(tr("Air Temp"));
	waterTempRadio	= new QRadioButton(tr("Water Temp"));
	ignAdvanceRadio	= new QRadioButton(tr("Ignition Advance"));
	fuelAdvRadio	= new QRadioButton(tr("Fuel Advance"));
	dutyRadio	= new QRadioButton(tr("Duty %"));
	injTimeRadio	= new QRadioButton(tr("Inj Time"));
	throttleRadio	= new QRadioButton(tr("Throttle pos"));

	switch(channel)
	{
		case 0:
			LambdaRadio->setChecked(true);
			break;
		case 1:
			RPMradio->setChecked(true);
			break;
		case 2:
			VEradio->setChecked(true);
			break;
		case 3:
			MAPradio->setChecked(true);
			break;
		case 4:
			airTempRadio->setChecked(true);
			break;
		case 5:
			waterTempRadio->setChecked(true);
			break;
		case 6:
			ignAdvanceRadio->setChecked(true);
			break;
		case 7:
			fuelAdvRadio->setChecked(true);
			break;
		case 8:
			dutyRadio->setChecked(true);
			break;
		case 9:
			injTimeRadio->setChecked(true);
			break;
		case 10:
			throttleRadio->setChecked(true);
			break;

	}

	QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
	connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));


	QGroupBox *groupBox1 = new QGroupBox(tr("Channels:"));
	QGridLayout *vbox = new QGridLayout;

	vbox->addWidget(LambdaRadio,1,0);
	vbox->addWidget(RPMradio,2,0);
	vbox->addWidget(VEradio,3,0);
	vbox->addWidget(MAPradio,4,0);
	vbox->addWidget(airTempRadio,5,0);
	vbox->addWidget(waterTempRadio,6,0);
	vbox->addWidget(ignAdvanceRadio,7,0);
	vbox->addWidget(fuelAdvRadio,8,0);
	vbox->addWidget(dutyRadio,9,0);
	vbox->addWidget(injTimeRadio,10,0);
	vbox->addWidget(throttleRadio,11,0);
	groupBox1->setLayout(vbox);

	grid->addWidget(groupBox1,1,0,1,2);
	grid->addWidget(accept,3,1);

	dialog->setLayout(grid);
	dialog->show();

}

void PlotArea::acceptDialog()
{
	if( LambdaRadio->isChecked() )		channel = 0;
	if( RPMradio->isChecked() )		channel = 1;
	if( VEradio->isChecked() )		channel = 2;
	if( MAPradio->isChecked() )		channel = 3;
	if( airTempRadio->isChecked() )		channel = 4;
	if( waterTempRadio->isChecked() )	channel = 5;
	if( ignAdvanceRadio->isChecked() )	channel = 6;
	if( fuelAdvRadio->isChecked() )		channel = 7;
	if( dutyRadio->isChecked() )		channel = 8;
	if( injTimeRadio->isChecked() )		channel = 9;
	if( throttleRadio->isChecked() )	channel = 10;

	dialog->accept();
	clear();
}

void PlotArea::rejectDialog()
{
	dialog->reject();
}
