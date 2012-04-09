/**
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * The Original Code is Copyright (C) 2007-2010 by Alexander Pinzon Fernandez.
 * Bogota - Colombia
 * All rights reserved.
 *
 * Author(s): Alexander Pinzón Fernández.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** 
 * @brief sjSkeletonizer San Jeronimo Viewer Application.
 * @author Alexander Pinzon Fernandez.
 * @date 21/02/2011 5:27 p.m.
 */

#ifndef __SJAPPLICATION__H__
#define __SJAPPLICATION__H__

#include <QDockWidget>
#include <QMainWindow>
#include <QTabWidget>
#include <QToolBox>
#include <QWidget>
#include <QLineEdit>
#include <QSlider>
#include "sjUtils.h"
#include "sjKernelPlugin.h"

using namespace sj;


class sjApplication: public QMainWindow{
    Q_OBJECT

public:
    sjApplication();
    ~sjApplication();
	QTabWidget * getCentralQTabWidget();

//protected:
    //void closeEvent( QCloseEvent* );

private slots:
    void open();
    void save();
    void about();
	void iterateLaplacian();
	void initSimplificator();
	void iterateSimplificator();
	void closeTab(int index);
	void changueSliderIteration(int value);
	void changueComputeWeight(QString text );



private:
	void loadPlugins();
	QTabWidget * central_QTabWidget;
	QDockWidget * tools_QDockWidget;
	QToolBox * tool_box_QToolBox;
	QLineEdit * txt_iterations;
	QSlider * sld_iterations;

	sjKernelPlugin & kernel_engine;


};


#endif //__SJAPPLICATION__H__

