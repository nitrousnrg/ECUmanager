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

#include "QuickTune.h"

QuickTune::QuickTune(QWidget *parent) : QWidget(parent)
{
	versionName = "QuickTune v0.01";
	setWindowTitle(versionName);
	setGeometry(300,100,600,600);

	tableMAP = new graph2D;
	tableMAP->setColumns(12);
	for(int i=0;i<12;++i)
	{
		tableMAP->setPoint(i*20,0);
		tableMAP->setScaleX(i,i*10+20);
		if(i<9)
			tableMAP->setScaleY(i,i*10);
	}

	tableRPM = new graph2D;
	tableRPM->setColumns(21);
	for(int i=0;i<21;++i)
	{
		tableRPM->setPoint(i*20,0);
		tableRPM->setScaleX(i,i*500);
		if(i<9)
			tableRPM->setScaleY(i,i*10);
	}

	QGridLayout * grid = new QGridLayout;
	grid->addWidget(tableMAP,0,0);
	grid->addWidget(tableRPM,1,0);

	setLayout(grid);

	connect(tableMAP,SIGNAL(valueChanged(int)),this,SLOT(MAPvalueChanged(int)));
	connect(tableRPM,SIGNAL(valueChanged(int)),this,SLOT(RPMvalueChanged(int)));

	show();
}

void QuickTune::MAPvalueChanged(int index)
{
    if(index<12)   //number of columns limit
        emit QTuneChange(MAP,index); //0
}
void QuickTune::RPMvalueChanged(int index)
{
    if(index<21)   //number of columns limit
        emit QTuneChange(RPM,index); //0
}

int QuickTune::getMAP(int index)
{
    if(index<12)   //number of columns limit
	return tableMAP->getPoint(index);
}
int QuickTune::getRPM(int index)
{
    if(index<21)   //number of columns limit
        return tableRPM->getPoint(index);
}

