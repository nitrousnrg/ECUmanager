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

#include <math.h>
#include <qapplication.h>
#include <qwt3d_surfaceplot.h>
// #include <qwt3d_function.h>
#include <QMap3D.h>
#include <QColorDialog>
#include <QStyle>

// using namespace Qwt3D;
/*
  class Rosenbrock : public Function
  {
  public:

	Rosenbrock(SurfacePlot& pw)
	:Function(pw)
	{
	}

	double operator()(double x, double y)
	{
	  return log((1-x)*(1-x) + 100 * (y - x*x)*(y - x*x)) / 8;
	}
  };
*/
/*
  class Plot : public SurfacePlot
  {
  public:
	  Plot();
  };
*/

QMap3D::QMap3D()
{

	dataTable = (double **) malloc(12*sizeof(double*));
	for(int i = 0;i<12;++i)
		dataTable[i] = (double *)malloc(21*sizeof(double));
	for (int row=0; row<12; row++)
		for(int col=0;col<12;col++)
			dataTable[row][col]=0;
	//loadFromData(dataTable,12,21,0,21,0,12);

	RGBA rgb = Qt2GL(QColor(210,209,208));
	setBackgroundColor(rgb);
	//setTitle("VE");

	//coordinates()->axes[X1].setLabelString("MAP");
	//coordinates()->axes[Y1].setLabelString("RPM");

	setRotation(30,0,-55);
	setScale(1,1,1);
	setShift(0.15,0,0);
	setZoom(1.0);

	/*	for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
		{
			coordinates()->axes[i].setMajors(4);
			coordinates()->axes[i].setMinors(4);
		}
	*/

	//coordinates()->axes[Z1].setLabelString(QChar(0x38f)); // Omega - see http://www.unicode.org/charts/

	//setCoordinateStyle(BOX);

	updateData();
	updateGL();
}

void QMap3D::setBackground(QColor color)
{
	RGBA rgb = Qt2GL(color);
	setBackgroundColor(rgb);
}


bool    QMap3D::loadData(double data,int row, int column)
{

	dataTable[row][column] = data;
	loadFromData(dataTable,12,21,0,10,0,10);

	return true;
}


void    QMap3D::update()
{
	updateGL();
}


QSize   QMap3D::minimumSizeHint() const
{
	return QSize(100, 100);
}


QSize  QMap3D::sizeHint() const
{
	return QSize(300, 200);
}


QMap3D::~QMap3D()
{
	for(int i = 0;i<12;++i)
		free(dataTable[i]);
	free(dataTable);
}
