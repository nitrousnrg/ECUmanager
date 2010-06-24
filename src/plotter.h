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

#ifndef PLOTAREA_H
#define PLOTAREA_H

#include <QtGui>

class QBrush;
class QPen;
class QWidget;
class QMenu;
class QCheckBox;

enum dataStyle {HistogramStyle,PlotStyle};

class PlotArea : public QWidget
{
	Q_OBJECT

		public:
		PlotArea(QWidget *parent = 0);

		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		void clear();
		void desplazar(float newValue);
		int getChannel();
		QColor getChannelColor(int channel);
		QColor getBackgroundColor();
		QColor getFontColor();
		void setBackColor(QColor);
		void setFontColor(QColor);
		void setChannel1Color(QColor);
		void setPoints(float newValue,int size);
		void setStyle(int);
		void setRows(int);
		void setColumns(int);
		void setScaleFactorX(float,bool reversed);
		void setScaleFactorY(float);
		void setSize(int,int);
		void readSettings();
		void writeSettings();
		enum channels {lambda,RPM,VE,MAP,airTemp,waterTemp,ignAdv,injAdv,duty,injTime,throttle};

	protected:
		void paintEvent(QPaintEvent *event);
		void mouseReleaseEvent(QMouseEvent*);

	private slots:
		void chooseDialog();
		void setBackColorSlot();
		void setFontColorSlot();
		void setChannel1ColorSlot();
		void acceptDialog();
		void rejectDialog();

	private:
		bool reversed;
		int channel;
		int columns;
		int rows;
		int xSize;
		int ySize;
		int myWidth;
		int style;
		int pointsReceived;
		float scaleFactorX;
		float scaleFactorY;
		float scaleY[20];
		QPoint *points;
		QPen pen;
		QBrush brush;
		QString num;
		QCheckBox *LambdaBox;
		QCheckBox *RPMBox;
		QCheckBox *VEBox;
		QCheckBox *MAPBox;
		QCheckBox *airTempBox;
		QCheckBox *waterTempBox;
		QCheckBox *ignAdvanceBox;
		QCheckBox *fuelAdvBox;
		QCheckBox *dutyBox;
		QCheckBox *injTimeBox;
		QCheckBox *throttleBox;
		QMenu *menu;
		QMenu *colors;
		QPalette palette;
		QAction *chooseAct;
		QAction *selectChannelAct;
		QAction *backColorAct;
		QAction *fontColorAct;
		QAction *chan1ColorAct;
		QAction *chan2ColorAct;
		QAction *chan3ColorAct;
		QDialog *dialog;

		QColor BackColor;
		QColor penColor;
		QColor channel1Color;
		QBrush backBrush;
		//static const QPoint;
};
#endif
