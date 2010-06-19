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

#ifndef QSUBVIEWER_H
#define QSUBVIEWER_H

#include "plotter.h"
#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>



class QSubViewer : public QWidget
{
Q_OBJECT
public:
	QSubViewer(QWidget *parent = 0);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void clear();
	void setBackColor(QColor color);
	void setFontColor(QColor color);
	void setChannel1Color(QColor color);
	void setPoints(float,int size);
	int getChannel();
	QColor getBackgroundColor();
	QColor getFontColor();
	QColor getChannelColor(int);
	void desplazar(float);
	void setChannel(int);

signals:
	void valueChanged(int);
private:
	PlotArea *plot;
	QGridLayout *grid;
	QComboBox *comboY;
	QComboBox *comboY2;
	QComboBox *comboY3;
	QComboBox *comboX;

};

#endif
