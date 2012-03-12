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
 
#ifndef __SJPLUGINWIDGET__H__
#define __SJPLUGINWIDGET__H__

#include <QWidget>
#include <QObject>
#include <QSlider>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <string>
#include <vector>
#include "sjKernelPlugin.h"
//#include "sjMeshFilterServer.h"
//#include "sjExternalServer.h"
#include "sjPipeFilter.h"
#include <sstream>
#include "sjApplication.h"

using namespace sj;


 class sjSliderSlotSignal : public QObject
 {
     Q_OBJECT

 public:
	 sjSliderSlotSignal(double a_min_value, double a_max_value, double a_step_value) {
			min_value		= a_min_value;
			max_value		= a_max_value;
			step_value		= a_step_value;
	 }

	 QString value() const { return m_value; }

public slots:
	void setValue(int value){
		double result;
		if(step_value != 0)  result = ((double)value) * step_value + min_value;
		else result = 0.0;

		std::stringstream out;
		out << result;
		m_value = QString(out.str().c_str());
		emit valueChanged(m_value);
	 }

 signals:
	 void valueChanged(QString newValue);

 private:
	 QString m_value;
	 double min_value;
	 double max_value;
	 double step_value;
 };

class sjPluginWidget: public QWidget{
	Q_OBJECT
public:
	sjPluginWidget(sjPolyhedronPipe::sjFilter * a_mesh_filter, sjApplication * parent = 0, Qt::WindowFlags f = 0 );
	std::string getName();
	~sjPluginWidget();
signals:
	void getMesh(sjPolyhedron);
public slots:
	void configure();
	void iterate();

private:
	sjPolyhedronPipe::sjFilter * mesh_filter;
	QSlider * getSlider(sjParameterBase * param);
	std::string name;
	std::vector<QLineEdit *> m_txt_sliders;
	std::vector<QSlider *> m_sliders;
	sjApplication * m_parent;
};

#endif