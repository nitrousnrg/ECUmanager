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
#include <QPlayer.h>

QPlayer::QPlayer(QTimer *timer1, QTimer *timer2, QFile *file1)
{
	playTimer = timer1;
	refreshTimer = timer2;

	file = file1;
	QDesktopWidget *desktop = new QDesktopWidget;

	setWindowTitle("QPlayer");
	setGeometry(desktop->width()-100, desktop->height()-60, 220, 25);

	xSize = 130;
	ySize = 30;
	busy = false;

	slider = new QSlider(Qt::Horizontal,this);
	QPushButton *playButton = new QPushButton(QIcon(":/icons/amarok_play.png"),"");
	QPushButton *pauseButton = new QPushButton(QIcon(":/icons/amarok_pause.png"),"");
	playButton->setFlat(true);
	pauseButton->setFlat(true);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(playButton);
	layout->addWidget(pauseButton);
	layout->addWidget(slider);

	connect(playButton, SIGNAL(released()), this, SLOT(play() ) );
	connect(pauseButton,SIGNAL(released()), this, SLOT(pause()) );
	connect(slider,SIGNAL(sliderReleased()), this, SLOT(seekFile()) );
	connect(slider,SIGNAL(sliderPressed()), this, SLOT(sliderPress()) );

	setLayout(layout);
	show();
}


								 //Recibe el timer que controla el flujo de datos del archivo
void QPlayer::setFileTimer(QTimer * timer)
{
	playTimer = timer;
}


void QPlayer::setPosition(int pos)
{
	slider->setValue(pos);
}


int QPlayer::pos()
{   return slider->value();}

void QPlayer::sliderPress()
{   busy = true;    }

void QPlayer::seekFile()
{
	file->seek(slider->value() * file->size() / 100);
	busy = false;
}


void QPlayer::play()
{
	playTimer->start();
	refreshTimer->start();
}


void QPlayer::pause()
{
	playTimer->stop();
	refreshTimer->stop();

}


bool QPlayer::isBusy()
{   return busy;    }
QSize QPlayer::minimumSizeHint() const
{
	return QSize(20, 20);
}


QSize QPlayer::sizeHint() const
{
	return QSize(xSize, ySize);
}
