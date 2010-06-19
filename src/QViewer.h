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

#ifndef QVIEWER_H
#define QVIEWER_H

#include "QSubViewer.h"
#include "plotter.h"

#include <QWidget>
#include <QGridLayout>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QSettings>
#include <QCloseEvent>

#include <QFileDialog>
#include <QMenuBar>
#include <QMainWindow>

class QViewer : public QMainWindow
{
Q_OBJECT
public:
	QViewer();
	QString getVersion();
	void plotChannelData(int channel,int index);
	void desplazar(int channel, float value);
	int getChannel(int display);
	void setChannel(int,int);
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void openFile();
	void updateChannel0(int updChannel);
	void updateChannel1(int updChannel);
	void updateChannel2(int updChannel);
	bool exportCsv();
	void printDialog();

private:
	void readSettings();
	void writeSettings();
	bool online;

	char data[10];
	int channel[3];
	QAction * openFileAct;
	QAction * exportCsvAct;
	QAction * printAct;
	QWidget *window;
	QMenuBar *menu;
	QMenu *fileMenu;
	QString versionName;
	QString fileName;
	QGridLayout *grid;
	QComboBox *comboY;
	QComboBox *comboX;
	QSubViewer *display;
//	QSubViewer *display2;
//	QSubViewer *display3;

	char *points;
	void createDisplay();

	QFile *file;

	union  transformer
	{
		char byte[2];
		int entero;
	}RPM,auxTransformer;

	struct degreeConverter
	{
		char byte[2];
		float angle;
	}auxDegree;
};

#endif
