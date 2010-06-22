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

/* Dialogs, and secondary stuff	*/

void qt4application::configureECUmanager()
{
	dialog = new QDialog(this);
	dialog->setModal(true);
	//nitrousDialogOpen = true;
	dialog->setSizeGripEnabled(false);
	dialog->resize(380,360);
	dialog->setWindowTitle(tr("Configure ECU manager"));
	configDialogOpen = true;

	QGridLayout *grid = new QGridLayout;
	QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
	connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));

	//General tab

	QGridLayout *generalLayout = new QGridLayout;
	generalLayout->setColumnStretch(0,100);
	generalLayout->setRowStretch(1,100);
	QLabel *hardwareTargetName = new QLabel(tr("Hardware target"));
	hardwareTargetBox = new QComboBox();
	hardwareTargetBox->addItem("ePIC project");
        hardwareTargetBox->addItem("freeEMS project");
        hardwareTargetBox->addItem("core28");
        hardwareTargetBox->setCurrentIndex(hardwareTargetBox->findText(hardwareTarget));

	generalLayout->addWidget(hardwareTargetName,0,0);
	generalLayout->addWidget(hardwareTargetBox,0,1);

	//Communications
	QGridLayout *vbox1 = new QGridLayout;
	vbox1->setColumnStretch(0,100);
	vbox1->setRowStretch(1,100);
	QLabel *serialPortName = new QLabel(tr("Serial Port"));
	serialPortEdit = new QLineEdit(serialPort);
	serialPortEdit->setAlignment(Qt::AlignRight);
	vbox1->addWidget(serialPortName,0,0);		//default serial port="/dev/ttyUSB0";
	vbox1->addWidget(serialPortEdit,0,1);

	/*  User interface  */
	QGridLayout *vbox2 = new QGridLayout;
        QCheckBox *VEcheckbox = new QCheckBox("VE", this);
        QCheckBox *MAPcheckbox = new QCheckBox("MAP", this);
        QCheckBox *AirTempcheckbox = new QCheckBox("Air Temp", this);
        QCheckBox *WaterTempcheckbox = new QCheckBox("Water Temp", this);
        QCheckBox *AFcheckbox = new QCheckBox("Air/Fuel", this);
        QCheckBox *throttleCheckbox = new QCheckBox("Throttle", this);
        QCheckBox *dutyCheckbox = new QCheckBox("Duty cycle", this);
        QCheckBox *injTimeCheckbox = new QCheckBox("Injection time", this);
        QCheckBox *RPMcheckbox = new QCheckBox("RPM", this);
        QCheckBox *plotCheckbox = new QCheckBox("2D plot", this);
        QCheckBox *plot3Dcheckbox = new QCheckBox("3D plot", this);
        QCheckBox *tableCheckbox = new QCheckBox("Table", this);

        VEcheckbox->setChecked(VEbar->isVisible());
        MAPcheckbox->setChecked(MAPbar->isVisible());
        AirTempcheckbox->setChecked(AirTempbar->isVisible());
        WaterTempcheckbox->setChecked(TEMPbar->isVisible());
        AFcheckbox->setChecked(AirFuelbar->isVisible());
        throttleCheckbox->setChecked(throttleBar->isVisible());
        dutyCheckbox->setChecked(Dutybar->isVisible());
        injTimeCheckbox->setChecked(InyTimebar->isVisible());
        plotCheckbox->setChecked(plot->isVisible());
        plot3Dcheckbox->setChecked(map3D->isVisible());
        tableCheckbox->setChecked(VE_table->isVisible());
        RPMcheckbox->setChecked(renderArea->isVisible());

        connect(VEcheckbox,SIGNAL(clicked(bool)),VEbar,SLOT(setVisible(bool)));
        connect(VEcheckbox,SIGNAL(clicked(bool)),VElabel,SLOT(setVisible(bool)));
        connect(MAPcheckbox,SIGNAL(clicked(bool)),MAPbar,SLOT(setVisible(bool)));
        connect(MAPcheckbox,SIGNAL(clicked(bool)),MAPlabel,SLOT(setVisible(bool)));
        connect(AirTempcheckbox,SIGNAL(clicked(bool)),AirTempbar,SLOT(setVisible(bool)));
        connect(AirTempcheckbox,SIGNAL(clicked(bool)),AirTemplabel,SLOT(setVisible(bool)));
        connect(WaterTempcheckbox,SIGNAL(clicked(bool)),TEMPbar,SLOT(setVisible(bool)));
        connect(WaterTempcheckbox,SIGNAL(clicked(bool)),TEMPlabel,SLOT(setVisible(bool)));
        connect(AFcheckbox,SIGNAL(clicked(bool)),AirFuelbar,SLOT(setVisible(bool)));
        connect(AFcheckbox,SIGNAL(clicked(bool)),AirFuellabel,SLOT(setVisible(bool)));
        connect(throttleCheckbox,SIGNAL(clicked(bool)),throttleBar,SLOT(setVisible(bool)));
        connect(throttleCheckbox,SIGNAL(clicked(bool)),throttleLabel,SLOT(setVisible(bool)));
        connect(dutyCheckbox,SIGNAL(clicked(bool)),Dutybar,SLOT(setVisible(bool)));
        connect(dutyCheckbox,SIGNAL(clicked(bool)),Dutylabel,SLOT(setVisible(bool)));
        connect(injTimeCheckbox,SIGNAL(clicked(bool)),InyTimebar,SLOT(setVisible(bool)));
        connect(injTimeCheckbox,SIGNAL(clicked(bool)),InyTimelabel,SLOT(setVisible(bool)));
        connect(plotCheckbox,SIGNAL(clicked(bool)),plot,SLOT(setVisible(bool)));
        connect(plot3Dcheckbox,SIGNAL(clicked(bool)),map3D,SLOT(setVisible(bool)));
        connect(tableCheckbox,SIGNAL(clicked(bool)),VE_table,SLOT(setVisible(bool)));
        connect(RPMcheckbox,SIGNAL(clicked(bool)),renderArea,SLOT(setVisible(bool)));


        vbox2->addWidget(VEcheckbox,    1,1);
        vbox2->addWidget(MAPcheckbox,   2,1);
        vbox2->addWidget(AirTempcheckbox,3,1);
        vbox2->addWidget(WaterTempcheckbox,4,1);
        vbox2->addWidget(AFcheckbox,    5,1);
        vbox2->addWidget(throttleCheckbox,6,1);
        vbox2->addWidget(dutyCheckbox,  7,1);
        vbox2->addWidget(injTimeCheckbox,1,2);
        vbox2->addWidget(RPMcheckbox, 2,2);
        vbox2->addWidget(plotCheckbox,  3,2);
        vbox2->addWidget(plot3Dcheckbox,4,2);
        vbox2->addWidget(tableCheckbox,  5,2);

	/*  security */
	QGridLayout *vbox4 = new QGridLayout;



	QTabWidget *tabBar = new QTabWidget();
	QWidget *comm = new QWidget();
	comm->setLayout(vbox1);
	QWidget *general = new QWidget();
	general->setLayout(generalLayout);



	QWidget *interface = new QWidget();
	interface->setLayout(vbox2);
	QWidget *security = new QWidget();
	security->setLayout(vbox4);
	tabBar->addTab(general,"General");
	tabBar->addTab(comm,"Communications");
	tabBar->addTab(interface,"User Interface");
	tabBar->addTab(security,"Security");


	//tabBar->setLayout(vbox);

	grid->addWidget(tabBar,1,0,1,2);
	//grid->addWidget(groupBox2,1,0,1,2);
	grid->addWidget(accept,3,1);

	dialog->setLayout(grid);
	dialog->show();
}

void qt4application::setMainTableSize()
{
    setMainTableSizeDialogOpen = true;

    dialog = new QDialog(this);
    QFormLayout *formLayout = new QFormLayout;
    QString num;
    connect(dialog, SIGNAL(rejected()), this, SLOT(acceptDialog()));

    headerTableRPM->setRowHeight(0,18);
    for(int i=0; i<21; ++i)
    {
        headerContentsRPM[i].setText(num.setNum(confParameter.headerRPM[i]));
        headerTableRPM->setItem(0,i,&(headerContentsRPM[i]));
        headerTableRPM->setColumnWidth(i,47);
    }

    headerTableMAP->setRowHeight(0,18);
    for(int i=0; i<21; ++i)
    {
        headerContentsMAP[i].setText(num.setNum(confParameter.headerMAP[i]));
        headerTableMAP->setItem(0,i,&(headerContentsMAP[i]));
        headerTableMAP->setColumnWidth(i,47);
    }

    dialog->setModal(true);
    dialog->setSizeGripEnabled(false);
    dialog->resize(1100,50);
    dialog->setWindowTitle(tr("Configure table headers"));
    formLayout->addRow(tr("RPM"), headerTableRPM);
    formLayout->addRow(tr("MAP"), headerTableMAP);
    dialog->setLayout(formLayout);
    dialog->show();

}


void qt4application::injectorTestDialog()
{
	if(serialThread->getQextSerialPort())
	{
		tester = new QInjTester(serialThread->getQextSerialPort());
		tester->show();
	}
	else
                qDebug("hi, i'm a useless inj. test debug dialog' %d",(int)serialThread->getQextSerialPort());
}

void qt4application::quickTune()
{
	QuickTune_ON = true;
	for(int i=0;i<12;++i)
		parcialVE_MAP[i] = 0;
	for(int i=0;i<21;++i)
		parcialVE_RPM[i] = 0;

	QTune = new QuickTune();
	connect(QTune,SIGNAL(QTuneChange(int,int)),this,SLOT(QuickTuneChange(int,int)));
}
void qt4application::QuickTuneChange(int channel, int index)
{
	switch(channel)
	{
		case QuickTune::MAP:
		{
			parcialVE_MAP[11-index] = QTune->getMAP(index);
			index = 11-index;
			for (int i=0;i<21;++i)
			{
				VEtable[index][i].entero = parcialVE_MAP[index] + parcialVE_RPM[i];
				VEtableItem[index][i].setTextAlignment(Qt::AlignVCenter);
				VEtableItem[index][i].setData(0, QVariant( ( (float)(VEtable[index][i].entero)/10 ) ));
				VE_table->setItem(index,i,&(VEtableItem[index][i]));
			}
			break;
		}
		case QuickTune::RPM:
		{
			parcialVE_RPM[index] = QTune->getRPM(index);
			for (int i=0;i<13;++i)
			{
				VEtable[i][index].entero = parcialVE_MAP[i] + parcialVE_RPM[index];
				VEtableItem[i][index].setTextAlignment(Qt::AlignVCenter);
				VEtableItem[i][index].setData(0, QVariant( ( (float)(VEtable[i][index].entero)/10 ) ));
				VE_table->setItem(i,index,&(VEtableItem[i][index]));
			}
		}
	}
}

void qt4application::parametersDialog()
{
	dialog = new QDialog(this);
	parametersDialogOpen = true;
	QGridLayout *grid = new QGridLayout;

	QLabel *displacementLabel = new QLabel(tr("Displacement (cm3)"));
	editDisplacement = new QLineEdit(string1.setNum(confParameter.displacement));
	editDisplacement->setAlignment(Qt::AlignRight);

	QLabel *cylLabel = new QLabel(tr("Cylinders"));
	editCyl = new QLineEdit(string1.setNum(confParameter.cylinders));
	editCyl->setAlignment(Qt::AlignRight);

	QLabel *idleLabel = new QLabel(tr("Idle (RPM)"));
	editIdle = new QLineEdit(string1.setNum(confParameter.idleRPM));
	editIdle->setAlignment(Qt::AlignRight);

	QLabel *fuelCutLabel = new QLabel(tr("Fuel Cut (RPM)"));
	editFuelCut = new QLineEdit(string1.setNum(confParameter.fuelCut));
	editFuelCut->setAlignment(Qt::AlignRight);

	QLabel *launchRPMLabel = new QLabel(tr("Launch ctrl (RPM)"));
	editLaunchRPM = new QLineEdit(string1.setNum(confParameter.launchRPM));
	editLaunchRPM->setAlignment(Qt::AlignRight);

	QLabel *InjSizelabel = new QLabel(tr("Flow (cm3/min)"));
	editSize = new QLineEdit(string1.setNum(confParameter.injSize));
	editSize->setAlignment(Qt::AlignRight);

	QLabel *DeadTimelabel = new QLabel(tr("Dead Time (us)"));
	editDeadTime = new QLineEdit(string1.setNum(confParameter.deadTime));
	editDeadTime->setAlignment(Qt::AlignRight);

	QRadioButton *dualRamp = new QRadioButton(tr("Single Injector\nper cyl"));
	dualRamp->setChecked(true);
	QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
	connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));

	dialog->setModal(true);
	dialog->setSizeGripEnabled(false);
	dialog->resize(300,560);
	dialog->setWindowTitle(tr("Parameters"));


	QGroupBox *groupBox2 = new QGroupBox(tr("Engine"));
	QGridLayout *vbox2 = new QGridLayout;
	vbox2->addWidget(cylLabel,0,0);
	vbox2->addWidget(editCyl,0,1);
	vbox2->addWidget(displacementLabel,1,0);
	vbox2->addWidget(editDisplacement,1,1);
	vbox2->addWidget(fuelCutLabel,2,0);
	vbox2->addWidget(editFuelCut,2,1);
	vbox2->addWidget(idleLabel,3,0);
	vbox2->addWidget(editIdle,3,1);
	vbox2->addWidget(launchRPMLabel,4,0);
	vbox2->addWidget(editLaunchRPM,4,1);
	groupBox2->setLayout(vbox2);

	QGroupBox *groupBox1 = new QGroupBox(tr("Injectors"));
	QGridLayout *vbox = new QGridLayout;
	vbox->addWidget(InjSizelabel,0,0);
	vbox->addWidget(editSize,0,1);
	vbox->addWidget(DeadTimelabel,1,0);
	vbox->addWidget(editDeadTime,1,1);
	vbox->addWidget(dualRamp,2,0);
	groupBox1->setLayout(vbox);

	grid->addWidget(groupBox2,0,0,1,2);
	grid->addWidget(groupBox1,1,0,1,2);
	grid->addWidget(accept,3,1);

	dialog->setLayout(grid);
	dialog->show();
}

void qt4application::boostDialog()
{
}

void qt4application::tempDialog()
{
	dialog = new QDialog(this);
	tempDialogOpen = true;
	QGridLayout *grid = new QGridLayout;

	calibration = new QCheckBox(tr("Calibrate sensor"));
	calibration->setChecked(false);

	QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	tempCorrGraph = new graph2D;
	for(int i=0;i<13;++i)
	{
		tempCorrGraph->setPoint(i*20,(int)(confParameter.tempCorr[i]*400));
		tempCorrGraph->setScaleX(i,i*10-10);
		if(i<9)
			tempCorrGraph->setScaleY(i,(float)i/20);
	}


	//tempSensorGraph = new graph2D;

	connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
	connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));
	connect(calibration, SIGNAL(released()), this, SLOT(putTempCalibration()));

	dialog->setModal(true);
	dialog->setSizeGripEnabled(false);
	dialog->resize(360,360);
	dialog->setWindowTitle(tr("Water Temp Correction"));


	QGroupBox *groupBox2 = new QGroupBox(tr("Enrich %"));
	QGridLayout *vbox2 = new QGridLayout;
	vbox2->addWidget(tempCorrGraph,1,0);
	vbox2->addWidget(calibration,2,0);
	//vbox2->addWidget(tempSensorGraph,3,0);
	groupBox2->setLayout(vbox2);


	grid->addWidget(groupBox2,0,0,1,2);
	grid->addWidget(accept,3,1);

	dialog->setLayout(grid);
	//tempSensorGraph->hide();
	dialog->show();
}
void qt4application::putTempCalibration()
{
	if(calibration->isChecked())
	{
		//tempSensorGraph->show();
		dialog->resize(300,560);
	}
	else
	{
		//tempSensorGraph->hide();
		dialog->resize(300,360);
	}
}

void qt4application::accelDialog()		//dialogos para el tema de la bomba de pique
{
	dialog = new QDialog(this);
	dialog->setModal(true);
	dialog->setSizeGripEnabled(false);
	dialog->resize(430,480);
	dialog->setWindowTitle(tr("Acceleration enrichment"));

	accelEnrichDialogOpen = true;


	QGridLayout *grid = new QGridLayout;
	QVBoxLayout *vbox = new QVBoxLayout;
	QGroupBox *groupBox = new QGroupBox(tr("Source"));

	QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
	connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));

	QPixmap image(":/step_response.png");

	QLabel *imageLabel = new QLabel;
	QLabel *tresholdLabel = new QLabel("Treshold (kPa/sec)");
	QLabel *initialEnrichLabel = new QLabel("Initial enrichment factor");
	QLabel *decayLabel = new QLabel("Decay (revs)");

	enrichFactorEdit = new QLineEdit(string1.setNum(confParameter.accel_pump_enrich_factor));
	decayEdit = new QLineEdit(string1.setNum(confParameter.accel_pump_decay));
	accelTresholdEdit = new QLineEdit(string1.setNum(confParameter.accel_pump_treshold));
	
	QRadioButton *MAPradio = new QRadioButton(tr("MAP"));
	QRadioButton *TPSradio = new QRadioButton(tr("TPS"));

	if(confParameter.accel_pump_source == MAPsourced)
		MAPradio->setChecked(true);
	else
		TPSradio->setChecked(true);

	grid->addWidget(tresholdLabel,2,0);
	grid->addWidget(accelTresholdEdit,2,1);
	grid->addWidget(initialEnrichLabel,3,0);
	grid->addWidget(enrichFactorEdit,3,1);
	grid->addWidget(decayLabel,4,0);
	grid->addWidget(decayEdit,4,1);
	grid->addWidget(accept,5,1);

	vbox->addWidget(imageLabel);
	vbox->addWidget(groupBox);
	vbox->addLayout(grid);
		
	QVBoxLayout *vbox2 = new QVBoxLayout;
	vbox2->addWidget(MAPradio);
	vbox2->addWidget(TPSradio);
	vbox2->addStretch(1);
	groupBox->setLayout(vbox2);

	imageLabel->setPixmap(image);


	dialog->setLayout(vbox);
	dialog->show();
}

void qt4application::nitrousDialog()
{
	dialog = new QDialog(this);
	dialog->setModal(true);
	nitrousDialogOpen = true;
	dialog->setSizeGripEnabled(false);
	dialog->resize(328,360);
	dialog->setWindowTitle(tr("Nitrous Control"));
//	QString string1;

	QGridLayout *grid = new QGridLayout;
	QDialogButtonBox *accept = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(accept, SIGNAL(accepted()), this, SLOT(acceptDialog()));
	connect(accept, SIGNAL(rejected()), this, SLOT(rejectDialog()));

	QLabel *minimum = new QLabel(tr("Min"));
	QLabel *RPMlabel = new QLabel(tr("RPM"));
	QLabel *MAPLabel = new QLabel(tr("MAP"));
	QLabel *throttleLabel = new QLabel(tr("Throttle (%)            "));

	QLabel *flow1 = new QLabel(tr("Extra fuel (ms)"));
	QLabel *retard1 = new QLabel(tr("Ignition retard"));

	QLabel *flow2 = new QLabel(tr("Extra fuel (ms)"));
	QLabel *retard2 = new QLabel(tr("Ignition retard"));

	QLabel *flow3 = new QLabel(tr("Extra fuel (ms)"));
	QLabel *retard3 = new QLabel(tr("Ignition retard"));

	/*  conditions  */
	editNitrousRPM = new QLineEdit(string1.setNum(confParameter.nitrousRPM));
	editNitrousMAP = new QLineEdit(string1.setNum(confParameter.nitrousMAP));
	editNitrousTPS = new QLineEdit(string1.setNum(confParameter.nitrousTPS));

	editNitrousFlow1 = new QLineEdit(string1.setNum(confParameter.nitrousStage1Flow));
	editNitrousFlow2 = new QLineEdit(string1.setNum(confParameter.nitrousStage2Flow));
	editNitrousFlow3 = new QLineEdit(string1.setNum(confParameter.nitrousStage3Flow));
	editNitrousRetard1 = new QLineEdit(string1.setNum(confParameter.nitrousStage1Retard));
	editNitrousRetard2 = new QLineEdit(string1.setNum(confParameter.nitrousStage2Retard));
	editNitrousRetard3 = new QLineEdit(string1.setNum(confParameter.nitrousStage3Retard));

	editNitrousRPM->setAlignment(Qt::AlignRight);
	editNitrousMAP->setAlignment(Qt::AlignRight);
	editNitrousTPS->setAlignment(Qt::AlignRight);
	editNitrousFlow1->setAlignment(Qt::AlignRight);
	editNitrousFlow2->setAlignment(Qt::AlignRight);
	editNitrousFlow3->setAlignment(Qt::AlignRight);
	editNitrousRetard1->setAlignment(Qt::AlignRight);
	editNitrousRetard2->setAlignment(Qt::AlignRight);
	editNitrousRetard3->setAlignment(Qt::AlignRight);

	QGridLayout *vbox1 = new QGridLayout;
	vbox1->addWidget(minimum,0,1);
	vbox1->addWidget(RPMlabel,1,0);
	vbox1->addWidget(editNitrousRPM,1,1);
	vbox1->addWidget(MAPLabel,2,0);
	vbox1->addWidget(editNitrousMAP,2,1);
	vbox1->addWidget(throttleLabel,3,0);
	vbox1->addWidget(editNitrousTPS,3,1);
	vbox1->setRowStretch(1,100);

	/*  stage 1  */
	QGridLayout *vbox2 = new QGridLayout;
	vbox2->addWidget(flow1,0,0);
	vbox2->addWidget(editNitrousFlow1,0,1);
	vbox2->addWidget(retard1,1,0);
	vbox2->addWidget(editNitrousRetard1,1,1);

//	vbox2->addWidget(MAPLabel,2,0);
//	vbox2->addWidget(throttleLabel,3,0);

	/*  stage 2 */
	QGridLayout *vbox3 = new QGridLayout;
	vbox3->addWidget(flow2,0,0);
	vbox3->addWidget(editNitrousFlow2,0,1);
	vbox3->addWidget(retard2,1,0);
	vbox3->addWidget(editNitrousRetard2,1,1);

	/*  stage 3 */
	QGridLayout *vbox4 = new QGridLayout;
	vbox4->addWidget(flow3,0,0);
	vbox4->addWidget(editNitrousFlow3,0,1);
	vbox4->addWidget(retard3,1,0);
	vbox4->addWidget(editNitrousRetard3,1,1);



	QTabWidget *tabBar = new QTabWidget();
	QWidget *conditions = new QWidget();
	conditions->setLayout(vbox1);
	QWidget *stage1 = new QWidget();
	stage1->setLayout(vbox2);
	QWidget *stage2 = new QWidget();
	stage2->setLayout(vbox3);
	QWidget *stage3 = new QWidget();
	stage3->setLayout(vbox4);
	tabBar->addTab(conditions,"Conditions");
	tabBar->addTab(stage1,"Stage 1");
	tabBar->addTab(stage2,"Stage 2");
	tabBar->addTab(stage3,"Stage 3");


	//tabBar->setLayout(vbox);

	grid->addWidget(tabBar,1,0,1,2);
	//grid->addWidget(groupBox2,1,0,1,2);
	grid->addWidget(accept,3,1);

	dialog->setLayout(grid);
	dialog->show();
}

void qt4application::readSettings()
{
        QSettings settings("FreeEMS", "ECU manager");
	serialPort = settings.value("Serial Port",QString("/dev/ttyUSB0")).toString();
	hardwareTarget = settings.value("Hardware Target",QString("ePIC project")).toString();
	confFile = settings.value("default File",QString()).toString();
	logFileName = settings.value("default Log File",QString()).toString();
	plot->setBackColor( settings.value("plotBackground Color",Qt::white).value<QColor>() );
        plot->setFontColor( settings.value("plotFont Color",Qt::darkGray).value<QColor>() );
        plot->setChannel1Color( settings.value("plotChannel 1 Color",Qt::red).value<QColor>() );
	resize(settings.value("size", QSize(1200, 600)).toSize());
        move(settings.value("pos", QPoint(50, 50)).toPoint());

        VEbar->         setVisible(settings.value("Show VE bar",true).toBool());
        VElabel->       setVisible(settings.value("Show VE bar",true).toBool());
        MAPbar->        setVisible(settings.value("Show MAP bar",true).toBool());
        MAPlabel->      setVisible(settings.value("Show MAP bar",true).toBool());
        AirTempbar->    setVisible(settings.value("Show Air Temp bar",true).toBool());
        AirTemplabel->  setVisible(settings.value("Show Air Temp bar",true).toBool());
        TEMPbar->       setVisible(settings.value("Show Water Temp bar",true).toBool());
        TEMPlabel->     setVisible(settings.value("Show Water Temp bar",true).toBool());
        AirFuelbar->    setVisible(settings.value("Show Air/fuel bar",true).toBool());
        AirFuellabel->  setVisible(settings.value("Show Air/fuel bar",true).toBool());
        throttleBar->   setVisible(settings.value("Show throttle bar",true).toBool());
        throttleLabel-> setVisible(settings.value("Show throttle bar",true).toBool());
        Dutybar->       setVisible(settings.value("Show duty cycle bar",true).toBool());
        Dutylabel->     setVisible(settings.value("Show duty cycle bar",true).toBool());
        InyTimebar->    setVisible(settings.value("Show injection time bar",true).toBool());
        InyTimelabel->  setVisible(settings.value("Show injection time bar",true).toBool());
        plot->          setVisible(settings.value("Show 2D plot",true).toBool());
        map3D->         setVisible(settings.value("Show 3D plot",true).toBool());
        renderArea->    setVisible(settings.value("Show RPM",true).toBool());

}

void qt4application::writeSettings()
{
        QSettings settings("FreeEMS", "ECU manager");
	settings.setValue("Serial Port",serialPort);
	settings.setValue("Hardware Target",hardwareTarget);
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("default File",confFile);
	settings.setValue("default Log File",logFileName);
	settings.setValue("plotBackground Color",plot->getBackgroundColor());
	settings.setValue("plotFont Color",plot->getFontColor());
        settings.setValue("plotChannel 1 Color",plot->getChannelColor(1));
        settings.setValue("Show VE bar",VEbar->isVisible());
        settings.setValue("Show MAP bar",MAPbar->isVisible());
        settings.setValue("Show Air Temp bar",AirTempbar->isVisible());
        settings.setValue("Show Water Temp bar",TEMPbar->isVisible());
        settings.setValue("Show Air/fuel bar",AirFuelbar->isVisible());
        settings.setValue("Show throttle bar",throttleBar->isVisible());
        settings.setValue("Show duty cycle bar",Dutybar->isVisible());
        settings.setValue("Show injection time bar",InyTimebar->isVisible());
        settings.setValue("Show 2D plot",plot->isVisible());
        settings.setValue("Show 3D plot",map3D->isVisible());
        settings.setValue("Show table",VE_table->isVisible());
        settings.setValue("Show RPM",renderArea->isVisible());
}

void qt4application::alarmsDialog()
{
	alarmDialogOpen = true;

	dialog = new QDialog(this);
	dialog->setModal(true);
	dialog->setSizeGripEnabled(false);
	dialog->resize(360,360);
	dialog->setWindowTitle(tr("Alarms"));

	QLabel *rpmLabel = new QLabel("RPM");
	QLineEdit *RPMmEditMax = new QLineEdit();
	//QLabel *waterTempLabel = new QLabel("Water Temp");
	//QLineEdit *waterTempMaxEdit = new QLineEdit();

	QGridLayout *grid = new QGridLayout();
	grid->addWidget(rpmLabel,1,0);
	grid->addWidget(RPMmEditMax,1,1);
	
	dialog->setLayout(grid);
	dialog->show();
}

void qt4application::printDialog()
{
	QString filePrintName = QFileDialog::getOpenFileName(this, tr("Choose log file"),confFile,tr("Config File(*.txt)"));
	QFile file(filePrintName);
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
				painter.drawText(0,i,QString(file.readLine()));
			}
	//printer->newPage();
		// print ...
		painter.end();
	}
}

void qt4application::documentWasModified()
{	setWindowModified(true);}

void qt4application::about()
{
      QMessageBox::about(this, tr("About ECU manager"),
            tr("<b>ECU mangager</b> is a high-end software designed to "
               "control a microchip-based ecu, trying to be a powerful analysis "
               "tool. This an alpha version."));
}
