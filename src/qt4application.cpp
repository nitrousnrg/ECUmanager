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


#include "qt4application.h"

qt4application::qt4application()
{
//init!
	RPM.entero = 0;
	VE.entero = 0;
	fuel.entero = 0;
	duty = 0;
	Dwell = 0;
	MAP = 0;
        TEMP = 0;
	waterTEMP = 0;
	throttle = 0;
	plot1 = NULL; //(QViewer)
	playMenu = NULL; //QPlayer
	serialThread = NULL;

        oxygenSensor = (char*)malloc(16);

	for(int row = 0;row<12;++row)
	{
		for(int column = 0;column<21;++column)
		{
			VEtable[row][column].entero = 0;
			IgnTable[row][column].angle = 0;
			IgnTable[row][column].byte[1] = 0;
			IgnTable[row][column].byte[0] = 0;
		}
		fuelAdv[row].byte[1] = 0;
		fuelAdv[row].byte[0] = 0;
	}
	confParameter.cylinders	= 0;
	confParameter.displacement = 0;
	confParameter.fuelCut = 0;
	confParameter.idleRPM = 0;
	confParameter.launchRPM = 0;
	confParameter.injSize = 0;
	confParameter.deadTime = 0;
	for(int i=0;i<13;++i)
		confParameter.tempCorr[i] = 0;

	confParameter.accel_pump_treshold = 100;
	confParameter.accel_pump_enrich_factor = 0;
	confParameter.accel_pump_decay = 0;

	confParameter.nitrousRPM = 0;
	confParameter.nitrousMAP = 0;
	confParameter.nitrousTPS = 0;
	confParameter.nitrousStage1Flow = 0;
	confParameter.nitrousStage2Flow = 0;
	confParameter.nitrousStage3Flow = 0;
	confParameter.nitrousStage1Retard = 0;
	confParameter.nitrousStage2Retard = 0;
	confParameter.nitrousStage3Retard = 0;


//oxygenSensor=="Narrowband";

	confFile = "";
	openedFile = "empty";
	logFileName = "noName";
	//Dialog flags. There is only one dialog object, but it is used in many forms
	parametersDialogOpen = false;
	tempDialogOpen = false;
	accelEnrichDialogOpen = false;
	nitrousDialogOpen = false;
	injectorTestDialogOpen = false;
	configDialogOpen = false;
        debugEnabledFlag = false;


	logbuffer[0] = 60;
	logbuffer[1] = 60;
	logbuffer[2] = 60;
	logbuffer[3] = 60;

        map3D = new QMap3D();		//This is the openGL 3D surface.

	for(int row = 0;row<12;++row)
		for(int column = 0;column<21;++column)
		{
			map3D->loadData(VEtable[row][column].entero,row,column);	
		}

	connectedECU = FALSE;

        setWindowTitle(tr("%1[*] - %2").arg("ECU manager").arg(tr("FreeEMS")));
	setWindowIcon(QIcon(":/amarok_configure.png"));
	
	logEnabled = false;
	Ign_table_ON = false;
	VE_table_ON = false;
	QuickTune_ON = false;

	createVEtable();
	createLinearTable();
	Ign_table = 0;
	createLabels();
	createBars();
	createActions();
        createMenus();
	createToolBars();
	createStatusBar();

        renderArea = new RenderArea;
	renderArea->setValue(0);

	plot = new PlotArea;
	plot->setScaleFactorY(14);

	createLayout();
	readSettings();
}

void qt4application::refresh()		//Here every data (bars, gauges, plots) is updated every 100msec
{
	if( plot1 != NULL )	//Viewer was opened
	{
		for(int i=0; i<=2; ++i)
		{
			switch(plot1->getChannel(i))
			{
				case 0:
					//lambda
					break;
				case 1:
					plot->desplazar(serialThread->getFChannel(commThread::RPM)/50);	//RPM
					break;
				case 2:
					plot->desplazar(serialThread->getFChannel(commThread::VE)/5);
					break;
				case 3:
					plot->desplazar(serialThread->getChannel(commThread::MAP)*1.3);
					break;
				case 4:
					plot->desplazar(serialThread->getFChannel(commThread::airTemp)*2);
					break;
				case 5:
					plot->desplazar(serialThread->getFChannel(commThread::waterTemp));
					break;
				case 6:
					plot->desplazar(serialThread->getFChannel(commThread::IgnAdvance)* 4);
					break;
				case 7:
					plot->desplazar(serialThread->getFChannel(commThread::FuelAdvance) );
					break;
				case 8:
					plot->desplazar(serialThread->getFChannel(commThread::DUTY)*2);
					break;
				case 9:
					plot->desplazar(serialThread->getFChannel(commThread::FUEL)/62.5);
					break;
				case PlotArea::throttle:
					plot->desplazar(serialThread->getFChannel(commThread::THROTTLE));
					break;
			}
		}
	}

	switch(plot->getChannel())
	{
		case 0:
			//lambda
			break;
		case 1:
			plot->desplazar(serialThread->getFChannel(commThread::RPM)/50);	//RPM
			break;
		case 2:
			plot->desplazar(serialThread->getFChannel(commThread::VE)/5);
			break;
		case 3:
			plot->desplazar(serialThread->getChannel(commThread::MAP)*1.3);
			break;
		case 4:
			plot->desplazar(serialThread->getFChannel(commThread::airTemp)*2);
			break;
		case 5:
			plot->desplazar(serialThread->getFChannel(commThread::waterTemp));
			break;
		case 6:
			plot->desplazar(serialThread->getFChannel(commThread::IgnAdvance)* 4);
			break;
		case 7:
			plot->desplazar(serialThread->getFChannel(commThread::FuelAdvance) );
			break;
		case 8:
			plot->desplazar(serialThread->getFChannel(commThread::DUTY)*2);
			break;
		case 9:
			plot->desplazar(serialThread->getFChannel(commThread::FUEL)/62.5);
			break;
		case PlotArea::throttle:
			plot->desplazar(serialThread->getFChannel(commThread::THROTTLE));
			break;
	}
	//plot->desplazar(MAP*1.66);
	//plot->desplazar(RPM.entero/50);

	renderArea->setValue( serialThread->getFChannel(commThread::RPM) / 1000);

	QString dataStream;
	dataStream="Ign Adv: ";
	auxiliarByteArray.setNum(serialThread->getFChannel(commThread::IgnAdvance),'f',1);
	dataStream += auxiliarByteArray + "\nDwell: ";
	auxiliarByteArray.setNum(Dwell,'f',1);

	dataStream += auxiliarByteArray + "ms\n\nFuel Adv: ";
	auxiliarByteArray.setNum(serialThread->getFChannel(commThread::FuelAdvance),'f',1);
	dataStream += auxiliarByteArray + "\nCorrections: ";
	auxiliarByteArray.setNum(0);
	dataStream += auxiliarByteArray + "%";

	
	heavyDataLabel->setText(dataStream);


	auxiliarByteArray.setNum(serialThread->getFChannel(commThread::VE),'f',1);
	VElabel->setText("VE\n"+auxiliarByteArray+"%");
	VEbar->setValue(serialThread->getChannel(commThread::VE));

	auxiliarByteArray.setNum(serialThread->getChannel(commThread::MAP));
	MAPbar->setRange(20,130);
	MAPbar->setValue(serialThread->getChannel(commThread::MAP));
	MAPlabel->setText("MAP\n"+auxiliarByteArray+"kPa");

	auxiliarByteArray.setNum(serialThread->getChannel(commThread::airTemp));
	AirTempbar->setValue(serialThread->getChannel(commThread::airTemp));
	AirTemplabel->setText("Air\nTemp\n"+auxiliarByteArray+"C");

	auxiliarByteArray.setNum(serialThread->getChannel(commThread::waterTemp));
	TEMPbar->setValue(serialThread->getChannel(commThread::waterTemp));
	TEMPlabel->setText("Water\nTemp\n"+auxiliarByteArray+"C");

	auxiliarByteArray.setNum(serialThread->getChannel(commThread::LAMBDA));
	AirFuelbar->setValue(serialThread->getChannel(commThread::LAMBDA));
	AirFuellabel->setText(("Lambda\n"+auxiliarByteArray));



	auxiliarByteArray.setNum(serialThread->getChannel(commThread::THROTTLE));
	throttleBar->setValue(serialThread->getChannel(commThread::THROTTLE));
	throttleLabel->setText("Throttle\n"+auxiliarByteArray+"%");

	auxiliarByteArray.setNum(serialThread->getFChannel(commThread::FUEL)/625,'f',1);
	InyTimebar->setValue(serialThread->getFChannel(commThread::FUEL)/625);
	InyTimelabel->setText("Inj\nTime\n"+auxiliarByteArray+"ms");

	auxiliarByteArray.setNum(serialThread->getFChannel(commThread::DUTY),'f',1);
	Dutybar->setValue(serialThread->getChannel(commThread::DUTY));
	Dutylabel->setText("Duty\n"+auxiliarByteArray+"%");

}

void qt4application::closeEvent(QCloseEvent *event)
{
	if(playMenu != NULL)
		playMenu->close();
	writeSettings();
	event->accept();
}

void qt4application::newFile()
{
}

void qt4application::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),confFile,tr("Config(*.txt);;Logged Data(*.log)"));
	if (!fileName.isEmpty())
	{
		confFile = fileName;	//wrong!
		openedFile = fileName;
		loadFile(fileName);
	}
}

void qt4application::save()
{
	if (openedFile == "empty") 
	{
		return saveAs();
	}
	else
	{
		saveFile(openedFile);
		return;
	}
//return TRUE;
}

void qt4application::saveAs()
{
      QString fileName = QFileDialog::getSaveFileName(this);
      if (!fileName.isEmpty())
		saveFile(fileName);
//	return TRUE;
}

void qt4application::connectECU()
{


        sendfileAct->setDisabled(false);
	upgradeFirmwareAct->setDisabled(false);
	startLogAct->setDisabled(false);
	fetchAct->setDisabled(false);
	injectorTestAct->setDisabled(false);
	connectAct->setDisabled(true);
	disconnectAct->setDisabled(false);
	refreshTimer->start();

	serialThread = new commThread();
	serialThread->setPort(serialPort);
	serialThread->setTarget(hardwareTarget);
	serialThread->run();
//	serialThread->setPort(serialPort);

           /* FreeEMS stuff */
//connect the signal emitted when a packet is sent or received
        if(connect(serialThread,SIGNAL(packetArrived(QByteArray)),this,SLOT(showPacket(QByteArray)))  )
            qDebug("seniales conectadas -connectECU()-");

        connect(getInterfaceVersionAct, SIGNAL(triggered()), serialThread, SLOT(getInterfaceVersion()));
        connect(getFirmwareVersionAct, SIGNAL(triggered()), serialThread, SLOT(getFirmwareVersion()));
        qDebug("seniales conectadas -connectECU()-");

	if(serialThread->isOnline())
	{
		connectedECU = TRUE;
		refreshTimer->start();
		statusBar()->showMessage(tr("Connected"),6);


		sendfileAct->setDisabled(false);
		upgradeFirmwareAct->setDisabled(false);
		startLogAct->setDisabled(false);
		fetchAct->setDisabled(false);
		injectorTestAct->setDisabled(false);

	}
	/*else
	{
		QMessageBox::warning(	this,
					tr("Application"),
					tr("Cannot open file %1:\n%2.").arg(serialPort).arg(serial->errorString()));
	}*/
}

void qt4application::disconnectECU()
{
	sendfileAct->setDisabled(true);
	upgradeFirmwareAct->setDisabled(true);
	startLogAct->setDisabled(true);
	fetchAct->setDisabled(true);
	injectorTestAct->setDisabled(true);
	connectAct->setDisabled(false);
	disconnectAct->setDisabled(true);
	refreshTimer->stop();

	delete serialThread;
}

void qt4application::checkChange(QTableWidgetItem *item)	//Cargo los cambios de la tabla a la VE (si son correctos)
{	int row = item->row();
	int column= item->column(); 
	if(VE_table_ON)
	{
		QVariant var = VEtableItem[row][column].data(Qt::DisplayRole);
		VEtable[row][column].entero = var.toInt();
		VEtable[row][column].entero *= 10;	//supongo que anda
		if(connectedECU)
		{
			QByteArray array("T");
			const unsigned char address = (11-row)*21+column;
			array.append(address);

			array.append(VEtable[row][column].byte[1]);
			array.append(VEtable[row][column].byte[0]);
			serial->write(array,4);
		}
		if(!QuickTune_ON)
		{
                //	map3D->loadData(((double)VEtable[row][column].entero)/100,row,column);
                //	map3D->update();
		}
	}

	if(fuel_adv_ON)
	{
		QVariant var = fuelAdvTableItem[column].data(Qt::DisplayRole);
		fuelAdv[column].angle = var.toDouble();
		fuelAdv[column].byte[1] = (BYTE)var.toDouble();
		fuelAdv[column].byte[0] = (BYTE)var.toDouble();


		if(connectedECU)
		{
			QByteArray array("E");	//noo
			const unsigned char address = column;
			array.append(address);

			array.append(fuelAdv[column].byte[1]);
			array.append(fuelAdv[column].byte[0]);
			serial->write(array,4);
		}

		//IgnTable[row][column].angle = (float)var.toDouble();
		//IgnTable[row][column].angle *= 10;	//supongo que anda
	}

//	tableItem[0][0].setData(Qt::DisplayRole, QVariant(VEtable[row][column].entero));


//if(item->data() > 0)
	

}

void qt4application::showFuel()
{
	if(!VE_table_ON)
	{
		Ign_table_ON = false;
		fuel_adv_ON = false;
		VE_table_ON = false;

		for(int row = 0;row<12;++row)
			for(int column = 0;column<21;++column)
			{
				VEtableItem[row][column].setTextAlignment(Qt::AlignVCenter);
				VEtableItem[row][column].setData(0, QVariant( ( (float)(VEtable[row][column].entero)/10 ) ));
				VE_table->setItem(row,column,&(VEtableItem[row][column]));
				VE_table->update();
			}
		VE_table->show();
		fuelAdvTable->hide();

		VE_table_ON = true;
	}
}


void qt4application::showFuelTiming()
{
	if( !fuel_adv_ON )
	{	VE_table->hide();
                fuelAdvTable->show();
	}
        fuel_adv_ON = true;
	Ign_table_ON = false;
	VE_table_ON = false;
}

void qt4application::showIgnition()
{
	if(Ign_table_ON == false)
	{
		VE_table_ON = false;
		fuel_adv_ON = false;

		for(int row = 0;row<12;++row)
			for(int column = 0;column<21;++column)
			{
				VEtableItem[row][column].setTextAlignment(Qt::AlignVCenter);
				VEtableItem[row][column].setData(0, QVariant( ( (float)(IgnTable[row][column].angle) ) ));
				VE_table->setItem(row,column,&(VEtableItem[row][column]));
			}
		VE_table->show();
		fuelAdvTable->hide();

		Ign_table_ON = true;
	}
}

void qt4application::openECU()
{
	serial = new QextSerialPort(serialPort);
	serial->setBaudRate(BAUD19200);
	serial->setParity(PAR_NONE);
	serial->setDataBits(DATA_8); 
	serial->setStopBits(STOP_1);
	serial->setFlowControl(FLOW_OFF);
	serial->setTimeout(0,20);
	if(!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::warning(	0,
					tr("Application"),
					tr("Cannot open file %1:\n%2.").arg(serialPort).arg(serial->errorString()));
	}

	serial->write("R");	//send command

	openECUtimer = new QTimer(this);

        connect(openECUtimer,SIGNAL(timeout()),this,SLOT(openECUslot()));
        openECUtimer->start(800);
	byteCounter = 0;



}
void qt4application::openECUslot()
{
	qDebug("processing");
	const qint64 available = serial->bytesAvailable();
	if(available > 900)
	{
		QByteArray data = serial->read(available);

		unsigned char *dataBurst = new unsigned char[available];
		for(int i=0; i<available;++i)
			*(dataBurst+i) = *(data.data()+i);
		int i = 0;
		while( !(dataBurst[i] == 0 && dataBurst[i+3] == 1 && dataBurst[i+6] == 2 && dataBurst[i+9] == 3) && i < 400)
			++i;

	// VE table
		for(i=i; dataBurst[i]<=251; i+=3)
		{
			if(dataBurst[i] == (dataBurst[i+3] - 1))
			{
				VEtable[11-dataBurst[i]/21][dataBurst[i]%21].byte[1] = dataBurst[i+1];
				VEtable[11-dataBurst[i]/21][dataBurst[i]%21].byte[0] = dataBurst[i+2];
			}
			else
				i -= 2;
		}

		i+=3;
	//Ign Advance Table
		for(i=i; dataBurst[i]<=251; i+=3)
		{
			if(dataBurst[i] == (dataBurst[i+3] - 1))
			{
				IgnTable[11-dataBurst[i]/21][dataBurst[i]%21].byte[1] = dataBurst[i+1];
				IgnTable[11-dataBurst[i]/21][dataBurst[i]%21].byte[0] = dataBurst[i+2];
				IgnTable[11-dataBurst[i]/21][dataBurst[i]%21].angle = dataBurst[i+1]*6;
				IgnTable[11-dataBurst[i]/21][dataBurst[i]%21].angle += (float)dataBurst[i+2]*6/255;	//Fix: round to 2 digits after comma
			}
			else
				i -= 2;
		}
		++i;
	// Leavanto EEPROM

		Dwell = dataBurst[i] / 10;
		++i;
		for(int j=0;j<12;++j,i+=2)
		{
			fuelAdv[j].byte[1] = dataBurst[i];
			fuelAdv[j].byte[0] = dataBurst[i+1];
			fuelAdv[j].angle = fuelAdv[j].byte[1] * 6;
			fuelAdv[j].angle += (float)fuelAdv[j].byte[0] * 6 / 255;	//Fix: round to 2 digits after comma

			fuelAdvTableItem[j].setTextAlignment(Qt::AlignVCenter);
			fuelAdvTableItem[j].setData(0, QVariant( fuelAdv[j].angle ));
			fuelAdvTable->setItem(0,j,&(fuelAdvTableItem[j]));
			fuelAdvTable->setColumnWidth(j,48);

		}
		

		// Después, levantar la EEPROM.
	}

	if(VE_table_ON)
	{
		VE_table_ON = false;
		showFuel();
		for(int row = 0;row<12;++row)
			for(int column = 0;column<21;++column)
                                map3D->loadData((double)VEtable[row][column].entero/100,row,column);

                map3D->update();

	}

	openECUtimer->stop();
	//timer->start();
	serial->close();
	qDebug("done");

	//openECUtimer->~openECUtimer();
}

void qt4application::sendFilesInit()	//programar!
{
	serialThread->burnFlash(QFileDialog::getOpenFileName(this, tr("Open File"),confFile,tr("Logged Data(*.log)")));
}

/*bool qt4application::maybeSave()
{
      if (textEdit->document()->isModified()) {
            int ret = QMessageBox::warning(this, tr("Application"),
                        tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::Default,
                        QMessageBox::No,
                        QMessageBox::Cancel | QMessageBox::Escape);
            if (ret == QMessageBox::Yes)
            return save();
            else if (ret == QMessageBox::Cancel)
            return false;
      }
      return true;
}*/


void qt4application::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
		QMessageBox::warning(this, tr("Application"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
	int i=0;
	while(!file.atEnd())
	{
		QByteArray line = file.readLine();
		process_line(line,i);
		++i;
	}
//      QApplication::setOverrideCursor(Qt::WaitCursor);
     // textEdit->setPlainText(in.readAll());
      QApplication::restoreOverrideCursor();

    //  setCurrentFile(fileName);
      statusBar()->showMessage(tr("File loaded"), 2000);
}
void qt4application::process_line(QByteArray line,int i)
{
	qDebug("reading line #%d",i);
	if(i>=2 && i<14)
	{
		sscanf(line,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
											&(VEtable[i-2][0].entero),
											&(VEtable[i-2][1].entero),
											&(VEtable[i-2][2].entero),
											&(VEtable[i-2][3].entero),
											&(VEtable[i-2][4].entero),
											&(VEtable[i-2][5].entero),
											&(VEtable[i-2][6].entero),
											&(VEtable[i-2][7].entero),
											&(VEtable[i-2][8].entero),
											&(VEtable[i-2][9].entero),
											&(VEtable[i-2][10].entero),
											&(VEtable[i-2][11].entero),
											&(VEtable[i-2][12].entero),
											&(VEtable[i-2][13].entero),
											&(VEtable[i-2][14].entero),
											&(VEtable[i-2][15].entero),
											&(VEtable[i-2][16].entero),
											&(VEtable[i-2][17].entero),
											&(VEtable[i-2][18].entero),
											&(VEtable[i-2][19].entero),
											&(VEtable[i-2][20].entero));
	}
	if( (i==13) && (VE_table_ON == true) ) //mal
	{
		VE_table_ON = false;
		for(int row = 0;row<12;++row)
                        for(int column = 0;column<21;++column)
			{
                             qDebug("VEtable fill: row: %d col: %d",row,column);
                                qDebug("VEtable = %f",(float)(VEtable[row][column].entero)/10 );
				VEtableItem[row][column].setTextAlignment(Qt::AlignVCenter);
                                VEtableItem[row][column].setData(0, QVariant( (float)(VEtable[row][column].entero)/10  ));

                                QColor cellColor;
                                cellColor.setHsv( (1000 - VEtable[row][column].entero) / 3, 230, 200);
                                VEtableItem[row][column].setBackground(QBrush(cellColor) );

                                VE_table->setItem(row,column,&(VEtableItem[row][column]));

			}
		for(int row = 0;row<12;++row)
			for(int column = 0;column<21 ;++column)
                                map3D->loadData((double)VEtable[row][column].entero/100,row,column);
		VE_table_ON = true;
	}

	if(i>=17 && i<29)
	{
		sscanf(line,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f",
											&(IgnTable[i-17][0].angle),
											&(IgnTable[i-17][1].angle),
											&(IgnTable[i-17][2].angle),
											&(IgnTable[i-17][3].angle),
											&(IgnTable[i-17][4].angle),
											&(IgnTable[i-17][5].angle),
											&(IgnTable[i-17][6].angle),
											&(IgnTable[i-17][7].angle),
											&(IgnTable[i-17][8].angle),
											&(IgnTable[i-17][9].angle),
											&(IgnTable[i-17][10].angle),
											&(IgnTable[i-17][11].angle),
											&(IgnTable[i-17][12].angle),
											&(IgnTable[i-17][13].angle),
											&(IgnTable[i-17][14].angle),
											&(IgnTable[i-17][15].angle),
											&(IgnTable[i-17][16].angle),
											&(IgnTable[i-17][17].angle),
											&(IgnTable[i-17][18].angle),
											&(IgnTable[i-17][19].angle),
											&(IgnTable[i-17][20].angle));
	}
	if(i==29)
	{
		for(int row = 0;row<12;++row)
			for(int column = 0;column<21;++column)
			{
				IgnTable[row][column].byte[1] = (unsigned char)(IgnTable[row][column].angle/6);
				IgnTable[row][column].byte[0] = (unsigned char)((IgnTable[row][column].angle/6 - (float)IgnTable[row][column].byte[1])*255);
			}

		if( (Ign_table_ON == true) )
			for(int row = 0;row<12;++row)
				for(int column = 0;column<21;++column)
				{
					VEtableItem[row][column].setTextAlignment(Qt::AlignVCenter);
					VEtableItem[row][column].setData( 0,QVariant( ( IgnTable[row][column].angle ) ));
					VE_table->setItem(row,column,&(VEtableItem[row][column]));
				}
	}

	if(i==32)
	{
		sscanf(line,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f",	&(fuelAdv[0].angle),
										&(fuelAdv[1].angle),
										&(fuelAdv[2].angle),
										&(fuelAdv[3].angle),
										&(fuelAdv[4].angle),
										&(fuelAdv[5].angle),
										&(fuelAdv[6].angle),
										&(fuelAdv[7].angle),
										&(fuelAdv[8].angle),
										&(fuelAdv[9].angle),
										&(fuelAdv[10].angle),
										&(fuelAdv[11].angle) );

		for(int column = 0;column<12;++column)
		{
			fuelAdvTableItem[column].setTextAlignment(Qt::AlignVCenter);
			fuelAdvTableItem[column].setData(0, QVariant( fuelAdv[column].angle ));
			fuelAdvTable->setItem(0,column,&(fuelAdvTableItem[column]));
			fuelAdvTable->setColumnWidth(column,48);

			fuelAdv[column].byte[1] = (unsigned char)(fuelAdv[column].angle/6);
			fuelAdv[column].byte[0] = (unsigned char)((fuelAdv[column].angle/6 - fuelAdv[column].byte[1])*256);
		}

	}

	//Engine parameters
//-4
	if(i==37)
		sscanf(line,"%d",&(confParameter.cylinders));
	if(i==39)
		sscanf(line,"%d",&(confParameter.displacement));
	if(i==41)
	{
		sscanf(line,"%d",&(confParameter.fuelCut));
		renderArea->setLimit(confParameter.fuelCut + 1000);
	}
	if(i==43)
		sscanf(line,"%d",&(confParameter.idleRPM));
	if(i==45)
		sscanf(line,"%d",&(confParameter.launchRPM));
	if(i==47)
		sscanf(line,"%d",&(confParameter.injSize));
	if(i==49)
		sscanf(line,"%d",&(confParameter.deadTime));
	if(i==51)
                sscanf(line,"%s",oxygenSensor);

	if(i>=54 && i <=66)	//64?
	{int nada;
		sscanf(line,"%d\t%f", &nada,&(confParameter.tempCorr[i-54]));
	}

	if(i==69)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%d",rockandrollnenene,&(confParameter.nitrousRPM));
	}
	if(i==70)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%d",rockandrollnenene,&(confParameter.nitrousMAP));
	}
	if(i==71)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%d",rockandrollnenene,&(confParameter.nitrousTPS));
	}
	if(i==74)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage1Flow));
	}
	if(i==75)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage1Retard));
	}
	if(i==78)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage2Flow));
	}
	if(i==79)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage2Retard));
	}
	if(i==82)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage3Flow));
	}
	if(i==83)
	{	char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage3Retard));
	}
	if(i==86)
	{	char rockandrollnenene[15];
		char source[16];
		sscanf(line,"%[^\t]\t%s",rockandrollnenene,source);
		if(strcmp(source, "MAP"))
			confParameter.accel_pump_source = MAPsourced;
		if(strcmp(source, "TPS"))
			confParameter.accel_pump_source = TPSsourced;
	}
	if(i==87)
	{	char rockandrollnenene[15];
                sscanf(line,"%[^\t]%c",rockandrollnenene,&(confParameter.accel_pump_treshold));
	}
	if(i==88)
	{	char rockandrollnenene[15];
                sscanf(line,"%[^\t]%c",rockandrollnenene,&(confParameter.accel_pump_decay));
	}
	if(i==89)
	{	char rockandrollnenene[15];
                sscanf(line,"%[^\t]%c",rockandrollnenene,&(confParameter.accel_pump_enrich_factor));
	}


}


bool qt4application::saveFile(const QString &fileName)
{
	QByteArray array;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
        	QMessageBox::warning(this, tr("Application"),tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return false;
	}

	file.write("\tFuel Map\n\n");

	for(int row = 0;row<12;++row)
	{
		for(int column = 0;column<21;++column)
		{
			array.setNum(VEtable[row][column].entero);
			file.write(array + "\t");
		}
		file.write("\n");
	}

	file.write("\n\tIgn Map (°BTDC)\n\n");
	for(int row = 0;row<12;++row)
	{
		for(int column = 0;column<21;++column)
		{
			array.setNum(IgnTable[row][column].angle,'f',2);
			file.write(array + "\t");
		}
		file.write("\n");
	}

	file.write("\n\tFuel Advance (°BTDC)\n\n");
	for(int i=0;i<12;++i)
	{
		array.setNum(fuelAdv[i].angle,'f',2);
		file.write(array + "\t");
	}

	file.write("\n\n\tEngine Parameters\n\nCylinders:\n");
	file.write(array.setNum(confParameter.cylinders)+"\n");
	file.write("Displacement (cm³):\n");
	file.write(array.setNum(confParameter.displacement)+"\n");
	file.write("FuelCut (RPM):\n");
	file.write(array.setNum(confParameter.fuelCut)+"\n");
	file.write("Idle (RPM):\n");
	file.write(array.setNum(confParameter.idleRPM)+"\n");
	file.write("Launch ctrl (RPM):\n");
	file.write(array.setNum(confParameter.launchRPM)+"\n");
	file.write("Injector Flow (cm³/min):\n");
	file.write(array.setNum(confParameter.injSize)+"\n");
	file.write("Injector DeadTime (microSec):\n");
	file.write(array.setNum(confParameter.deadTime)+"\n");
	file.write("Oxygen sensor type:\n");
	file.write(oxygenSensor);
	file.write("\n\nTemperature correction table (°C vs enrich %)\n");

	for(int i=0;i<13;++i)
	{
		array.setNum(i*10-10);
		file.write(array + "\t");
		array.setNum(confParameter.tempCorr[i]);
		file.write(array + "\n");
	}
	file.write("\nNitrous Control\nminimum RPM:\t");
	file.write(array.setNum(confParameter.nitrousRPM)+"\n");
	file.write("minimum MAP:\t");
	file.write(array.setNum(confParameter.nitrousMAP)+"\n");
	file.write("minimum TP:\t");
	file.write(array.setNum(confParameter.nitrousTPS)+"\n\n");
	file.write("Stage 1:\nExtra Fuel (ms)\t");
	file.write(array.setNum(confParameter.nitrousStage1Flow)+"\n");
	file.write("Ign Retard:\t");
	file.write(array.setNum(confParameter.nitrousStage1Retard)+"\n");
	file.write("\nStage 2:\nExtra Fuel (ms)\t");
	file.write(array.setNum(confParameter.nitrousStage2Flow)+"\n");
	file.write("Ign Retard:\t");
	file.write(array.setNum(confParameter.nitrousStage2Retard)+"\n");
	file.write("\nStage 3:\nExtra Fuel (ms)\t");
	file.write(array.setNum(confParameter.nitrousStage3Flow)+"\n");
	file.write("Ign Retard:\t");
	file.write(array.setNum(confParameter.nitrousStage3Retard)+"\n\n");
	file.write("Accel enrichment\n");
	file.write("Source:\t\t");
	if(confParameter.accel_pump_source == MAPsourced)
		file.write("MAP\n");
	else
		file.write("TPS\n");
	file.write("Treshold:\t");
	file.write(array.setNum(confParameter.accel_pump_treshold)+"\n");
	file.write("Decay:\t\t");
	file.write(array.setNum(confParameter.accel_pump_decay)+"\n");
	file.write("Amplifier:\t");
	file.write(array.setNum(confParameter.accel_pump_enrich_factor)+"\n");



	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void qt4application::acceptDialog()
{
	//aquí guarda toda la nueva configuración (de todos los diálogos posibles)
	//ojo es info almacenada en RAM
	if(parametersDialogOpen)
	{
		string1 = editDisplacement->text();
		confParameter.displacement = string1.toUInt();

		string1 = editCyl->text();
		confParameter.cylinders = string1.toUShort();

		string1 = editIdle->text();
		confParameter.idleRPM = string1.toUInt();
	
		string1 = editFuelCut->text();
		confParameter.fuelCut = string1.toUInt();
		renderArea->setLimit(confParameter.fuelCut +1000);

		string1 = editLaunchRPM->text();
		confParameter.launchRPM = string1.toUInt();

		string1 = editSize->text();
		confParameter.injSize = string1.toUInt();

		string1 = editDeadTime->text();
		confParameter.deadTime = string1.toUInt();
		parametersDialogOpen = false;
	}
	if(tempDialogOpen)
	{
		for(int i=0;i<13;++i)
			confParameter.tempCorr[i] = (float)tempCorrGraph->getPoint(i)/360;
		tempDialogOpen = false;
	}

	if(accelEnrichDialogOpen)
	{
		string1 = enrichFactorEdit->text();
		confParameter.accel_pump_enrich_factor = string1.toUInt();

		string1 = decayEdit->text();
		confParameter.accel_pump_decay = string1.toUInt();
		
		string1 = accelTresholdEdit->text();
		confParameter.accel_pump_treshold = string1.toUInt();
		accelEnrichDialogOpen = false;
		
	}
	if(nitrousDialogOpen)
	{
		string1 = editNitrousRPM->text();
		confParameter.nitrousRPM = string1.toUInt();

		string1 = editNitrousMAP->text();
		confParameter.nitrousMAP = string1.toUInt();

		string1 = editNitrousTPS->text();
		confParameter.nitrousTPS = string1.toUInt();

		string1 = editNitrousFlow1->text();
		confParameter.nitrousStage1Flow = string1.toFloat();
		string1 = editNitrousFlow2->text();
		confParameter.nitrousStage2Flow = string1.toFloat();
		string1 = editNitrousFlow3->text();
		confParameter.nitrousStage3Flow = string1.toFloat();

		string1 = editNitrousRetard1->text();
		confParameter.nitrousStage1Retard = string1.toFloat();
		string1 = editNitrousRetard2->text();
		confParameter.nitrousStage2Retard = string1.toFloat();
		string1 = editNitrousRetard3->text();
		confParameter.nitrousStage3Retard = string1.toFloat();
		nitrousDialogOpen = false;
	}

	if(configDialogOpen)
	{
		serialPort = serialPortEdit->text();
		hardwareTarget = hardwareTargetBox->itemText(hardwareTargetBox->currentIndex());
		configDialogOpen = true;
	}

	dialog->accept();
}

void qt4application::rejectDialog()
{	dialog->reject();}


void qt4application::startLog()
{
	//tempLogFile = new QFile("templog.log");
	//tempLogFile->open(QIODevice::WriteOnly);
	

	//logEnabled=TRUE;

	serialThread->startLogging();

	saveLogAct->setDisabled(false);
	startLogAct->setDisabled(true);

	statusBar()->showMessage(tr("Recording"));

}

void qt4application::saveLog()
{
	QDateTime date;
	date = date.currentDateTime();
	QString dateString = date.toString("d-M-yy hh:mm.log");
	statusBar()->showMessage(dateString);
		const QString logFileName = QFileDialog::getSaveFileName(this,	tr("Choose log file"),
										dateString,
										tr("Datalog File(*.log)"));
	serialThread->saveLog(logFileName);
}

void qt4application::replay()
{
	logFileName = QFileDialog::getOpenFileName(this, tr("Choose log file"),logFileName,tr("Logged Data(*.log)"));

	if (!logFileName.isEmpty())
	{
		if( !serialThread )
		{
			serialThread = new commThread();
			//serialThread->setPort(serialPort);
			serialThread->setTarget(hardwareTarget);
			serialThread->run();
		}
		serialThread->replayLog(logFileName);


		if(connectedECU)
		{
			connectedECU = false;
		}
		else
		{
		//	timer->start();
			refreshTimer->start();
			statusBar()->showMessage(tr("Playing"),6);
		}

		sendfileAct->setDisabled(true);
		upgradeFirmwareAct->setDisabled(true);
		startLogAct->setDisabled(true);

	//	playMenu = new QPlayer(timer, refreshTimer,openedLogFile);
	}	
	else
		logFileName = "noName";
}

void qt4application::viewData()
{
	plot1 = new QViewer();
	plot1->show();
}

void qt4application::upgradeFirmware()
{
  //     const QString firmwareFile = QFileDialog::getOpenFileName(this, tr("Choose firmware"),"",tr("MicrochipFlashCode(*.hex)"));
//	upgrade *newFirmware = new upgrade(serial,&firmwareFile);
}

QString qt4application::strippedName(const QString &fullFileName)
{
      return QFileInfo(fullFileName).fileName();
}

qt4application::~qt4application()
{
	if(openedLogFile->isOpen())
		openedLogFile->close();

	//plot1->close();	//no anduvo
}
