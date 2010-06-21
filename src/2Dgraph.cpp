/***************************************************************************
 *   Copyright (C) 2007 by marcos chaparro                                 *
 *   nitrousnrg@gmail.com                                                  *
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

 #include <QtGui>
 #include "2Dgraph.h"

//Water temp correction widget. Graphic control.
 graph2D::graph2D(QWidget *parent) : QWidget(parent)
 {
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	scribbling = false;
	//QPoint *punto = new QPoint[10];

	columns = 13;
	
	for(int i=0; i<21;++i)
	{
		punto1[i].setX(i*20+20);
		punto1[i].setY(180);
	}
 }

 QSize graph2D::minimumSizeHint() const
 {
     return QSize(260, 200);
 }

 QSize graph2D::sizeHint() const
 {
     return QSize(300, 200);
 }

 void graph2D::paintEvent(QPaintEvent * /* event */)
 {
	QColor BackColor(150, 150, 150);

	//QPen pen;
	QPainter painter(this);
	painter.setPen(BackColor);
	painter.scale((float)width() / (20*columns), height() / 200.0);

        QFont newFont("Times", 6, QFont::Normal);
	setFont(newFont);
	QString num;
	for(int i=0;i<(columns-1);++i)
	{
		if(i<9)
		{
			painter.setPen(pen);
			num.setNum(scaleY[i+1]);
			painter.drawText(0, 20*(i+1)+3, num);
			painter.setPen(BackColor);
			painter.drawLine(20,20*i,20*columns,20*i);
		}
		painter.setPen(pen);
		num.setNum(scaleX[i]);
		painter.drawText(20*(i+1)-4,190,num);
		painter.setPen(BackColor);


		painter.drawLine(20*i+20,0,20*i+20,180);
	}
	painter.drawLine(20*columns-0.5,0,20*columns-0.5,180);
	painter.drawLine(20,180,20*columns,180);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(pen);
	painter.drawPolyline(punto1,columns);
 }

bool graph2D::setPoint(int x, int y)	//x goes from 0 to 180
{					//y goes from 0 to 180
	if(x>260 )
		return false;
	punto1[x/20].setX(x+20);
	punto1[x/20].setY(180-y);
	return true;
}

int graph2D::getPoint(int index)
{
	return 180 - punto1[index].y();
}

bool graph2D::setScaleX(int index,float number)
{
	if(!(index < columns && index >= 0))
		return false;
	else
	{
		scaleX[index] = number;
		return true;
	}
}

bool graph2D::setScaleY(int index,float number)
{
	if(!(index < columns && index >= 0))
		return false;
	else
	{
		scaleY[9-index] = number;
		return true;
	}
}


void graph2D::setColumns(int col)
{
	columns = col;
}


void graph2D::mousePressEvent(QMouseEvent *event)
 {
    if (event->button() == Qt::LeftButton)
         scribbling = true;
 }

 void graph2D::mouseMoveEvent(QMouseEvent *event)
 {
	if ((event->buttons() & Qt::LeftButton) && scribbling)
	{
		if(event->x() >= 10 && event->y() < height())
		{
			int index = (event->x()-10)*columns/width();
                        
                        if(index>20)    //I don't have more than 20 columns
                            return;
                        punto1[index].setX(index*20 + 20);
			if(event->y()*200/height() < 180)
				punto1[index].setY(event->y()*200/height());
			else
				punto1[index].setY(180);

			emit valueChanged(index);
			update();
		}
	}
 }
void graph2D::mouseReleaseEvent(QMouseEvent *event)
 {
	if (event->button() == Qt::LeftButton && scribbling)
	{
		scribbling = false;
	}
 }
