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
 * @date 21/01/2011 10:07 a.m.
 */

#include "sjApplication.h"
#include "sjDataIO.h"
#include "sjViewer.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <iostream>
#include <QVBoxLayout>

sjApplication::sjApplication():
QMainWindow( 0)
{
	
	QAction * openAct = new QAction(("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip("Open an OFF file");
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	QAction * saveAct = new QAction(("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip("Save the model to disk");
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	QMenu *fileMenu;
    fileMenu = menuBar()->addMenu(("&File"));
	fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

	QAction * aboutAct = new QAction(("&About"), this);
	aboutAct->setStatusTip("About San Jeronimo Viewer");
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	QMenu * helpMenu;
	helpMenu = menuBar()->addMenu(("&Help"));
    helpMenu->addAction(aboutAct);

	central_QTabWidget = new QTabWidget(this);
	setCentralWidget(central_QTabWidget);
	tools_QDockWidget = new QDockWidget("Tools", this);
	tool_box_QToolBox = new QToolBox(tools_QDockWidget);
	tools_QDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	tools_QDockWidget->setWidget(tool_box_QToolBox);
	addDockWidget(Qt::LeftDockWidgetArea, tools_QDockWidget);

	QWidget * panel = new QWidget();
	QPushButton * cmd_iterateLaplacian = new QPushButton("Iterate Laplacian System", panel);
	connect(cmd_iterateLaplacian , SIGNAL(clicked()),this, SLOT(iterateLaplacian()));
	QVBoxLayout *toolLayout = new QVBoxLayout;
	toolLayout->addWidget(cmd_iterateLaplacian);
	QSpacerItem* qspaceritem = new QSpacerItem( 20, 200,	QSizePolicy::Maximum, QSizePolicy::Expanding );
	toolLayout->addSpacerItem(qspaceritem);
	panel->setLayout(toolLayout);
	tool_box_QToolBox->addItem(panel, "Laplacian Smoothing");

}

sjApplication::~sjApplication()
{
    
}


 void sjApplication::open() 
 {
	 std::cout<<"Invoked <b>File|Open</b>";
	 QString fileName = QFileDialog::getOpenFileName(this,
		 tr("Open 3D File"), "./", tr("3D Files (*.off)"));
	 sjDataIO dataio;
	 dataio.setFileName(fileName.toLatin1().constData());
	 dataio.load();
	 sjSkeletonizer * myviewer = new sjSkeletonizer(central_QTabWidget, false);
	 myviewer->setVerticesFaces(dataio.getPolyhedronModel());
	 
	 central_QTabWidget->addTab(myviewer, fileName.right(fileName.size()-1-fileName.lastIndexOf("/")));

 }

 void sjApplication::save()
 {
     std::cout<<"Invoked <b>File|Save</b>";
 }

  void sjApplication::about()
 {
     std::cout<<"Invoked <b>Help|About</b>";
 }

void sjApplication::iterateLaplacian()
{
	std::cout<<"Invoked <b>Iterate Laplacian</b>";
	sjSkeletonizer * current = (sjSkeletonizer *)( central_QTabWidget->currentWidget());
	if( current != NULL){
		current->LaplacianSmoothing();
	}
}