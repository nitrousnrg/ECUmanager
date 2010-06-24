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

#ifndef DGRAPH_H
#define DGRAPH_H

#include <QBrush>
#include <QPen>
#include <QWidget>

class QWidget;
class QPen;
class QWidget;

class graph2D : public QWidget
{
	Q_OBJECT

		public:

		graph2D(QWidget *parent = 0);

		bool setPoint(int,int);
		int getPoint(int index);
		bool setScaleX(int index,float number);
		bool setScaleY(int index,float number);
		void setColumns(int);
		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		//	void desplazar(float newValue);

		signals:
		void valueChanged(int);
	protected:
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		//private slots:

	private:
		int columns;
		QPen pen;
		QBrush brush;
		QPoint punto1[21];
		bool scribbling;
		float scaleX[21];
		float scaleY[21];
		//QPoint punto[10];
		//static const QPoint;
};
#endif
