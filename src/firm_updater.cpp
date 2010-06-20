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

#include <firm_updater.h>

 upgrade::upgrade (QextSerialPort *serialreferenced,const QString *fileName)
{
	serial = serialreferenced;
	serial->setRts(1);
        fileName = 0;
}

void upgrade::loadFlashData(int data)
{
/* Command to send:
0 1 0 0 0
*/
	sendBit(0);
	sendBit(1);
	sendBit(0);
	sendBit(0);
	sendBit(0);

//now, send data

	sendBit(0);	//start
	for(int i=0;i<14;++i)
		sendBit(data & (1<<i));		//LSB first
	sendBit(0);	//stop
}

void upgrade::incrAddress()
{
	sendBit(0);
	sendBit(0);
	sendBit(1);
	sendBit(1);
	sendBit(0);
}

void upgrade::erase_program()
{
	sendBit(0);
	sendBit(1);
	sendBit(0);
	sendBit(0);
	sendBit(0);
}
void upgrade::sendBit(bool bit)
{
	serial->setRts(1);
	serial->setDtr(bit);	//data
	serial->setRts(0);	//captures on clock's falling edge
}








