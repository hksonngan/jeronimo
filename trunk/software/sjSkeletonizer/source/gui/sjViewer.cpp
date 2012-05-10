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

#include "sjViewer.h"
#include "sjDataIO.h"
#include "sjUtils.h"
#include "sjLog.h"
#include <CGAL/Point_3.h>
//#include <CGAL/IO/Polyhedron_iostream.h>

//#include <QObject>
//#include <QtOpenGL/qgl.h>
using namespace std;
using namespace qglviewer;

int sjViewer::index_id = 0;

sjViewer::sjViewer(QWidget* parent, bool antialiasing)
  : QGLViewer(parent),
    antialiasing(antialiasing),
    twosides(false),
	paint_skeleton(false)/*,
	laplacian_system(0)*/
{
	m_index_id = index_id++;
  
}

void sjViewer::setScene()
{
}

void sjViewer::setAntiAliasing(bool b)
{
  antialiasing = b;
  updateGL();
}

void sjViewer::setTwoSides(bool b)
{
  twosides = b;
  updateGL();
}

void sjViewer::draw(){
  draw_aux(false);
}

void sjViewer::initializeGL(){	
	QGLViewer::initializeGL();
}

void sjViewer::drawModel(){

	/*for ( sjFIterator f = polyhedron.facets_begin(); f != polyhedron.facets_end(); ++f){
		
		sjHalfedge_facet_circulator j = f->facet_begin();
		
		
		int i = 0;
		do{
		
			puntos[i++] = j->vertex()->point();
		}while(++j != f->facet_begin());
		a = puntos[0];
		b = puntos[1];
		c = puntos[2];


		sjPoint_3 pnormal = normalVector(puntos[0], puntos[1], puntos[2]);
		//Point_3 color3 = normalize(a);
		sjPoint_3 color3(0.9,0.1,0.1);
   
		
		glBegin(GL_TRIANGLES);
			//glColor3f(1.0f,1.0f,1.0f);
			glColor3f( (float)color3[0] ,(float)color3[1],(float)color3[2]);
			glNormal3f( (float)pnormal[0],(float)pnormal[1], (float)pnormal[2]);
			glVertex3f( (float)a[0]/max_min, (float)a[1]/max_min, (float)a[2]/max_min);
			glVertex3f( (float)b[0]/max_min, (float)b[1]/max_min, (float)b[2]/max_min);
			glVertex3f( (float)c[0]/max_min, (float)c[1]/max_min, (float)c[2]/max_min);
		glEnd();
	}*/



	if(paint_skeleton){
		
		for(int i=0; i<m_skeleton.halfedges_bool.size(); i++){
			if(m_skeleton.halfedges_bool[i]){
				sjGraphPoint p1, p2;
				p1 = m_skeleton.points_data[m_skeleton.halfedges_data[i].point_incident_id];
				p2 = m_skeleton.points_data[m_skeleton.halfedges_data[i].point_opposite_id];
   
				float wred, wgreen, wblue;
				wred = getRedSpectrumColor(m_skeleton.MIN_WEIGHT, m_skeleton.MAX_WEIGHT, m_skeleton.halfedges_data[i].weight);
				wgreen = getGreenSpectrumColor(m_skeleton.MIN_WEIGHT, m_skeleton.MAX_WEIGHT, m_skeleton.halfedges_data[i].weight);
				wblue = getBlueSpectrumColor(m_skeleton.MIN_WEIGHT, m_skeleton.MAX_WEIGHT, m_skeleton.halfedges_data[i].weight);

				glPointSize(3.0f + 5.0f * (1.0f - (float)(m_skeleton.halfedges_data[i].weight/( m_skeleton.MAX_WEIGHT - m_skeleton.MIN_WEIGHT))));
				
				glBegin(GL_LINES);
					glColor3f( wred,wgreen,wblue);
					glVertex3f( (float)p1.x/max_min, (float)p1.y/max_min, (float)p1.z/max_min);
					glVertex3f( (float)p2.x/max_min, (float)p2.y/max_min, (float)p2.z/max_min);
				glEnd();

				glBegin(GL_POINTS);
					//glColor3f( (float)0.1 ,(float)0.1,(float)0.9);
					glColor3f( wred,wgreen,wblue);
					glVertex3f( (float)p1.x/max_min, (float)p1.y/max_min, (float)p1.z/max_min);
					glVertex3f( (float)p2.x/max_min, (float)p2.y/max_min, (float)p2.z/max_min);
				glEnd();
			}
		}
	}else{
		sjPoint_3 puntos[3], a,b,c;
	
		for ( sjHalfedge_handle f = polyhedron.halfedges_begin(); f != polyhedron.halfedges_end(); ++f){
		
			a = f->vertex()->point();
			b = f->opposite()->vertex()->point();


			//sjPoint_3 pnormal = normalVector(puntos[0], puntos[1], puntos[2]);
			//Point_3 color3 = normalize(a);
			sjPoint_3 color3(0.9,0.1,0.1);
   
		
			glBegin(GL_LINES);
				//glColor3f(1.0f,1.0f,1.0f);
				glColor3f( (float)color3[0] ,(float)color3[1],(float)color3[2]);
				//glNormal3f( (float)pnormal[0],(float)pnormal[1], (float)pnormal[2]);
				glVertex3f( (float)a[0]/max_min, (float)a[1]/max_min, (float)a[2]/max_min);
				glVertex3f( (float)b[0]/max_min, (float)b[1]/max_min, (float)b[2]/max_min);
			glEnd();
		}
	}

}

void sjViewer::draw_aux(bool with_names){
	

	float ambientLight1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diffuseLight1[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float specularLight1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	
	float position1[] = { 1.5f, 1.0f, -8.0f, 1.0f };	
	float position2[] = { 1.5f, 1.0f, 8.0f, 1.0f };

	GLfloat light_position[] = { -10.0f, 1.0f, -8.0f, 1.0f };	

	GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight1);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight1);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT2, GL_POSITION, position2);


	drawModel();
  
}

sjPoint_3 sjViewer::normalize(sjPoint_3 p){
	// calculate the length of the vector
	double len = (double)(CGAL::sqrt((p[0] * p[0]) + (p[1] * p[1]) + (p[2] * p[2])));
 
    // avoid division by 0
    if (len == 0.0)
        len = 1.0;
 
    // reduce to unit size
	double x,y,z;
    x = p[0]/len;
    y = p[1]/len;
    z = p[2]/len;
	return sjPoint_3(x,y,z);
}

sjPoint_3 sjViewer::normalVector(sjPoint_3 a, sjPoint_3 b, sjPoint_3 c){
	sjVector_3 pnormal = CGAL::cross_product(b-a,c-a);
	return normalize(sjPoint_3(pnormal[0], pnormal[1], pnormal[2]));
}

void sjViewer::drawWithNames(){
  
}

void sjViewer::postSelection(const QPoint&){
}


void sjViewer::init(){
	
	glShadeModel ( GL_FLAT );     //off

	glShadeModel ( GL_SMOOTH ); 

	glClearColor (0.4, 0.4, 0.4, 0.0);
	

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
    glEnable(GL_DEPTH_TEST);

	   glColorMaterial(GL_FRONT, GL_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   glPointSize(5.0f);

   
   

glDepthFunc(GL_LEQUAL);


	// Restore previous viewer state.
	restoreStateFromFile();

	// Opens help window
	//help();

}

QString sjViewer::helpString() const
{
  QString text("<h2>S i m p l e V i e w e r</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
  text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
  text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
  text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
  text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
  text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}

void sjViewer::LaplacianSmoothing(){
	/*if(laplacian_system == NULL){
		laplacian_system = new sjLaplacianSmoothing();
		laplacian_system->setMeshG(polyhedron);
		laplacian_system->initLaplacianSmoothing();
	}
	laplacian_system->iterateLaplacianSmoothingOGF2();
	//laplacian_system->meanCurvatureSmoothing();
	this->setVerticesFaces(laplacian_system->getMeshG());
	this->update();*/
}

void sjViewer::consume(sjPolyhedronPipe::PolyhedronType a_meshG){
	sjLogDebug("sjViewer::consume 1\n");
	sjLogDebug("sjViewer::consume 3\n");
	this->polyhedron = a_meshG;
	this->setBoundingBox( calcBoundingBox(this->polyhedron));
	sjLogDebug("sjViewer::consume 4\n");
	sjLogDebug("sjViewer::consume 5\n");
	this->update();
	sjLogDebug("sjViewer::consume 6\n");
}