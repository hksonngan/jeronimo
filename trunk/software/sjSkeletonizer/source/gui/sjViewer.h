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

#ifndef sjViewer_H
#define sjViewer_H

#include <QGLViewer/qglviewer.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_scan_OFF.h> 
#include <CGAL/HalfedgeDS_vector.h>
#include <vector>
#include <fstream>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>
#include <CGAL/Polyhedron_3.h>
//#include "sjLaplacianSmoothing.h"
#include <QtGui>
#include <QMainWindow>
#include <QMenu>
#include <QAction>

#include "sjDataType.h"
#include "sjDataType.h"
#include "sjPipeFilter.h"
#include "sjGraphSkeleton.h"
#include "sjSimplificator.h"

using namespace sj;

class QWidget;

class sjViewer : public QGLViewer, public sjPolyhedronPipe::sjSink, public sjSubject {

  Q_OBJECT

public:

	void setSkeleton(sjGraphSkeletonType aske){
		m_skeleton = aske;
		paint_skeleton = true;
	}

	sjGraphSkeletonType getSkeleton(){
		return m_skeleton;
	}

    
  
  sjViewer(QWidget * parent, bool antialiasing = false);

  int getId(){return m_index_id;}

  // overload several QGLViewer virtual functions
  void draw();
  void initializeGL();
  virtual void init();
  void drawWithNames();
  void postSelection(const QPoint&);
  
  void setScene();
  bool antiAliasing() const { return antialiasing; }


  sjPolyhedron getVerticesFaces(){
	  return polyhedron;
  }

  void setBoundingBox(sjIso_cuboid_3 bb){
	  bounding_box = bb;
	  	double min, max;
	double pX[24];
	int ko = 0;
	while(ko< 24){
		pX[ko] = (bounding_box.vertex(ko))[0];
		pX[ko+1] = (bounding_box.vertex(ko))[1];
		pX[ko+2] = (bounding_box.vertex(ko))[2];
		ko = ko + 3;
	}
	min = *(std::min_element(pX,pX+24));
	max = *(std::max_element(pX,pX+24));
	max_min = std::abs( max - min);
  }

  void consume(sjPolyhedronPipe::PolyhedronType a_meshG);

  virtual QString helpString() const;
  sjPoint_3 normalize(sjPoint_3 p);
  sjPoint_3 normalVector(sjPoint_3 a, sjPoint_3 b, sjPoint_3 c);

signals:
  void selected(int);

public slots:
  void setVerticesFaces(sjPolyhedron p1){
	  polyhedron = p1;
	  this->update();
  }
  void setAntiAliasing(bool b);
  void setTwoSides(bool b);
  void LaplacianSmoothing();

private:
  double max_min;
  void drawModel();
  void draw_aux(bool with_names);

  bool antialiasing;
  bool twosides;


  //sjLaplacianSmoothing * laplacian_system;
  sjIso_cuboid_3 bounding_box;
  bool paint_skeleton;
  sjGraphSkeletonType m_skeleton;
  
  static int index_id;
  int m_index_id;


};


#endif