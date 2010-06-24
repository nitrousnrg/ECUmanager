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

#ifndef QPLAYER_H
#define QPLAYER_H

#include <QWidget>
#include <QSlider>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QFile>
#include <QDesktopWidget>

class QPlayer : public QWidget
{
	Q_OBJECT
		public:
		QPlayer(QTimer*, QTimer*, QFile*);
		void setFileTimer(QTimer*);
		void setPosition(int);
		int pos();
		bool isBusy();
		QSize minimumSizeHint() const;
		QSize sizeHint() const;
	private:
		int xSize,ySize;
		QTimer *playTimer;
		QTimer *refreshTimer;
		QFile *file;
		QSlider *slider;
		bool busy;

	private slots:
		void play();
		void pause();
		void sliderPress();
		void seekFile();

};
#endif
