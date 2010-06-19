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
#ifndef QINJECTORTEST_H
#define QINJECTORTEST_H

#include <QWidget>
#include <QTimer>
#include <qextserialport.h>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QLCDNumber>
#include <QProgressBar>
#include <QByteArray>

 
 class QWidget;

class QInjTester : public QWidget
{
	Q_OBJECT

public:
	QInjTester(QextSerialPort *port);
	~QInjTester();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;


protected:

private slots:
	void startFlow();
	void stopFlow();
	void startPulse();
	void stopPulse();
	void displayFlowLCD();
	void displayPulseLCD();
	void editONChange(const QString text);
	void editOFFChange(const QString text);

private:
	int timeCount;
	QextSerialPort *serial;
	QTimer *flowTimer;
	QTimer *pulseTimer;
	QTimer *LCDFlowTimer;
	QTimer *LCDPulseTimer;
	QPushButton *startFlowButton;
	QPushButton *stopFlowButton;
	QPushButton *startPulseButton;
	QPushButton *stopPulseButton;
	QLineEdit *editFlowDuration;
	QLineEdit *editPulseDuration;
	QLineEdit *editPulseON;
	QLineEdit *editPulseOFF;
	QLCDNumber *flowTimeLCD;
	QLCDNumber *pulseTimeLCD;
	QProgressBar *pulseBar;

	union  transformer
	{
		unsigned char byte[2];
		int entero;
	}ONtime,OFFtime,aux;
 };

 #endif
