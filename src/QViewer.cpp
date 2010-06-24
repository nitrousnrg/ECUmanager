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
#include "QViewer.h"

QViewer::QViewer()
{
	versionName = "QViewer v0.2";
	setWindowTitle(versionName);
	setWindowIcon(QIcon(":/amarok_visualizations.png"));
	setGeometry(300,100,600,600);

	fileName = "";
	online = false;

	openFileAct = new QAction(QIcon(":/fileopen.xpm"), tr("&Open..."), this);
	openFileAct->setShortcut(tr("Ctrl+O"));
	openFileAct->setStatusTip(tr("Open an existing file"));
	connect(openFileAct, SIGNAL(triggered()), this, SLOT(openFile()));

	exportCsvAct = new QAction(tr("&Export to CSV"), this);
	exportCsvAct->setShortcut(tr("Ctrl+E"));
	exportCsvAct->setDisabled(true);
	exportCsvAct->setStatusTip(tr("Export opened file"));
	connect(exportCsvAct, SIGNAL(triggered()), this, SLOT(exportCsv()));

	printAct = new QAction(tr("Print"), this);
	printAct->setShortcut(tr("Ctrl+P"));
	printAct->setDisabled(true);
	connect(printAct, SIGNAL(triggered()), this, SLOT(printDialog()));

	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openFileAct);
	fileMenu->addAction(exportCsvAct);
	fileMenu->addAction(printAct);

	channel[0] = PlotArea::MAP;
	channel[1] = PlotArea::injTime;
	channel[2] = PlotArea::RPM;

	QVBoxLayout *vMainLayout = new QVBoxLayout();

	display = new QSubViewer[3];

	connect(display  ,SIGNAL(valueChanged(int)),this,SLOT(updateChannel0(int)));
	connect(display+1,SIGNAL(valueChanged(int)),this,SLOT(updateChannel1(int)));
	connect(display+2,SIGNAL(valueChanged(int)),this,SLOT(updateChannel2(int)));

	//	display[1] = new QSubViewer();
	//	display[2] = new QSubViewer();

	display[0].setChannel(PlotArea::MAP);
	display[1].setChannel(PlotArea::injTime);
	display[2].setChannel(PlotArea::RPM);

	vMainLayout->addWidget(display);
	vMainLayout->addWidget(display+1);
	vMainLayout->addWidget(display+2);

	window = new QWidget;
	window->setLayout(vMainLayout);

	setCentralWidget(window);

	readSettings();
}


void QViewer::createDisplay()
{
}


QSize QViewer::minimumSizeHint() const
{
	return QSize(200, 200);
}


QSize QViewer::sizeHint() const
{
	return QSize(300, 300);
}


QString QViewer::getVersion()
{   return versionName; }

void QViewer::openFile()
{
	fileName = QFileDialog::getOpenFileName(this, tr("Open File"),fileName,tr("Log Data(*.log)"));
	if (fileName.isEmpty())
		return;

	exportCsvAct->setDisabled(false);
	printAct->setDisabled(false);

	file = new QFile(fileName);
	file->open( QIODevice::ReadOnly);

	plotChannelData(channel[0],0);
	plotChannelData(channel[1],1);
	plotChannelData(channel[2],2);

	plotChannelData(channel[0],0);
	plotChannelData(channel[1],1);
	plotChannelData(channel[2],2);
	online = false;

}


void QViewer::updateChannel0(int newChannel)
{
	if(online)
	{
		channel[0] = newChannel;

		//setChannel(newChannel,0);
	}
	else
	{
		if(!fileName.isEmpty())
		{
			plotChannelData(newChannel,0);

		}
	}
}


//esto es de negro
void QViewer::updateChannel1(int newChannel)
{
	if(!fileName.isEmpty())
	{
		plotChannelData(newChannel,1);

	}
}


void QViewer::updateChannel2(int newChannel)
{
	if(!fileName.isEmpty())
	{
		plotChannelData(newChannel,2);

	}
}


								 //n is the display #
void QViewer::setChannel(int newChannel,int n)
{
	(display+n)->setChannel(newChannel);
	(display+n)->clear();
}


void QViewer::plotChannelData(int channelData,int index)
{
	file->reset();
	bool twoBytes = false;

	switch(channelData)
	{
		case PlotArea::lambda:
			channelData = 255;	 //not implemented
			break;
		case PlotArea::RPM:
			channelData = 3;
			twoBytes = true;
			break;
		case PlotArea::VE:
			channelData = 5;
			twoBytes = true;
			break;
		case PlotArea::MAP:
			channelData = 1;
			break;
		case PlotArea::airTemp:
			channelData = 2;
			break;
		case PlotArea::injTime:
			channelData = 7;
			twoBytes = true;
			break;
		case PlotArea::waterTemp:
			channelData = 14;
			break;
		case PlotArea::throttle:
			channelData = 15;
			break;
	}

	int dataSize = file->size()/26;
	//int *realdata = new int[file->size()/12];
	int n = 0;
	while(!file->atEnd())
	{

		for(int i=0;i<5;++i)
			data[i] = data[i+1];

		file->getChar(data+5);

		if(twoBytes)
		{
			if(data[2] == channelData)
			{
				if(( (data[4] == data[2]+1) || (data[2] == 15)) && ((data[2] == data[0]+1) || data[0]==15))
				{
					//	if(900 > n)
					auxTransformer.byte[1] = data[3];
					auxTransformer.byte[0] = data[5];
					display[index].setPoints(auxTransformer.entero/100,dataSize);
					//realdata[n] = 4;//data[5];
					++n;
				}
			}
		}
		else
		if(data[4] == channelData)
			if(( (data[4] == data[2]+1) || (data[2] == 15)) && ((data[2] == data[0]+1) || data[0]==15))
		{
			//	if(900 > n)
			display[index].setPoints(data[5],dataSize);
			//realdata[n] = 4;//data[5];
			++n;
		}
	}
	//display1->setPoints(realdata,n);

	//delete realdata;
}


bool QViewer::exportCsv()
{
	bool ready = false;
	QString csvFileName=fileName;
	csvFileName.replace(".log",".csv");

	csvFileName = QFileDialog::getSaveFileName(this,tr("Export to Comma Separated Value"),
		csvFileName,
		tr("CSV File(*.csv)"));
	if (csvFileName.isEmpty())
		return false;

	file->reset();

	QFile *csvFile = new QFile(csvFileName);
	csvFile->open(QIODevice::WriteOnly | QIODevice::Text);

								 //header
	csvFile->write("MAP,air Temp,RPM,VE,inj Time,Duty,inj Adv,ign Adv,Dwell,water Temp,Throttle pos\n");
	char buffer[] = {0,0,0,0,0,0,0,0,0,0};
	while (!file->atEnd())
	{
		int i;
		float duty;
		QByteArray string;
		for(i=0;i<7;++i)
			buffer[i] = buffer[i+1];
		i=5;
		file->getChar(buffer+7);

		//if(((buffer[6] == buffer[4]+1) && ( (buffer[4] == buffer[2]+1 ) || ((buffer[4] == 13) && (buffer[6] == 1)))) || ((buffer[2] == 13) && (buffer[4] == 1)) )
		if(((buffer[6] == buffer[4]+1) || ((buffer[4] == 17) && (buffer[6] == 1))) )
		{
			if(!ready && (buffer[6] == 1) && (buffer[2] == 16))
				ready = true;
			if(ready)
			{
				switch(buffer[6])
				{
					case 1:		 //MAP
						string.setNum(buffer[7]);
						csvFile->write(string+',');
						break;
					case 2:		 //TEMP
						string.setNum(buffer[7]);
						csvFile->write(string+',');
						break;
					case 3:		 //RPM
						RPM.byte[1] = buffer[7];
						break;
					case 4:
						RPM.byte[0] =  buffer[7];
						string.setNum(RPM.entero);
						csvFile->write(string+',');
						break;
					case 5:		 //VE
						auxTransformer.byte[1] = buffer[7];
						break;
					case 6:
						auxTransformer.byte[0] =  buffer[7];
						string.setNum((float)RPM.entero/10,'f',2);
						csvFile->write(string+',');
						break;
					case 7:		 //fuel
						auxTransformer.byte[1] = buffer[7];
						break;
					case 8:
						auxTransformer.byte[0] = buffer[7];

						duty = (float)auxTransformer.entero/625 * (float)RPM.entero/600;

						string.setNum((float)auxTransformer.entero/625,'f',1);
						csvFile->write(string+',');
						string.setNum(duty,'f',1);
						csvFile->write(string+',');
						break;
					case 9:		 //fueladv
						auxTransformer.byte[1] = buffer[7];
						break;
					case 10:
						auxTransformer.byte[0] = buffer[7];
						string.setNum((float)(auxTransformer.byte[1]*6 + (float)auxTransformer.byte[0]*6/255),'f',2);
						csvFile->write(string+',');
						break;
					case 11:	 //Ignition Advance
						auxTransformer.byte[1] = buffer[7];
						break;
					case 12:
						auxTransformer.byte[0] = buffer[7];
						string.setNum((float)(auxTransformer.byte[1]*6 + (float)auxTransformer.byte[0]*6/255),'f',2);
						csvFile->write(string+',');
						break;
					case 13:
						string.setNum((float)buffer[7]/10,'f',1);
						csvFile->write(string+',');
						break;
					case 14:
						string.setNum(buffer[7]);
						csvFile->write(string+',');
						break;
					case 15:
						string.setNum(buffer[7]);
						csvFile->write(string+'\n');
						break;
				}
			}
		}
	}
	csvFile->close();
	return true;
}


void QViewer::readSettings()
{
	QSettings settings("FreeEMS", "QViewer");
	QPoint pos = settings.value("pos", QPoint(200, 400)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	fileName = settings.value("defaultFile",QString()).toString();

	display->setBackColor( settings.value("plotBackground Color. Display 1",Qt::black).value<QColor>() );
	display->setFontColor( settings.value("plotFont Color. Display 1",Qt::gray).value<QColor>() );
	display->setChannel1Color( settings.value("plotChannel 1 Color. Display 1",Qt::blue).value<QColor>() );

	(display+1)->setBackColor( settings.value("plotBackground Color. Display 2",Qt::black).value<QColor>() );
	(display+1)->setFontColor( settings.value("plotFont Color. Display 2",Qt::gray).value<QColor>() );
	(display+1)->setChannel1Color( settings.value("plotChannel 1 Color. Display 2",Qt::red).value<QColor>() );

	(display+2)->setBackColor( settings.value("plotBackground Color. Display 3",Qt::black).value<QColor>() );
	(display+2)->setFontColor( settings.value("plotFont Color. Display 3",Qt::gray).value<QColor>() );
	(display+2)->setChannel1Color( settings.value("plotChannel 1 Color. Display 3",Qt::green).value<QColor>() );

	resize(size);
	move(pos);
}


void QViewer::writeSettings()
{
	QSettings settings("FreeEMS", "QViewer");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("defaultFile",fileName);

	settings.setValue("plotBackground Color. Display 1",(display)->getBackgroundColor());
	settings.setValue("plotFont Color. Display 1",(display)->getFontColor());
	settings.setValue("plotChannel 1 Color. Display 1",(display)->getChannelColor(1));

	settings.setValue("plotBackground Color. Display 2",(display+1)->getBackgroundColor());
	settings.setValue("plotFont Color. Display 2",(display+1)->getFontColor());
	settings.setValue("plotChannel 1 Color. Display 2",(display+1)->getChannelColor(1));

	settings.setValue("plotBackground Color. Display 3",(display+2)->getBackgroundColor());
	settings.setValue("plotFont Color. Display 3",(display+2)->getFontColor());
	settings.setValue("plotChannel 1 Color. Display 3",(display+2)->getChannelColor(1));
}


void    QViewer::desplazar(int selectedChannel, float value)
{
	online = true;
	(display+selectedChannel)->desplazar(value);
}


int QViewer::getChannel(int n)	 //Retorna el canal que est monitoreando el display n
{
	return channel[n];
}


void    QViewer::printDialog()
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QPrinter *printer = new QPrinter(QPrinter::ScreenResolution);
	printer->setOrientation(QPrinter::Landscape);
	QPrintDialog printDialog(printer,this);

	if (printDialog.exec() == QDialog::Accepted)
	{
		QPainter painter;
		painter.begin(printer);
		painter.scale(0.65,0.70);
		for(int i=0; !file.atEnd(); i += 12)
		{
			//				painter.drawText(0,i,QString(file.readLine()));
		}
		//printer->newPage();
		// print ...
		painter.end();
	}
}


void QViewer::closeEvent(QCloseEvent *event)
{
	writeSettings();
	event->accept();
}
