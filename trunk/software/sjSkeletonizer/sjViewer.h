//Viewer class for qglviewer

#ifndef SJVIEWER_H
#define SJVIEWER_H

#include <QGLViewer/qglviewer.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_scan_OFF.h> 
#include <vector>
#include <fstream>
#include <CGAL/Point_3.h>
#include <CGAL/Vector_3.h>


typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3                                Point_3;
typedef Kernel::Vector_3                               Vector_3;

typedef CGAL::Polyhedron_3<Kernel>     Polyhedron;
typedef Polyhedron::Face_iterator FIterator ;
typedef Polyhedron::Vertex_iterator VIterator;
//typedef Polyhedron::
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;
typedef Polyhedron::Halfedge_around_vertex_circulator  Halfedge_vertex_circulator;

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

  void setVerticesFaces(Polyhedron p1){
	  polyhedron = p1;
  }

  virtual QString helpString() const;

signals:
  void selected(int);

public slots:
  void setAntiAliasing(bool b);
  void setTwoSides(bool b);
  void LaplacianSmoothing();

private:
  void drawModel();
  void draw_aux(bool with_names);

  bool antialiasing;
  bool twosides;
  Polyhedron polyhedron;
 
  Point_3 normalize(Point_3 p);
  Point_3 normalVector(Point_3 a, Point_3 b, Point_3 c);
};


#endif