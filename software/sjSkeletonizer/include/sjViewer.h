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
#include "sjLaplacianSmoothing.h"
#include <QtGui>
#include <QMainWindow>
#include <QMenu>
#include <QAction>

class QWidget;

class sjViewer : public QGLViewer {

  Q_OBJECT

public:

    
  
  sjViewer(QWidget * parent, bool antialiasing = false);

  // overload several QGLViewer virtual functions
  void draw();
  void initializeGL();
  virtual void init();
  void drawWithNames();
  void postSelection(const QPoint&);
  
  void setScene();
  bool antiAliasing() const { return antialiasing; }

  void setVerticesFaces(sjPolyhedron p1){
	  polyhedron = p1;
  }

  virtual QString helpString() const;

signals:
  void selected(int);

public slots:
  void setAntiAliasing(bool b);
  void setTwoSides(bool b);
  void LaplacianSmoothing();

protected:
     //void contextMenuEvent(QContextMenuEvent *event);

private slots:
     void open();

private:
  void drawModel();
  void draw_aux(bool with_names);

  bool antialiasing;
  bool twosides;
  sjPolyhedron polyhedron;
 
  sjPoint_3 normalize(sjPoint_3 p);
  sjPoint_3 normalVector(sjPoint_3 a, sjPoint_3 b, sjPoint_3 c);

  QMenu *fileMenu;
  QAction *openAct;
  void createActions();
  void createMenus();
  sjLaplacianSmoothing * laplacian_system;


};


#endif