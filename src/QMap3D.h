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
#include <qwt3d_surfaceplot.h>
#include <qwt3d_function.h>
#include <qwt3d_plot.h>

using namespace Qwt3D;

class QMap3D: public SurfacePlot
{
	Q_OBJECT
		private:
		double **dataTable;
		union  transformer
		{
			unsigned char byte[2];
			int entero;
		};
	public:
		QMap3D();
		~QMap3D();

		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		bool loadData(double data,int, int);
		void setBackground(QColor color);
		void update();

};
