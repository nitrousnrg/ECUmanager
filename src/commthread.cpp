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
#include "commthread.h"

/* write down here all communications protocols.
los datos deben salir por algn lado hacia qt4application. TODO: send 'U' when escaping */

void commThread::run()
{
	exec();
}

bool commThread::openPort()
{
	mutex.lock();

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(readSerialPort()));
	timer->setInterval(80);		 //as fast as you want. 20 works well with the PIC version
	bytes_to_int.entero = 0;

	online = false;

	serial = new QextSerialPort(serialPort.name);
	serial->setBaudRate( (BaudRateType)serialPort.serialPortBaudRate);
	serial->setParity( (ParityType)serialPort.serialPortParity);
	serial->setDataBits( (DataBitsType)serialPort.dataBits);
	serial->setStopBits( (StopBitsType)serialPort.stopBits);
	serial->setFlowControl(FLOW_OFF);
	serial->setTimeout(0,20);

	qDebug() << "Listening on " << serialPort.name;
	buffer = "";

	if(serial->open(QIODevice::ReadWrite))
	{
		timer->start();
		online = true;
		//	serial->write("C",1);		//PC is connected!!

	}
	else
	{
		qDebug("Cannot open serial port");
		QMessageBox::warning(   0,
			tr("Application"),
			tr("Cannot open file %1:\n%2.").arg(serialPort.name).arg(serial->errorString()));
	}
	mutex.unlock();

	return online;
}

								 //needed before run()
void commThread::setPort(serialPorts portAddress)
{
	if(serial == 0)
	{
		serialPort = portAddress;
		/*		serial = new QextSerialPort(serialPort);
				serial->setBaudRate(BAUD19200);
				serial->setParity(PAR_NONE);
		serial->setDataBits(DATA_8);
		serial->setStopBits(STOP_1);
		serial->setFlowControl(FLOW_OFF);
		serial->setTimeout(0,20);

		qDebug() << "Listening on " << serialPort;
		//qDebug( serialPort.latin1());
		buffer = "";

		if(serial->open(QIODevice::ReadWrite))
		{
		timer->start();
		online = true;
		//	serial->write("C",1);		//PC is connected!!

		}
		else
		{
		qDebug("Cannot open serial port");
		QMessageBox::warning(	0,
		tr("Application"),
		tr("Cannot open file %1:\n%2.").arg(serialPort).arg(serial->errorString()));
						}*/
		;
	}
	else
	{
		serial->close();
		serial->setPortName(portAddress.name);
		serial->open(QIODevice::ReadWrite);
	}
}


void commThread::replayLog(QString fileName)
{
	if( serial )
	{
		serial->close();
		serial = 0;
		timer->stop();
	}

	binaryFileName = fileName;

	logFile = new QFile(fileName);
	if (logFile->open(QFile::ReadOnly ))
	{
		timer->start();
	}
	else
		qDebug("wrong file");
}

//Here you switch between different device procols:
void commThread::readSerialPort()
{
								 //alpha version of pic16F877's firmware
	if(hardwareTarget == "ePIC project")
	{
		read_PIC_data();
	}
	if(hardwareTarget == "freeEMS project")
	{
		read_FreeEMS_data();
	}
								 //rt linux-based controller
	if(hardwareTarget == "core28")
	{
		//  read_core28_data();
	}

	//poner los procolos de todas las versiones.
}


void commThread::read_PIC_data()
{
	if(serial)
	{
		const qint64 available1 = serial->bytesAvailable();
		available = available1;
		if( available1 >= 9)
		{
			buffer = serial->read(available1);
			if( logging == true )
								 //,available);	//sacar available?
				tempLogFile->write(buffer);
		}
	}
	else
	{
		if((logFile->bytesAvailable() < 30))
		{
			available = 0;

			timer->stop();
			logFile->close();
		}
		else
		{
			buffer = logFile->read(18);
			available = 18;
		}
	}

	if( available >= 9)
	{

		int enable=0;

		for(int i=0;i<available-6;++i)
		{
			enable=0;

			if(((buffer[i]+1) == buffer[i+2]) && ( ((buffer[i+2]+1) == buffer[i+4] ) && ((buffer[i+4]+1) == buffer[i+6] )))
				enable = 1;

			if((buffer.at(i) == 17) && (buffer.at(i+2) == 1) && (buffer.at(i+4) == 2) && (buffer.at(i+6) == 3))
				enable = 2;

			if((buffer.at(i) == 16) && (buffer.at(i+2) == 17) && (buffer.at(i+4) == 1) && (buffer.at(i+6) == 2))
				enable = 3;

			if((buffer.at(i) == 15) && (buffer.at(i+2) == 16) && (buffer.at(i+4) == 17) && (buffer.at(i+6) == 1))
				enable = 4;

			if( (buffer.at(i) > 17) )
				enable=0;
			if( (buffer.at(i+2) > 17) )
				enable=0;
			if( (buffer.at(i+4) > 17) )
				enable=0;
			if( (buffer.at(i+6) > 17) )
				enable=0;
			if(i<2)
				enable=0;
			/*if(((((buffer[i]+1) == buffer[i+2]) && ( ((buffer[i+2]+1) == buffer[i+4] ) || ((buffer.at(i+2) == 15) && (buffer.at(i+4) == 1))))
						|| ((buffer.at(i) == 15) && (buffer.at(i+2) == 1)) ))*/
			if(enable)
				switch(buffer[i])
				{
					case 1:		 //MAP
						auxiliarByteArray.setNum(buffer[i+1]);
						channel[MAP] = auxiliarByteArray.toUShort();
						break;
					case 2:		 //TEMP
						channel[airTemp] = buffer.at(i+1);
						break;
					case 4:		 //RPM
						bytes_to_int.byte[1] = buffer[i-1];
						bytes_to_int.byte[0] =  buffer[i+1];
						channel[RPM] = bytes_to_int.entero;
					//qDebug() << "RPM" << bytes_to_int.byte[1] << " " << bytes_to_int.byte[0]<< "enable=" << enable << "i=" << i ;
						break;
					case 6:		 //VE
						bytes_to_int.byte[1] = buffer[i-1];
						bytes_to_int.byte[0] =  buffer[i+1];
						channel[VE] = (float)bytes_to_int.entero/10;

						break;
					case 8:		 //FUEL
						bytes_to_int.byte[1] = buffer[i-1];
						bytes_to_int.byte[0] = buffer[i+1];
						channel[FUEL] = bytes_to_int.entero;
						channel[DUTY] = (float)bytes_to_int.entero/625 * channel[RPM].toDouble()/1200;
						break;
					case 10:	 //fueladv
						bytes_to_int.byte[1] = buffer[i-1];
						bytes_to_degree.byte[0] = buffer.at(i+1);
						channel[FuelAdvance] = bytes_to_degree.byte[1]*6 + (float)bytes_to_degree.byte[0]*6/255;
					//				fuelAdvActual.angle = fuelAdvActual.byte[1]*6 + (float)fuelAdvActual.byte[0]*6/255;
						break;
					case 12:	 //Ignition Advance
						bytes_to_int.byte[1] = buffer[i-1];
						bytes_to_degree.byte[0] = buffer.at(i+1);
						channel[IgnAdvance] = bytes_to_degree.byte[1]*6 + (float)bytes_to_degree.byte[0]*6/255;
						break;
					case 13:
						channel[DWELL] = (float)buffer.at(i+1)/10;
						break;
					case 14:
						channel[waterTemp] = (float)buffer.at(i+1);
						break;
					case 15:
						auxiliarByteArray.setNum(buffer[i+1]);
						channel[THROTTLE] = auxiliarByteArray.toUShort();

					//					auxiliarByteArray.setNum(buffer[i+1]);
					//					throttle = auxiliarByteArray.toUShort();
					case 17:	 //LAMBDA
						bytes_to_int.byte[1] = buffer[i-1];
						bytes_to_int.byte[0] = buffer[i+1];
						channel[LAMBDA] = bytes_to_int.entero;
						break;
				}
		}
	}
	buffer.clear();
}


void commThread::read_FreeEMS_data()
{
	readPeriodicDataResponse();
}


void commThread::sendPeriodicDataRequest()
{
	aPacket packet;
	packet.setPacket("soy un paquete");
	//serial->write(packet.getPacket());
}


void commThread::readPeriodicDataResponse()
{
	//if( serial->bytesAvailable() > 700)
	{
		buffer = serial->read(serial->bytesAvailable());
		int index;


		QFile datalogFile("../Data Examples/comms20100421201222.bin");
		datalogFile.open(QIODevice::ReadOnly);
		buffer = datalogFile.readAll();
		datalogFile.close();

		//this Way I will lose bytes!
		// search occurrences of the end character (0xCC) from the beggining.
		while( (index = buffer.indexOf(0xCC, 0) ) != -1)
		{
			//Start from the end byte. Extract from the last occurence of a start byte, to the first occurence of the end byte
			//and pass that string to decodeFreeEMSPacket()
			int packetStart = buffer.lastIndexOf(0xAA,index);
			if(packetStart == -1)
				packetStart = 0;
			decodeFreeEMSPacket(buffer.mid(packetStart, index + 1 - packetStart));
//			qDebug()<<"buffer = "<<buffer.mid(packetStart, index + 1 - packetStart).toHex();
			buffer.remove(0,index+1);		//extract the decoded packet from the buffer.
		}
	}
}

void commThread::decodeFreeEMSPacket(QByteArray buffer)
{
	CoreVar *CoreVars = (CoreVar *)malloc(sizeof(CoreVar));
	DerivedVar *DerivedVars = (DerivedVar *)malloc(sizeof(DerivedVar));;
	ADCArray *ADCArrays = (ADCArray *)malloc(sizeof(ADCArray));;


	aPacket packet;
	if( !packet.setPacket(buffer) )
	{
		qDebug("bad packet");
		return;
	}
	qDebug("good packet");

	char *payload;
	char *payloadIndex;

	switch(packet.getPayloadID())
	{
		case responseBasicDatalog:
			payload = packet.getPayload().data();
			payloadIndex = payload;
			memcpy(CoreVars,payloadIndex,sizeof(CoreVar));
			payloadIndex += sizeof(CoreVar);
			memcpy(DerivedVars,payloadIndex,sizeof(DerivedVar));
			payloadIndex += sizeof(DerivedVar);
			memcpy(ADCArrays,payloadIndex,sizeof(ADCArray));

			channel[RPM] = CoreVars->RPM;
			channel[MAP] = CoreVars->MAP/512;
			channel[airTemp] = CoreVars->MAT/512;
			channel[THROTTLE] = CoreVars->TPS/512;
			channel[waterTemp] = CoreVars->CHT/512;
			channel[VE] = DerivedVars->VEMain/512;
			channel[FUEL] = DerivedVars->FinalPW/512;
			break;
		default :
			qDebug("Unknown packetID = %d", packet.getPayloadID());
			break;
	}
}

void commThread::sendFreeEMSDatalogRequest()
{
/*	aPacket packet;
	packet.setHeader(HEADER_HAS_LENGTH);
	packet.setPayloadID(requestBasicDatalog);
	packet.setChecksum();
	packet.addEscape();
	serial->write(packet.getPacket());*/
}


								 //puede acceder a todos los datos recibidos desde aqui
int commThread::getChannel(int address)
{   return channel[address].toInt();}

								 //member added for convenience
double commThread::getFChannel(int address)
{   return channel[address].toDouble();}

// upload configuration to flash
bool commThread::burnFlash(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		//	QMessageBox::warning(this, tr("Application"),tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return 1;
	}

	QTextStream in(&file);
	int i=0;
	while(!file.atEnd())
	{
		QByteArray line = file.readLine();
		if(hardwareTarget == "ePIC project")
			process_line(line,i);
		++i;
	}

	//file is loaded in ram, so its going to be sent

	serial->write("P",1);

	for(int i = 0; i<12; ++i)
		for(int j = 0; j<21; ++j)
	{
		plaindataVE.partido[11-i][j][1] = VEtable[i][j].byte[0];
		plaindataVE.partido[11-i][j][0] = VEtable[i][j].byte[1];
		IgnTable[i][j].byte[1] = (unsigned char)(IgnTable[i][j].angle/6);
		IgnTable[i][j].byte[0] = (unsigned char)((IgnTable[i][j].angle/6 - (float)IgnTable[i][j].byte[1])*255);
		plaindataIgn.partido[11-i][j][1] = IgnTable[i][j].byte[0];
		plaindataIgn.partido[11-i][j][0] = IgnTable[i][j].byte[1];

	}

	sendTimer = new QTimer(this);
	if(hardwareTarget == "ePIC project")
		connect(sendTimer, SIGNAL(timeout()), this, SLOT(sendFile()));
	sendTimer->start(20);
	byteCounter = 0;

	return 0;
}


void commThread::process_line(QByteArray line,int i)
{
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

	}

	if(i==32)
	{
		sscanf(line,"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f",   &(fuelAdv[0].angle),
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
		sscanf(line,"%d",&(confParameter.fuelCut));
	if(i==43)
		sscanf(line,"%d",&(confParameter.idleRPM));
	if(i==45)
		sscanf(line,"%d",&(confParameter.launchRPM));
	if(i==47)
		sscanf(line,"%d",&(confParameter.injSize));
	if(i==49)
		sscanf(line,"%d",&(confParameter.deadTime));

	if(i>=52 && i <=64)			 //64?
	{
		int nada;
		sscanf(line,"%d\t%f", &nada,&(confParameter.tempCorr[i-52]));
	}

	if(i==67)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%d",rockandrollnenene,&(confParameter.nitrousRPM));
	}
	if(i==68)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%d",rockandrollnenene,&(confParameter.nitrousMAP));
	}
	if(i==69)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%d",rockandrollnenene,&(confParameter.nitrousTPS));
	}
	if(i==72)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage1Flow));
	}
	if(i==73)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage1Retard));
	}
	if(i==76)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage2Flow));
	}
	if(i==77)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage2Retard));
	}
	if(i==80)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage3Flow));
	}
	if(i==81)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%f",rockandrollnenene,&(confParameter.nitrousStage3Retard));
	}

	if(i==86)
	{
		char rockandrollnenene[15];
		char source[16];
		sscanf(line,"%[^\t]\t%s",rockandrollnenene,source);
		if(strcmp(source, "MAP"))
			confParameter.accel_pump_source = MAPsourced;
		if(strcmp(source, "TPS"))
			confParameter.accel_pump_source = TPSsourced;
	}
	if(i==87)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%c",(char*)rockandrollnenene,&(confParameter.accel_pump_treshold));
	}
	if(i==88)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%c",(char*)rockandrollnenene,&(confParameter.accel_pump_decay));
	}
	if(i==89)
	{
		char rockandrollnenene[15];
		sscanf(line,"%[^\t]%c",(char*)rockandrollnenene,&(confParameter.accel_pump_enrich_factor));
	}
}


void commThread::sendFile()
{								 //	sendingBar->setValue(byteCounter);
	if(byteCounter < 504 )
	{
		const char dato = plaindataVE.plain[byteCounter];
		serial->write(&dato,1);
	}
	else
	{
		if(byteCounter < 1008 )
		{
			byteCounter -= 504;
			const char dato = plaindataIgn.plain[byteCounter];
			serial->write(&dato,1);
			byteCounter += 504;
		}
		else
		{
			if(byteCounter == 1008)
			{
				const char dato = 150;
				serial->write(&dato,1);
			}
			else
			{
				byteCounter -= 1009;
				if(byteCounter < 24)
				{
					if( byteCounter % 2 == 0)
					{
						const char dato = fuelAdv[byteCounter/2].byte[1];
						serial->write(&dato,1);
					}
					else
					{
						const char dato = fuelAdv[byteCounter/2].byte[0];
						serial->write(&dato,1);
					}
				}
				byteCounter += 1009;
			}
		}
	}

	if(byteCounter == 1033)
	{
		bytes_to_int.entero = confParameter.displacement;
		const char dato = bytes_to_int.byte[1];
		serial->write(&dato,1);
	}
	if(byteCounter == 1034)
	{
		bytes_to_int.entero = confParameter.displacement;
		const char dato = bytes_to_int.byte[0];
		serial->write(&dato,1);
	}

	if(byteCounter == 1035)
	{
		bytes_to_int.entero = confParameter.deadTime;
		const char dato = bytes_to_int.byte[1];
		serial->write(&dato,1);
	}

	if(byteCounter == 1036)
	{
		const char dato = bytes_to_int.byte[0];
		serial->write(&dato,1);
	}

	if(byteCounter == 1037)
	{
		bytes_to_int.entero = confParameter.injSize;
		const char dato = bytes_to_int.byte[1];
		serial->write(&dato,1);
	}
	if(byteCounter == 1038)
	{
		const char dato = bytes_to_int.byte[0];
		serial->write(&dato,1);
	}

	if(byteCounter == 1039)
	{
		bytes_to_int.entero = confParameter.fuelCut;
		const char dato = bytes_to_int.byte[1];
		serial->write(&dato,1);
	}
	if(byteCounter == 1040)
	{
		const char dato = bytes_to_int.byte[0];
		serial->write(&dato,1);
	}

	if(byteCounter == 1041)
	{
		bytes_to_int.entero = confParameter.idleRPM;
		const char dato = bytes_to_int.byte[1];
		serial->write(&dato,1);
	}
	if(byteCounter == 1042)
	{
		const char dato = bytes_to_int.byte[0];
		serial->write(&dato,1);
	}

	if(byteCounter == 1043)
	{
		bytes_to_int.entero = confParameter.launchRPM;
		const char dato = bytes_to_int.byte[1];
		serial->write(&dato,1);
	}
	if(byteCounter == 1044)
	{
		const char dato = bytes_to_int.byte[0];
		serial->write(&dato,1);
	}

	if(byteCounter == 1045)
	{
		const char dato = confParameter.accel_pump_enrich_factor;
		serial->write(&dato,1);
	}

	if(byteCounter >= 1046 && byteCounter < 1072 )
	{
		byteCounter -= 1046;
		if( byteCounter % 2 == 0)
		{
			bytes_to_int.entero = (int)(confParameter.tempCorr[byteCounter/2] * 100000);
			const char dato = bytes_to_int.byte[1];
			serial->write(&dato,1);
		}
		else
		{
			const char dato = bytes_to_int.byte[0];
			serial->write(&dato,1);
		}
		byteCounter += 1046;
	}

	if( byteCounter == 1072 )
	{
		;						 //	const char dato = confParameter.accel_pump_decay * 255 / 100;	//adapto el 0-100 a 0-255
		//		serial->write(&dato,1);
	}

	if( byteCounter == 1073 )
	{
		;						 //	const char dato = confParameter.accel_pump_treshold;
		//		serial->write(&dato,1);
	}

	if( byteCounter == 1080 )
	{
		sendTimer->~QTimer();	 //Destroy timer!
		exit();
		//	statusBar()->removeWidget(sendingBar);
	}

	++byteCounter;
}


void commThread::startLogging()
{
	logging = true;
	tempLogFile = new QFile("templog.log");
	tempLogFile->open(QIODevice::WriteOnly);
}


void commThread::saveLog(QString filename)
{
	if(logging)
	{
		if(filename.isEmpty())
			tempLogFile->remove();
		else
		{
			tempLogFile->rename("templog.log",filename);
			tempLogFile->close();
		}
		logging = false;
	}
}
