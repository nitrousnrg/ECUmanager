/****************************************************************************
 **
 ** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://www.trolltech.com/products/qt/opensource.html
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://www.trolltech.com/products/qt/licensing.html or contact the
 ** sales department at sales@trolltech.com.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QMenu>
#include <QInputDialog>

class RenderArea : public QWidget
{
	Q_OBJECT

		public:
		RenderArea(QWidget *parent = 0,int limit=10000, int redLine=11000, int initialAngle=20, int finalAngle=230);

		QSize minimumSizeHint() const;
		QSize sizeHint() const;
		void setValue(float value);
		void setLimit(int);
		void setRedLine(int);
		void setInitialAngle(int);
		void setFinalAngle(int);

	protected:
		void paintEvent(QPaintEvent *event);
		void mouseReleaseEvent(QMouseEvent*);

	private slots:
		void setLimitSlot();
		void setRedLineSlot();
		void setInitialAngleSlot();
		void setFinalAngleSlot();
		void setNeedleColorSlot();
		void setNumbersColorSlot();
		void setScale1ColorSlot();
		void setScale2ColorSlot();

	private:
		QPen pen;
		QBrush brush;
		int InitialAngle;
		int FinalAngle;
		int Limit;
		int RedLine;
		float actualPosition;
		QColor needleColor;
		QColor numbersColor;
		QColor scale1Color;
		QColor scale2Color;
		QColor hourColor;
		QColor redPen;

		QMenu *menu;
		QMenu *changeNumbersMenu;
		QMenu *changeColorsMenu;
		QAction* setLimitAct;
		QAction* setRedLineAct;
		QAction* setNeedleColorAct;
		QAction* setNumbersColorAct;
		QAction* setScale1ColorAct;
		QAction* setScale2ColorAct;
		QAction* setInitialAngleAct;
		QAction* setFinalAngleAct;
		QInputDialog *setLimitDialog;
		QInputDialog *setRedLineDialog;
		QInputDialog *setInitialAngleDialog;
		QInputDialog *setFinalAngleDialog;

};
#endif
