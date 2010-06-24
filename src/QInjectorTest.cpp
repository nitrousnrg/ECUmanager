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

#include"QInjectorTest.h"

QInjTester::QInjTester(QextSerialPort *port)
{
	serial = port;
	setWindowTitle(tr("Injector tester"));
	move(380,80);
	//setGeometry(300,100,500,320);
	timeCount = 50;
	flowTimer = new QTimer(this);
	flowTimer->setSingleShot(true);
	pulseTimer = new QTimer(this);
	pulseTimer->setSingleShot(true);

	LCDFlowTimer = new QTimer(this);
	LCDPulseTimer = new QTimer(this);

	QGridLayout *grid = new QGridLayout;

	QTextEdit *editFlowHelp = new QTextEdit(tr("Choose test duration. Pressing\n"
		"the start button, the injector will "
		"remain open for the selected time, or until stop button is pressed."));
	editFlowHelp->setReadOnly(true);

	QLabel *flowDurationLabel = new QLabel(tr("Duration (ms):     "));
	editFlowDuration = new QLineEdit("1000");
	editFlowDuration->setAlignment(Qt::AlignRight);

	flowTimeLCD = new QLCDNumber(6);
	flowTimeLCD->setSegmentStyle(QLCDNumber::Filled);

	startFlowButton = new QPushButton(tr("Start"));
	stopFlowButton = new QPushButton(tr("Stop"));

	connect(startFlowButton,SIGNAL(released()),this,SLOT(startFlow()));
	connect(stopFlowButton,SIGNAL(released()),this,SLOT(stopFlow()));
	connect(flowTimer,SIGNAL(timeout()),this,SLOT(stopFlow()));
	connect(LCDFlowTimer,SIGNAL(timeout()),this,SLOT(displayFlowLCD()));

	QGroupBox *groupBox2 = new QGroupBox(tr("Flow Test"));
	QGridLayout *vbox2 = new QGridLayout;
	vbox2->addWidget(editFlowHelp,0,0);
	vbox2->addWidget(flowTimeLCD,1,0);
	vbox2->addWidget(flowDurationLabel,0,1);
	vbox2->addWidget(editFlowDuration,0,2);
	vbox2->addWidget(startFlowButton,1,1);
	vbox2->addWidget(stopFlowButton,1,2);
	groupBox2->setLayout(vbox2);

	QTextEdit *editPulseHelp = new QTextEdit(tr("Elija la duracion de la prueba (ms).\n"
		"Cuando pulse el boton Start,los inyectores se abriran el durante el tiempo elegido"
		" y no se cerraran hasta que termine la prueba o se presione el boton Stop."));
	editPulseHelp->setReadOnly(true);

	QLabel *pulseDurationLabel = new QLabel(tr("Duration (ms):     "));
	editPulseDuration = new QLineEdit("1000");
	editPulseDuration->setAlignment(Qt::AlignRight);

	QLabel *pulseONLabel = new QLabel(tr("ON time (ms):     "));
	editPulseON = new QLineEdit("2.5");
	editPulseON->setAlignment(Qt::AlignRight);

	QLabel *pulseOFFLabel = new QLabel(tr("OFF time (ms):     "));
	editPulseOFF = new QLineEdit("2.5");
	editPulseOFF->setAlignment(Qt::AlignRight);

	ONtime.entero = 2500;
	OFFtime.entero = 2500;

	pulseTimeLCD = new QLCDNumber(6);
	pulseTimeLCD->setSegmentStyle(QLCDNumber::Filled);

	pulseBar = new QProgressBar;
	pulseBar->setRange(0,5000);
	pulseBar->setValue(2500);

	startPulseButton = new QPushButton(tr("Start"));
	stopPulseButton = new QPushButton(tr("Stop"));

	connect(startPulseButton,SIGNAL(released()),this,SLOT(startPulse()));
	connect(stopPulseButton,SIGNAL(released()),this,SLOT(stopPulse()));
	connect(editPulseON,SIGNAL(textChanged(const QString)),this,SLOT(editONChange(const QString )));
	connect(editPulseOFF,SIGNAL(textChanged(const QString)),this,SLOT(editOFFChange(const QString )));
	connect(pulseTimer,SIGNAL(timeout()),this,SLOT(stopPulse()));
	connect(LCDPulseTimer,SIGNAL(timeout()),this,SLOT(displayPulseLCD()));

	QGroupBox *groupBox1 = new QGroupBox(tr("Pulse Test"));
	QGridLayout *vbox = new QGridLayout;
	vbox->addWidget(editPulseHelp,0,0,5,1);
	vbox->addWidget(pulseTimeLCD,6,0);
	vbox->addWidget(pulseDurationLabel,0,1);
	vbox->addWidget(editPulseDuration,0,2);
	vbox->addWidget(pulseONLabel,1,1);
	vbox->addWidget(editPulseON,1,2);
	vbox->addWidget(pulseOFFLabel,2,1);
	vbox->addWidget(editPulseOFF,2,2);
	vbox->addWidget(pulseBar,4,1,1,2);
	vbox->addWidget(startPulseButton,6,1);
	vbox->addWidget(stopPulseButton,6,2);
	groupBox1->setLayout(vbox);

	grid->addWidget(groupBox2,0,0,1,2);
	grid->addWidget(groupBox1,1,0,1,2);

	setLayout(grid);
}


QInjTester::~QInjTester()
{
	delete serial;
	delete flowTimer;
	delete LCDFlowTimer;
	delete LCDPulseTimer;
	delete startFlowButton;
	delete stopFlowButton;
	delete startPulseButton;
	delete stopPulseButton;
	delete editFlowDuration;
	delete editPulseDuration;
	delete editPulseON;
	delete editPulseOFF;
	delete flowTimeLCD;
	delete pulseTimeLCD;
}


void QInjTester::startFlow()
{
	timeCount = 50;
	QString string1 = editFlowDuration->text();
	bool ok;
	string1.toInt(&ok,10);
	if(ok)
	{
		flowTimer->setInterval(string1.toUInt());
		flowTimer->start();
		LCDFlowTimer->start(50);
		serial->write("I");
	}
}


void QInjTester::stopFlow()
{
	if(timeCount != 50)
	{
		serial->write("I");
		LCDFlowTimer->stop();
		timeCount = 50;
	}
	//close();
}


void QInjTester::editONChange(const QString text)
{
	bool ok;
	text.toFloat(&ok);
	if(ok)
	{
		ONtime.entero = (int)(text.toFloat() * 1000);
		pulseBar->setRange(0,ONtime.entero + OFFtime.entero);
		pulseBar->setValue(ONtime.entero);
	}
}


void QInjTester::editOFFChange(const QString text)
{
	bool ok;
	text.toFloat(&ok);
	if(ok)
	{
		OFFtime.entero = (int)(text.toFloat() * 1000);
		pulseBar->setRange(0,ONtime.entero + OFFtime.entero);
		pulseBar->reset();
		pulseBar->setValue(ONtime.entero);
	}
}


void QInjTester::displayFlowLCD()
{
	timeCount += 50;
	flowTimeLCD->display(timeCount);
}


void QInjTester::displayPulseLCD()
{
	timeCount += 50;
	pulseTimeLCD->display(timeCount);
}


void QInjTester::startPulse()
{
	timeCount = 50;
	QString string1 = editPulseDuration->text();
	bool ok;
	string1.toInt(&ok,10);
	if(ok)
	{
		pulseTimer->setInterval(string1.toUInt());
		pulseTimer->start();
		LCDPulseTimer->start(50);

		QByteArray word("i");

		aux.entero = 65536 - ONtime.entero*5;
		word.append(aux.byte[1]);
		word.append(aux.byte[0]);

		aux.entero = 65536 - OFFtime.entero*5;
		word.append(aux.byte[1]);
		word.append(aux.byte[0]);

		serial->write(word);
	}
}


void QInjTester::stopPulse()
{
	if(timeCount != 50)
	{
		serial->write("i");
		LCDPulseTimer->stop();
		timeCount = 50;
	}
	//close();
}


QSize QInjTester::minimumSizeHint() const
{
	return QSize(30, 30);
}


QSize QInjTester::sizeHint() const
{
	return QSize(500,700);
}
