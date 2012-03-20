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
#include "sjLaplacianSmoothing.h"
#include "sjCoreSmoothing.h"
#include "sjLog.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <iostream>
#include <QVBoxLayout>
#include <fstream>
#include <iostream>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_scan_OFF.h> 

#include "sjPluginXMLLoader.h"
#include "sjParameter.h"
#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/configurator.h>


#include "sjPluginWidget.h"

using namespace sj;
using namespace log4cplus;
using namespace log4cplus::helpers;

sjApplication::sjApplication():
QMainWindow( 0), kernel_engine(sjKernelPlugin::getInstance())
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
	central_QTabWidget->setTabsClosable (true);
	connect(central_QTabWidget, SIGNAL(tabCloseRequested ( int )), this, SLOT(closeTab(int)));
	setCentralWidget(central_QTabWidget);
	tools_QDockWidget = new QDockWidget("Tools", this);
	tool_box_QToolBox = new QToolBox(tools_QDockWidget);
	tools_QDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	tools_QDockWidget->setWidget(tool_box_QToolBox);
	addDockWidget(Qt::LeftDockWidgetArea, tools_QDockWidget);
	
	QWidget * panel = new QWidget();
	QPushButton * cmd_iterateLaplacian = new QPushButton("Iterate Laplacian System", panel);
	connect(cmd_iterateLaplacian, SIGNAL(clicked()),this, SLOT(iterateLaplacian()));

	txt_iterations = new QLineEdit("1", panel);
	txt_iterations->setModified(false);
	sld_iterations = new QSlider(Qt::Horizontal, panel);
	sld_iterations->setMaximum (20 );
	sld_iterations->setMinimum (1);
	connect(sld_iterations, SIGNAL(valueChanged(int)),this, SLOT(changueSliderIteration(int)));

	QVBoxLayout *toolLayout = new QVBoxLayout;
	toolLayout->addWidget(txt_iterations);
	toolLayout->addWidget(sld_iterations);
	toolLayout->addWidget(cmd_iterateLaplacian);
	QSpacerItem* qspaceritem = new QSpacerItem( 20, 200,	QSizePolicy::Maximum, QSizePolicy::Expanding );
	toolLayout->addSpacerItem(qspaceritem);
	panel->setLayout(toolLayout);
	tool_box_QToolBox->addItem(panel, "Laplacian Smoothing");

	/*log4cplus::BasicConfigurator config;
    config.configure();*/
	/*
	try
	{
	PropertyConfigurator::doConfigure("logs.properties");
	}
	catch(exception e)
	{

		sjLogDebug("Exception occured while opening log4cplus.properties");
		sjLogDebug(e.what());
	}*/

	//PropertyConfigurator::doConfigure("logs.properties");
	//log4cplus::PropertyConfigurator config2("logs.properties");
	//config2.configure();
	/*
	log4cplus::Logger logger = log4cplus::Logger::getInstance("filelogger");
	//OG4CPLUS_INFO(logger, message);
	//LOG4CPLUS_INFO(logger, "sdasdasd");
	LOG4CPLUS_INFO(logger, "infoooo");
	LOG4CPLUS_ERROR(logger, "erorrrr");
	LOG4CPLUS_ERROR(logger, "erorrr1");
	LOG4CPLUS_ERROR(logger, "erorrr2");
	LOG4CPLUS_ERROR(logger, "erorrr3");
	LOG4CPLUS_DEBUG(logger, "debug");
	LOG4CPLUS_WARN(logger, "warn");*/


	
	/*
	sjPluginXmlLoader * loader = new sjPluginXmlLoader("E:/personal/maestria/tesis/Jeronimo/trunk/software/sjSkeletonizer/source/resources/example1.xml");
	if(loader  == NULL)
		sjLogDebug("loader is NULL");

	if(kernel_engine.addPlugin(loader))
		sjLogDebug("Si la registro\n");
	else
		sjLogDebug("No la registro\n");*/

	kernel_engine.setDefaultSystem( new PluginInitIndex());
	kernel_engine.setDefaultSystem( new PluginComputeRings());
	kernel_engine.setDefaultSystem( new PluginComputeLaplacian());
	kernel_engine.setDefaultSystem( new PluginComputeMeanCurvature());
	kernel_engine.setDefaultSystem( new PluginMeanCurvatureSmoothing());
	kernel_engine.setDefaultSystem( new PluginIsDegenerateVertex());
	kernel_engine.setDefaultSystem( new PluginComputeLineEquations());
	kernel_engine.setDefaultSystem( new PluginInitLaplacianSmoothing());
	kernel_engine.setDefaultSystem( new PluginIterateLaplacianSmoothingIntegrate());


}

sjApplication::~sjApplication()
{
    
}

void sjApplication::closeTab(int index)
{
	central_QTabWidget->removeTab(index);
} 

 void sjApplication::open() 
 {
	 sjLogDebug("sjApplication::open");
	 QString fileName = QFileDialog::getOpenFileName(this,
		 tr("Open 3D File"), "./", tr("3D Files (*.off)"));

	 sjLogDebug("sjApplication::open 1\n");
	 OFFLoaderSource * off_loader= new OFFLoaderSource(fileName.toLatin1().constData());
	 sjLogDebug("sjApplication::open 2\n");
	 sjPolyhedronPipe::sjPipe * pipe_source = new sjPolyhedronPipe::sjPipe();
	 sjLogDebug("sjApplication::open 3\n");
	 sjStateContext * ctx = new sjStateContext();
	 sjLogDebug("sjApplication::open 4\n");
	 sjPolyhedronPipe::sjPipe * pipe_sink = new sjPolyhedronPipe::sjPipe();
	 sjLogDebug("sjApplication::open 5\n");

	 sjViewer * myviewer = new sjViewer(central_QTabWidget, false);
	 sjLogDebug("sjApplication::open 6\n");
	 off_loader->setOutputPipe(pipe_source);
	 sjLogDebug("sjApplication::open 7\n");
	 pipe_source->setOuputConsumer(ctx);
	 sjLogDebug("sjApplication::open 8\n");
	 ctx->setInputPipe(pipe_source);
	 sjLogDebug("sjApplication::open 9\n");
	 ctx->setOutputPipe(pipe_sink);
	 sjLogDebug("sjApplication::open 10\n");
	 pipe_sink->setOuputConsumer(myviewer);
	 sjLogDebug("sjApplication::open 11\n");
	 myviewer->setInputPipe(pipe_sink);
	 sjLogDebug("sjApplication::open 12\n");
	 myviewer->attach(ctx);

	 sjLogDebug("sjApplication::open 13\n");
	 off_loader->update();
	 sjLogDebug("sjApplication::open -> END");

	 int index  = central_QTabWidget->addTab(myviewer, fileName.right(fileName.size()-1-fileName.lastIndexOf("/")));
		 central_QTabWidget->setCurrentIndex ( index );
	 

 }

 void sjApplication::save()
 {
	 QString fileNameqt = QFileDialog::getSaveFileName(this,
		 tr("Open 3D File"), "./", tr("3D Files (*.off)"));
	 sjDataIO dataio;
	char filename[256];
	sprintf(filename, "%s", fileNameqt.toLatin1().constData());

	fstream file_off(filename, ios::out);

	sjViewer * current = (sjViewer *)( central_QTabWidget->currentWidget());
	if( current != NULL){
		file_off<<(current->getVerticesFaces());
	}

	file_off.close();
 }

  void sjApplication::about()
 {
     std::cout<<"Invoked <b>Help|About</b>";
 }

void sjApplication::iterateLaplacian()
{
	sjLogDebug("sjApplication::iterateLaplacian 1\n");
	sjEvent * mevt = new sjEvent(sjEvent::EVT_ITERATE);
	sjLogDebug("sjApplication::iterateLaplacian 2\n");
	//this->kernel_engine.notify(mevt);
	sjViewer * current = (sjViewer *)( central_QTabWidget->currentWidget());
	sjLogDebug("sjApplication::iterateLaplacian 3\n");
	if( current != NULL){
		//int iter = sld_iterations->value();
		//for(int j = 0; j<iter; j++)
		sjLogDebug("sjApplication::iterateLaplacian 4\n");
		current->dispatch(mevt);
		sjLogDebug("sjApplication::iterateLaplacian 5\n");
	}
}

void sjApplication::changueSliderIteration(int value){
	char num[256];
	sprintf(num, "%d", value);
	txt_iterations->setText(QString(num));
	sld_iterations->setValue(value);
}

void sjApplication::loadPlugins(){

}

QTabWidget * sjApplication::getCentralQTabWidget(){
	return central_QTabWidget;
}