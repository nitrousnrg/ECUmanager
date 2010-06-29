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
#ifndef  UPDATER_H
#define UPDATER_H

#include "../lib/QExtSerialPort/qextserialport.h"

#include <QTimer>
//RTS clock
//DTR data

class upgrade
{
	public:
		upgrade(QextSerialPort *,const QString *);
		void loadFlashData(int data);
		void incrAddress();
		void erase_program();

	private:
		//void loadFlash();
		void sendBit(bool bit);

		QTimer *timer;
		QextSerialPort *serial;
};
#endif
