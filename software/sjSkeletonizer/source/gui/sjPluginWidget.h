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

#define GET_VALUES_NUMERIC_PARAM(name_type)\
			min_value = (int)((name_type *)param)->getMinValue();\
			max_value = (int)((name_type *)param)->getMaxValue();\
			default_value = (int)((name_type *)param)->getDefaultValue();\
			step_value = (int)((name_type *)param)->getStep();\

#include <QWidget>
#include <QObject>
#include <QSlider>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <string>
#include <vector>
#include "sjKernel.h"
#include "sjParameter.h"
#include <sstream>

using namespace sj;

 class sjSliderSlotSignal : public QObject
 {
     Q_OBJECT

 public:
     sjSliderSlotSignal() { }

	 QString value() const { return m_value; }

 public slots:
	 void setValue(int value){
		std::stringstream out;
		out << value;
		m_value = QString(out.str().c_str());
		 emit valueChanged(m_value);
	 }

 signals:
	 void valueChanged(QString newValue);

 private:
	 QString m_value;
 };

class sjPluginWidget: public QWidget{
	Q_OBJECT
public:
	sjPluginWidget(sjMeshFilter * a_mesh_filter, QWidget * parent = 0, Qt::WindowFlags f = 0 );
	std::string getName();
	~sjPluginWidget();
signals:
	void getMesh(sjPolyhedron);
public slots:
	void configure();
	void iterate();

private:
	sjMeshFilter * mesh_filter;
	QSlider * getSlider(sjParameterBase * param);
	std::string name;
	std::vector<QLineEdit *> m_txt_sliders;
	std::vector<QSlider *> m_sliders;
};

#endif