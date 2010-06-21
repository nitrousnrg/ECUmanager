/***************************************************************************
 *   Copyright (C) 2007 by marcos chaparro                                 *
 *   nitrousnrg@gmail.com                                                  *
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

/* The icon set is from Amarok, not mine! (Nice job guys)*/

#ifndef QT4APPLICATION_H
#define QT4APPLICATION_H

#include <QtGui>

#include <QMainWindow>
#include <QCloseEvent>
#define _TTY_POSIX_
#include <qextserialport.h>
#include <commthread.h>
//#include "commCore_FreeEMS.h"
#include <plotter.h>
#include <2Dgraph.h>
#include <firm_updater.h>
#include <renderarea.h>
#include <QViewer.h>
#include <QuickTune.h>
#include <QPlayer.h>
#include <QInjectorTest.h>
#include <qwt3d_surfaceplot.h> 
#include <qwt3d_function.h>
#include <qwt3d_plot.h>

#include "QMap3D.h"

#include <QLabel>

class QAction;
class QMenu;
class QTextEdit;
class PlotArea;
class RenderArea;
class Graph2D;

#define MAPsourced 0
#define TPSsourced 1

typedef unsigned char BYTE;

class qt4application:public QMainWindow
{
	Q_OBJECT

public:
	qt4application();
	~qt4application();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void connectECU();
	void disconnectECU();
	void openECU();
	void openECUslot();
	void sendFilesInit();
	void upgradeFirmware();
	void configureECUmanager();
	void refresh();
	void showIgnition();
	void showFuel();
	void showFuelTiming();
	void tempDialog();
	void accelDialog();
	void putTempCalibration();
	void boostDialog();
	void nitrousDialog();
	void parametersDialog();
	void alarmsDialog();
	void printDialog();
	void acceptDialog();
	void rejectDialog();
	void startLog();
	void saveLog();
	void replay();
	void viewData();
	void quickTune();
	void injectorTestDialog();
	void newFile();
	void open();
	void checkChange(QTableWidgetItem *);
	void QuickTuneChange(int channel, int index);
	void save();	//bool
	void saveAs();	//bool
	void about();
	void documentWasModified();

        void debugFreeEMS();
        void showPacket(QByteArray);
        void sendReset();


private:
	void createVEtable();
	void createLinearTable();
	void createBars();
	void createLabels();
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createLayout();
	void readSettings();
	void writeSettings();
	bool maybeSave();
	void loadFile(const QString &fileName);
	void process_line(QByteArray line,int i);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);

	bool connectedECU;
	bool fileopened;
	bool parametersDialogOpen;
	bool tempDialogOpen;
	bool accelEnrichDialogOpen;
	bool alarmDialogOpen;
	bool configDialogOpen;
	int nitrousDialogOpen;	//WTF?!?!
	bool logEnabled;
	int injectorTestDialogOpen;
	bool VE_table_ON;
	bool fuel_adv_ON;
	bool Ign_table_ON;
	bool QuickTune_ON;
        bool debugEnabledFlag;
 	QString strippedName(const QString &fullFileName);

	QWidget *window;
	QWidget *QmainLayout;
	QextSerialPort *serial;
	commThread *serialThread;
	QString hardwareTarget,hardwareTargetTemp;

        QMap3D *map3D;
	QViewer *plot1;
	QuickTune *QTune;
	upgrade *newFirmware;
	PlotArea *plot;
	graph2D *tempCorrGraph;
	graph2D *tempSensorGraph;
        RenderArea *renderArea;     //bad name for the tacho
	QPlayer *playMenu;
	QInjTester *tester;
	QTimer *sendTimer;
	QTimer *refreshTimer;
	QTimer *openECUtimer;
	char logbuffer[4];
	QByteArray auxiliarByteArray;

	QTableWidget *VE_table;
	QTableWidget *Ign_table;
	QTableWidget *fuelAdvTable;
        QTableWidget *packetTable;

        QTableWidgetItem VEtableItem[16][25];
	QTableWidgetItem ignTableItem[12][21];
	QTableWidgetItem fuelAdvTableItem[12];
        QTableWidgetItem *packetTableItem;
        QHBoxLayout *horizontaLayout;
	QDialog *dialog;
	QMenu *fileMenu;
	QMenu *adjustMenu;
	QMenu *adjustFuelMenu;

	QMenu *adjustIgnMenu;
	QMenu *adjustOptions;
	QMenu *viewMenu;
	QMenu *logMenu;
	QMenu *helpMenu;
	QMenu *toolsMenu;
	QToolBar *fileToolBar;
	QToolBar *commToolBar;
	QLabel *heavyDataLabel;
	QLabel *VElabel;
	QLabel *MAPlabel;
	QLabel *AirTemplabel;
	QLabel *TEMPlabel;
	QLabel *throttleLabel;
	QLabel *AirFuellabel;
	QLabel *InyTimelabel;
	QLabel *Dutylabel;

	QLineEdit * serialPortEdit;
	QLineEdit *enrichFactorEdit;	//accel pump
	QLineEdit *decayEdit;		//accel pump
	QLineEdit *accelTresholdEdit;	//accel pump
	QLineEdit *editDisplacement;
	QLineEdit *editCyl;
	QLineEdit *editIdle;
	QLineEdit *editFuelCut;
	QLineEdit *editLaunchRPM;
	QLineEdit *editSize;
	QLineEdit *editDeadTime;
	QComboBox *hardwareTargetBox;

	QCheckBox *calibration;
	QProgressBar *VEbar;
	QProgressBar *MAPbar;
	QProgressBar *TEMPbar;
	QProgressBar *AirFuelbar;
	QProgressBar *InyTimebar;
	QProgressBar *Dutybar;
	QProgressBar *throttleBar;
	QProgressBar *AirTempbar;
	QProgressBar *sendingBar;
	QString string1;
	QString serialPort;
 
	QAction *connectAct;
	QAction *disconnectAct;
	QAction *fetchAct;
	QAction *sendfileAct;
	QAction *upgradeFirmwareAct;
	QAction *view1Act;
	QAction *adjustParametersAct;
	QAction *adjustIgnitionAct;
	QAction *adjustFuelAct;
	QAction *adjustFuelTimingAct;
	QAction *adjustTempCorrectionAct;
	QAction *adjustAccelEnrichmentAct;
	QAction *adjustIgnTempCorrectionAct;
	QAction *adjustAlarmsAct;
	QAction *adjustBoostCtrlAct;
	QAction *adjustNitrousAct;
	QAction *adjustPWM1;
	QAction *adjustPWM2;
	QAction *configureECUManagerAct;
	QAction *startLogAct;
	QAction *saveLogAct;
	QAction *replayAct;
	QAction *viewAct;
	QAction *quickTuneAct;
	QAction *injectorTestAct;
	QAction *newAct;
	QAction *openAct;
	QAction *saveAct;
	QAction *saveAsAct;
	QAction *exitAct;
	QAction *printAct;
	QAction *cutAct;
	QAction *copyAct;
	QAction *pasteAct;
	QAction *connectorPinoutAct;
	QAction *aboutAct;
	QAction *aboutQtAct;

        /* FreeEMS stuff */
        QAction *sendResetAct;
        QAction *getInterfaceVersionAct;
        QAction *getFirmwareVersionAct;
        QAction *openDebugWindowAct;


	unsigned char MAP,throttle;
	char TEMP,waterTEMP;
	float duty,Dwell;
	int byteCounter;	//para la programación

        char *oxygenSensor;	//wb, nb, none

	struct estructura
	{
		unsigned int cylinders;
		unsigned int displacement;	//cm³
		unsigned int fuelCut;		//rpm
		unsigned int idleRPM;
		unsigned int launchRPM;
		unsigned int injSize;		//cm³/min
		unsigned int deadTime;		//us

		unsigned char accel_pump_source;
		unsigned char accel_pump_treshold;
		unsigned char accel_pump_decay;
		unsigned char accel_pump_enrich_factor;

		float tempCorr[13];		//enrich %
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

	union  transformer
	{
		BYTE byte[2];
		int entero;
	}RPM,VE,VEtable[12][21],fuel,auxTransformer;

	int parcialVE_MAP[12];
	int parcialVE_RPM[21];
	struct degreeConverter
	{
		BYTE byte[2];
		float angle;
	}IgnTable[12][21],fuelAdv[12],fuelAdvActual,IgnAdvActual;

	union zocotroco_de_numeros
	{
		BYTE partido[12][21][2];
		char plain[504];
	}plaindataVE,plaindataIgn;


	FILE *f;
	QString confFile;
	QString openedFile;
	QString logFileName;
	QFile *tempLogFile;
	QFile *openedLogFile;
	//QString &logFileName;

/* nitrous dialog */
	QLineEdit *editNitrousRPM;
	QLineEdit *editNitrousMAP;
	QLineEdit *editNitrousTPS;
	QLineEdit *editNitrousFlow1;
	QLineEdit *editNitrousFlow2;
	QLineEdit *editNitrousFlow3;
	QLineEdit *editNitrousRetard1;
	QLineEdit *editNitrousRetard2;
	QLineEdit *editNitrousRetard3;
};

#endif
