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
#ifndef COMM_THREAD_H
#define COMM_THREAD_H

/**
commthread se encarga de todos los aspectos de la comunicacion con el hardware.

	@author marcos chaparro <nitrousnrg@gmail.com>
*/

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QTimer>
#include <QFile>
#include <QVariant>
#include <QMessageBox>
#include <qextserialport.h>

//#include "commCore_FreeEMS.h"

#define MAPsourced 0
#define TPSsourced 1

class aPacket
{
	public:
		aPacket();
		void setPacket(QByteArray);
		QByteArray getPacket();
		void setHeaderAckFlag(bool);
		bool hasHeaderAckFlag();
		void setHeaderLengthFlag(bool);
		bool hasHeaderLengthFlag();
		void setPayloadID(int);
		int  getPayloadID();
		void setPayload(QByteArray payload);
		QByteArray getPayload();
		void setPayloadLength(int length);
		int  getPayloadLength();
		int  getCalculatedPayloadLength();
		//   unsigned char checksum(QByteArray data);
		void buildPacket();
		void preparePacket();	 //to send through rs232
		void unpreparePacket();	 //to be parsed after rs232
		void addEscape();

	private:
		unsigned char headerFlags;
		unsigned char checksum;
		int payloadID;
		unsigned int payloadLength;
		QByteArray payload;
		QByteArray fullPacket;

};

class commThread : public QThread
{
	Q_OBJECT
		public:
		commThread(){serial=0;logging=0;};
		void run();
								 //needed before run()
		void setPort(QString portAddress);
		bool burnFlash(QString fileName);
								 //needed before run() too
		void setTarget(QString target)
		{
			hardwareTarget = target;
		}
								 //puede acceder a todos los datos recibidos desde aqui
		int getChannel(int channel);
		QextSerialPort * getQextSerialPort(){return serial;};
		bool isOnline(){return online;};
		void replayLog(QString fileName);
		void startLogging();
		void saveLog(QString filename);
		//bool isSerial();
		double getFChannel(int channel);

		enum channelList{RPM,MAP,VE,FUEL,DUTY,FuelAdvance,IgnAdvance,waterTemp,airTemp,DWELL,THROTTLE,LAMBDA};

	public slots:
		void readSerialPort();
		void sendFile();
		//	bool burnAcachoOfFlash();

		/*FreeEMS stuff */
		void getInterfaceVersion();
		void getFirmwareVersion();
		void readFreeEMS();

		signals:
		void packetArrived(QByteArray);

	private:
		void read_PIC_data();
		void read_FreeEMS_data();
		void process_line(QByteArray line,int i);
		bool online;

		QVariant channel[20];	 //as big as you want
		QByteArray buffer, auxiliarByteArray;

		QextSerialPort *serial;
		QFile *logFile;
		QTimer *timer;
		QTimer *sendTimer;
		QMutex mutex;
		QFile *tempLogFile;
		QString serialPort;
		QString binaryFileName;
		bool isSerial;
		bool logging;
		QString hardwareTarget;
		int byteCounter;
		int available;
		char logbuffer[4];

		union  transformer
		{
			unsigned char byte[2];
			int entero;
		}bytes_to_int,VEtable[12][21];
		struct degreeConverter
		{
			unsigned char byte[2];
			float angle;
		}bytes_to_degree,IgnTable[12][21],fuelAdv[12];

		union zocotroco_de_numeros
		{
			unsigned char partido[12][21][2];
			char plain[504];
		}plaindataVE,plaindataIgn;

		struct estructura
		{
			unsigned int cylinders;
								 //cm
			unsigned int displacement;
			unsigned int fuelCut;//rpm
			unsigned int idleRPM;
			unsigned int launchRPM;
			unsigned int injSize;//cm/min
								 //us
			unsigned int deadTime;

			unsigned char accel_pump_source;
			unsigned char accel_pump_treshold;
			unsigned char accel_pump_decay;
			unsigned char accel_pump_enrich_factor;

			float tempCorr[13];	 //enrich %
			//float dwell;

			unsigned int nitrousRPM;
			unsigned int nitrousMAP;
			unsigned int nitrousTPS;
			float nitrousStage1Flow;
			float nitrousStage2Flow;
			float nitrousStage3Flow;
			float nitrousStage1Retard;
			float nitrousStage2Retard;
			float nitrousStage3Retard;
			//boost ctrl
		}confParameter;
};
#endif
