#include "sjViewer.h"
#include "sjDataIO.h"
//#include <CGAL/IO/Polyhedron_iostream.h>

//#include <QObject>
//#include <QtOpenGL/qgl.h>
using namespace std;
using namespace qglviewer;

sjViewer::sjViewer(QWidget* parent, bool antialiasing)
  : QGLViewer(parent),
    antialiasing(antialiasing),
    twosides(false)
{
  setBackgroundColor(::Qt::white);
  
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
	sjPoint_3 puntos[3], a,b,c;
	for ( sjFIterator f = polyhedron.facets_begin(); f != polyhedron.facets_end(); ++f){
		
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
		sjPoint_3 color3(0.9,0.9,0.9);
   
		glBegin(GL_TRIANGLES);
			//glColor3f(1.0f,1.0f,1.0f);
			glColor3f( (float)color3[0] ,(float)color3[1],(float)color3[2]);
			glNormal3f( (float)pnormal[0],(float)pnormal[1], (float)pnormal[2]);
			glVertex3f( (float)a[0] ,(float)a[1],(float)a[2]);
			glVertex3f( (float)b[0] ,(float)b[1],(float)b[2]);
			glVertex3f( (float)c[0] ,(float)c[1],(float)c[2]);
		glEnd();
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

void sjViewer::drawWithNames(){
  
}

void sjViewer::postSelection(const QPoint&){
}


void sjViewer::init(){
	
	glShadeModel ( GL_FLAT );     //off

	glShadeModel ( GL_SMOOTH ); 

	glClearColor (0.0, 0.0, 0.0, 0.0);
	

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
    glEnable(GL_DEPTH_TEST);

	   glColorMaterial(GL_FRONT, GL_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   
   

glDepthFunc(GL_LEQUAL);


	// Restore previous viewer state.
	restoreStateFromFile();

	// Opens help window
	help();

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
	int k,  i = 0;
	cout<<"\n\n\n";
	for ( sjVIterator v1 = polyhedron.vertices_begin(); v1 != polyhedron.vertices_end(); ++v1){
		sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
		vcir->vertex()->index = i;
		i++;
	}
	size_t size = (size_t )i;
	vector< vector<double> > lmatrix(size, vector<double>(size,0));   

	for ( sjVIterator v = polyhedron.vertices_begin(); v != polyhedron.vertices_end(); ++v){
		sjHalfedge_vertex_circulator vcir = v->vertex_begin();
		//cout<<"Punto: "<<vcir->vertex()->index<<" = "<<vcir->vertex()->point()<<"\n";
		//cout<<"  Vecinos: \n";
		do{
			sjPoint_3 punto = vcir->next()->vertex()->point();
			//cout<<"  Vecino: "<<vcir->next()->vertex()->index<<" = " <<vcir->next()->vertex()->point()<<"\n";
		}while(++vcir != v->vertex_begin ());
	}
	cout<<"Vertices: "<<i;
	
}