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

#include "qt4application.h"

/* create() members	*/

void qt4application::createVEtable()
{
	int i;

	QStringList RPMheader;
	QStringList MAPheader;
	QString num;
//	VEtableItem = new QTableWidgetItem[25][25];
	headerTableRPM = new QTableWidget(1,21);
	headerTableMAP = new QTableWidget(1,21);
	headerContentsRPM = new QTableWidgetItem[21];
	headerContentsMAP = new QTableWidgetItem[21];   VE_table = new QTableWidget(12, 21, this);
								 //This enables the right click pop up
	VE_table->setContextMenuPolicy(Qt::CustomContextMenu);
	VE_table_ON = true;
	connect(VE_table,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(setMainTableSize()));

	for(i=0;i<12;++i)
	{
		VE_table->setRowHeight(i,19);
		MAPheader.prepend(num.setNum(confParameter.headerMAP[i]));
	}
	for(i=0;i<21;++i)
	{
		VE_table->setColumnWidth(i,45);
		RPMheader.append(num.setNum(confParameter.headerRPM[i]));
	}

	VE_table->setHorizontalHeaderLabels ( RPMheader );
	VE_table->setVerticalHeaderLabels ( MAPheader );
	VE_table->setMaximumHeight( 275 );

	connect(VE_table,SIGNAL(itemChanged(QTableWidgetItem * )),this, SLOT(checkChange(QTableWidgetItem *)));
}


void qt4application::createLinearTable()
{
	fuelAdvTable = new QTableWidget(1, 12, this);
	for(int column = 0;column<12;++column)
	{
		fuelAdvTableItem[column].setTextAlignment(Qt::AlignVCenter);
		fuelAdvTableItem[column].setData(0, QVariant( 0 ));
		fuelAdvTable->setItem(0,column,&(fuelAdvTableItem[column]));
		fuelAdvTable->setColumnWidth(column,48);
	}
	fuelAdvTable->hide();
	fuelAdvTable->setRowHeight(0,19);
	fuelAdvTable->setRowHeight(1,24);

	QStringList Hheader;
	Hheader<<"0"<<"1000"<<"2000"<<"3000"<<"4000"<<"5000"<<"6000"<<"7000"<<"8000"<<"9000"<<"10000"<<"11000";
	fuelAdvTable->setHorizontalHeaderLabels ( Hheader );
	fuelAdvTable->setMaximumWidth( 596 );

	connect(fuelAdvTable,SIGNAL(itemChanged(QTableWidgetItem * )),this, SLOT(checkChange(QTableWidgetItem *)));
}


void qt4application::createLayout()
{
	QVBoxLayout *cornerLayout = new QVBoxLayout;
	cornerLayout->addWidget(plot);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(heavyDataLabel);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(VElabel);
	mainLayout->addWidget(VEbar);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(MAPlabel);
	mainLayout->addWidget(MAPbar);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(AirTemplabel);
	mainLayout->addWidget(AirTempbar);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(TEMPlabel);
	mainLayout->addWidget(TEMPbar);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(AirFuellabel);
	mainLayout->addWidget(AirFuelbar);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(throttleLabel);
	mainLayout->addWidget(throttleBar);
	mainLayout->addSpacing(15);
	mainLayout->addWidget(Dutylabel);
	mainLayout->addWidget(Dutybar);
	mainLayout->addWidget(InyTimelabel);
	mainLayout->addWidget(InyTimebar);
	mainLayout->addSpacing(20);
	mainLayout->addLayout(cornerLayout);

	QmainLayout = new QWidget;
	QmainLayout->setLayout(mainLayout);
	QmainLayout->setMaximumHeight( 180 );

	horizontaLayout = new QHBoxLayout;
	//	horizontaLayout->addWidget(tablesLabels);
	horizontaLayout->addWidget(map3D);
	horizontaLayout->addWidget(VE_table);
	horizontaLayout->addWidget(fuelAdvTable);
	horizontaLayout->addWidget(renderArea);

	QVBoxLayout *verticalLayout = new QVBoxLayout;
	verticalLayout->addWidget(QmainLayout);
	verticalLayout->addLayout(horizontaLayout);

	window = new QWidget;
	window->setLayout(verticalLayout);

	setCentralWidget(window);

}


void qt4application::createLabels()
{
	QFont font("Helvetica", 11, QFont::Bold);

	heavyDataLabel = new QLabel(this);
	heavyDataLabel->setFont(font);
	heavyDataLabel->setText("Ign Adv:\nDwell:\n\nFuel Adv:\nCorrections:");

	VElabel = new QLabel(this);
	VElabel->setText("VE");
	VElabel->setAlignment(Qt::AlignCenter |Qt::AlignRight);

	MAPlabel = new QLabel(this);
	//	MAPlabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	MAPlabel->setText("MAP");
	MAPlabel->setAlignment(Qt::AlignCenter |Qt::AlignRight);

	TEMPlabel = new QLabel(this);
	TEMPlabel->setText("Water\nTemp");
	TEMPlabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);

	AirFuellabel = new QLabel(this);
	AirFuellabel->setText("A/F");
	AirFuellabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);

	throttleLabel = new QLabel(this);
	throttleLabel->setText("Throttle");
	throttleLabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);

	InyTimelabel = new QLabel(this);
	InyTimelabel->setText("Iny Time");
	InyTimelabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);

	AirTemplabel = new QLabel(this);
	AirTemplabel->setText("Air\nTemp");
	AirTemplabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);

	Dutylabel = new QLabel(this);
	Dutylabel->setText("Duty");
	Dutylabel->setAlignment(Qt::AlignCenter | Qt::AlignRight);

	/*	tablesLabels = new QLabel(this);
		tablesLabels->setFont(font);
		tablesLabels->setText("VE map");
			tablesLabels->setAlignment(Qt::AlignRight);*/
}


void qt4application::createBars()
{
	VEbar = new QProgressBar(this);
	VEbar->setTextVisible(FALSE);
	VEbar->setRange(0,120);
	VEbar->show();
	VEbar->setOrientation(Qt::Vertical);

	MAPbar= new QProgressBar(this);
	//	MAPbar->setRange(10,120);
	MAPbar->setTextVisible(FALSE);
	MAPbar->show();
	MAPbar->setOrientation(Qt::Vertical);

	TEMPbar= new QProgressBar(this);
	TEMPbar->setRange(0,110);
	TEMPbar->setTextVisible(FALSE);
	TEMPbar->show();
	TEMPbar->setOrientation(Qt::Vertical);

	//Alarm!
	/*
		QPalette TEMPpalette = TEMPbar->palette();
		TEMPpalette.setColor(QPalette::Base,QColor(255,0,0,90));
		TEMPpalette.setColor(QPalette::Window,Qt::red);
		TEMPbar->setPalette(TEMPpalette);*/

	throttleBar = new QProgressBar(this);
	throttleBar->setRange(0,100);
	throttleBar->setTextVisible(FALSE);
	throttleBar->show();
	throttleBar->setOrientation(Qt::Vertical);

	AirFuelbar= new QProgressBar(this);
	if(strcmp(oxygenSensor,"Wideband"))
		AirFuelbar->setRange(0,1024);
	if(strcmp(oxygenSensor,"Narrowband"))
		AirFuelbar->setRange(0,206);
	if(strcmp(oxygenSensor,"Not Available"))
		AirFuelbar->setRange(0,1024);

	AirFuelbar->setTextVisible(FALSE);
	AirFuelbar->show();
	AirFuelbar->setOrientation(Qt::Vertical);

	InyTimebar= new QProgressBar(this);
	InyTimebar->setRange(0,20);
	InyTimebar->setTextVisible(FALSE);
	InyTimebar->show();
	InyTimebar->setOrientation(Qt::Vertical);

	Dutybar= new QProgressBar(this);
	//Dutybar->setRange(0,300);
	Dutybar->setTextVisible(FALSE);
	Dutybar->show();
	Dutybar->setOrientation(Qt::Vertical);

	AirTempbar= new QProgressBar(this);
	AirTempbar->setRange(0,50);	 //-5C is healthier
	AirTempbar->setTextVisible(FALSE);
	AirTempbar->show();
	AirTempbar->setOrientation(Qt::Vertical);

}


void qt4application::createActions()
{

	refreshTimer = new QTimer(this);
	connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
	refreshTimer->setInterval(80);

	connectAct = new QAction(QIcon(":/network-wired.png"),tr("&Connect ECU"), this);
	connectAct->setStatusTip(tr("Starts communication with ECU"));
	connect(connectAct, SIGNAL(triggered()), this, SLOT(connectECU()));

	disconnectAct = new QAction(QIcon(":/network-wired.png"),tr("&Disconnect ECU"), this);
	disconnectAct->setStatusTip(tr("Stop communications"));
	disconnectAct->setDisabled(true);
	connect(disconnectAct, SIGNAL(triggered()), this, SLOT(disconnectECU()));

	fetchAct = new QAction(tr("Open ECU"), this);
	fetchAct->setStatusTip(tr("Reads config file from ECU"));
	fetchAct->setDisabled(true);
	connect(fetchAct, SIGNAL(triggered()), this, SLOT(openECU()));

	sendfileAct = new QAction(QIcon(":/amarok_burn.png"),tr("Send File"),this);
	sendfileAct->setStatusTip(tr("Send config file to ECU"));
	sendfileAct->setDisabled(true);
	connect(sendfileAct, SIGNAL(triggered()),this,SLOT(sendFilesInit()));

	upgradeFirmwareAct = new QAction(tr("Firmware upgrade"),this);
	upgradeFirmwareAct->setStatusTip(tr("Reboot ECU with brand new firmware"));
	upgradeFirmwareAct->setDisabled(true);
	connect(upgradeFirmwareAct, SIGNAL(triggered()),this,SLOT(upgradeFirmware()));

	adjustParametersAct = new QAction(tr("&Parameters"), this);
	adjustParametersAct->setStatusTip(tr("Shows dialog to set param"));
	connect(adjustParametersAct, SIGNAL(triggered()),this,SLOT(parametersDialog()));

	adjustIgnitionAct = new QAction(tr("Ignition Table"), this);
	adjustIgnitionAct->setStatusTip(tr("Shows ign map for calibration"));
	connect(adjustIgnitionAct, SIGNAL(triggered()),this,SLOT(showIgnition()));

	adjustIgnTempCorrectionAct = new QAction(tr("Temp Corr"), this);
	adjustIgnTempCorrectionAct->setStatusTip(tr("Engine temp advance/retard"));
	connect(adjustIgnTempCorrectionAct, SIGNAL(triggered()),this,SLOT(tempDialog()));

	adjustFuelAct = new QAction(tr("VE Table"), this);
	adjustFuelAct->setStatusTip(tr("Shows VE map for calibration"));
	connect(adjustFuelAct, SIGNAL(triggered()),this,SLOT(showFuel()));

	adjustFuelTimingAct = new QAction(tr("Timing"), this);
	adjustFuelTimingAct->setStatusTip(tr("Timing calibration (BTDC)"));
	connect(adjustFuelTimingAct, SIGNAL(triggered()),this,SLOT(showFuelTiming()));

	adjustTempCorrectionAct = new QAction(tr("Temp Corr"), this);
	adjustTempCorrectionAct->setStatusTip(tr("Engine temp enrichment"));
	connect(adjustTempCorrectionAct, SIGNAL(triggered()),this,SLOT(tempDialog()));

	adjustAccelEnrichmentAct = new QAction(tr("Accel Enrichment"), this);
	adjustAccelEnrichmentAct->setStatusTip(tr("Engine enrichment under acceleration"));
	connect(adjustAccelEnrichmentAct, SIGNAL(triggered()),this,SLOT(accelDialog()));

	adjustAlarmsAct = new QAction(tr("Alarms"), this);
	//adjustAlarmsAct->setStatusTip(tr("Shows dialog for calibration"));
	connect(adjustAlarmsAct, SIGNAL(triggered()),this,SLOT(alarmsDialog()));

	adjustBoostCtrlAct = new QAction(tr("Boost Ctrl"), this);
	adjustBoostCtrlAct->setDisabled(true);
	adjustBoostCtrlAct->setStatusTip(tr("Shows dialog for calibration"));
	connect(adjustBoostCtrlAct, SIGNAL(triggered()),this,SLOT(boostDialog()));

	adjustNitrousAct = new QAction(tr("Nitrous"), this);
	adjustNitrousAct->setStatusTip(tr("Shows dialog for calibration"));
	connect(adjustNitrousAct, SIGNAL(triggered()),this,SLOT(nitrousDialog()));

	adjustPWM1 = new QAction(tr("PWM 1"), this);
	adjustPWM1->setDisabled(true);
	adjustPWM1->setStatusTip(tr("Shows dialog for output 1 calibration"));
	connect(adjustPWM1, SIGNAL(triggered()),this,SLOT(nitrousDialog()));

	adjustPWM2 = new QAction(tr("PWM 2"), this);
	adjustPWM2->setDisabled(true);
	adjustPWM2->setStatusTip(tr("Shows dialog for output 2 calibration"));
	connect(adjustPWM2, SIGNAL(triggered()),this,SLOT(nitrousDialog()));

	view1Act = new QAction(tr("Std Arrange"), this);
	view1Act->setDisabled(true);
	view1Act->setStatusTip(tr("Recommended Layout"));
	connect(view1Act, SIGNAL(triggered()),this,SLOT(nitrousDialog()));

	startLogAct = new QAction(tr("Start Logging"), this);
	startLogAct->setDisabled(true);
	startLogAct->setStatusTip(tr("Log engine data"));
	connect(startLogAct, SIGNAL(triggered()),this,SLOT(startLog()));

								 // if logenabled = false -> disabed!
	saveLogAct = new QAction(QIcon(":/amarok_burn.png"), tr("Save Log"), this);
	saveLogAct->setDisabled(true);
	saveLogAct->setStatusTip(tr("Ends log & save"));
	connect(saveLogAct, SIGNAL(triggered()),this,SLOT(saveLog()));

	printAct = new QAction(tr("Print"), this);
	printAct->setShortcut(tr("Ctrl+P"));
	//printAct->setDisabled(true);
	//printAct->setStatusTip(tr("Prin"));
	connect(printAct, SIGNAL(triggered()),this,SLOT(printDialog()));

	replayAct = new QAction(QIcon(":/amarok_play.png"),tr("Replay"), this);
	replayAct->setStatusTip(tr("Reproduces logged file"));
	connect(replayAct, SIGNAL(triggered()),this,SLOT(replay()));

	quickTuneAct = new QAction(tr("QuickTune"), this);
	quickTuneAct->setStatusTip(tr("Simplistic tunning method"));
	connect(quickTuneAct, SIGNAL(triggered()),this,SLOT(quickTune()));

	injectorTestAct = new QAction(tr("Injector Test"), this);
	injectorTestAct->setDisabled(true);
	injectorTestAct->setStatusTip(tr("Force ECU to generate injection pulses"));
	connect(injectorTestAct, SIGNAL(triggered()),this,SLOT(injectorTestDialog()));

	viewAct = new QAction(QIcon(":/amarok_visualizations.png"), tr("QViewer"), this);
	viewAct->setStatusTip(tr("Open data viewing tool"));
	connect(viewAct, SIGNAL(triggered()),this,SLOT(viewData()));

	configureECUManagerAct = new QAction(QIcon(":/amarok_configure.png"), tr("Configure manager"), this);
	configureECUManagerAct->setStatusTip(tr("Engine temp enrichment"));
	connect(configureECUManagerAct, SIGNAL(triggered()),this,SLOT(configureECUmanager()));

	newAct = new QAction(QIcon(":/filenew.xpm"), tr("&New"), this);
	newAct->setShortcut(tr("Ctrl+N"));
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

	openAct = new QAction(QIcon(":/fileopen.xpm"), tr("&Open..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/filesave.xpm"), tr("&Save"), this);
	saveAct->setShortcut(tr("Ctrl+S"));
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	/*    cutAct = new QAction(QIcon(":/editcut.xpm"), tr("Cu&t"), this);
		  cutAct->setShortcut(tr("Ctrl+X"));
		  cutAct->setStatusTip(tr("Cut the current selection's contents to the "
								  "clipboard"));
		  connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

		  copyAct = new QAction(QIcon(":/editcopy.xpm"), tr("&Copy"), this);
		  copyAct->setShortcut(tr("Ctrl+C"));
		  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
								  "clipboard"));
		  connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

		  pasteAct = new QAction(QIcon(":/editpaste.xpm"), tr("&Paste"), this);
		  pasteAct->setShortcut(tr("Ctrl+V"));
		  pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
								  "selection"));
		  connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));
	*/

	connectorPinoutAct = new QAction(tr("&Pinout"), this);
	//connectorPinoutAct->setStatusTip(tr("Show the application's About box"));
	//connect(connectorPinoutAct, SIGNAL(triggered()), this, SLOT(connectorPinout()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	/* FreeEMS stuff */

	sendResetAct = new QAction(tr("Soft &Reset"), this);
	sendResetAct->setStatusTip(tr("Instructs the device to software reset itself"));
	sendResetAct->setDisabled(true);
	connect(sendResetAct, SIGNAL(triggered()), this, SLOT(sendReset()));

	getInterfaceVersionAct = new QAction(tr("&Interface Version"), this);
	getInterfaceVersionAct->setDisabled(true);

	getFirmwareVersionAct = new QAction(tr("&Firmware Version"), this);
	getFirmwareVersionAct->setDisabled(true);

	sendDatalogRequestAct = new QAction(tr("Request &Datalog"),this);
	sendDatalogRequestAct->setStatusTip(tr("Start the data logging of all EMS variables"));
	sendDatalogRequestAct->setDisabled(true);
	connect(sendDatalogRequestAct, SIGNAL(triggered()), this, SLOT(sendFreeEMSDatalogRequest()));


	openDebugWindowAct = new QAction(tr("De&bug"), this);
	openDebugWindowAct ->setStatusTip(tr("Open debug window"));
	openDebugWindowAct->setDisabled(true);
	connect(openDebugWindowAct, SIGNAL(triggered()), this, SLOT(debugFreeEMS()));

}


void qt4application::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(connectAct);
	fileMenu->addAction(fetchAct);
	fileMenu->addAction(sendfileAct);
	fileMenu->addAction(upgradeFirmwareAct);
	fileMenu->addAction(disconnectAct);
	fileMenu->addSeparator();
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addAction(printAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	adjustMenu = menuBar()->addMenu(tr("&Edit"));
	adjustMenu->addAction(adjustParametersAct);
	adjustFuelMenu = adjustMenu->addMenu(tr("&Fuel"));
	adjustIgnMenu = adjustMenu->addMenu(tr("&Ignition"));
	adjustOptions = adjustMenu->addMenu(tr("&Options"));

	adjustIgnMenu->addAction(adjustIgnitionAct);
	adjustIgnMenu->addAction(adjustIgnTempCorrectionAct);
	adjustFuelMenu->addAction(adjustFuelAct);
	adjustFuelMenu->addAction(adjustTempCorrectionAct);
	adjustFuelMenu->addAction(adjustAccelEnrichmentAct);
	adjustFuelMenu->addAction(adjustFuelTimingAct);

	adjustOptions->addAction(adjustAlarmsAct);
	adjustOptions->addAction(adjustBoostCtrlAct);
	adjustOptions->addAction(adjustNitrousAct);
								 //la admisin de luis
	adjustOptions->addAction(adjustPWM1);
	adjustOptions->addAction(adjustPWM2);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(view1Act);

	logMenu = menuBar()->addMenu(tr("&Data Acquisition"));
	logMenu->addAction(startLogAct);
	logMenu->addAction(saveLogAct);
	logMenu->addAction(replayAct);
	logMenu->addAction(viewAct);

	toolsMenu = menuBar()->addMenu(tr("&Tools"));
	toolsMenu->addAction(quickTuneAct);
	toolsMenu->addAction(injectorTestAct);
	toolsMenu->addAction(configureECUManagerAct);

//	if(hardwareTarget == "freeEMS project")
	{
		toolsMenu = menuBar()->addMenu(tr("&FreeEMS"));
		toolsMenu->addAction(sendResetAct);
		toolsMenu->addAction(getInterfaceVersionAct);
		toolsMenu->addAction(getFirmwareVersionAct);
		toolsMenu->addAction(sendDatalogRequestAct);
		toolsMenu->addAction(openDebugWindowAct);
	}

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(connectorPinoutAct);
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}


void qt4application::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	fileToolBar->addAction(saveAct);

	commToolBar = addToolBar(tr("Comm"));
	commToolBar->addAction(connectAct);
	commToolBar->addAction(sendfileAct);
	commToolBar->addAction(disconnectAct);

	/*    editToolBar = addToolBar(tr("Edit"));
		editToolBar->addAction(cutAct);
		editToolBar->addAction(copyAct);
		editToolBar->addAction(pasteAct);*/
}


void qt4application::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}
