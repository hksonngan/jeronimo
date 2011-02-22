//#include "sjConfig.h"
//#include "ui_sjSkeletonizerUI.h"
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
 #include <QDockWidget>
#include "sjViewer.h"
#include "sjDataIO.h"
#include "sjLaplacianSmoothing.h"
#include "sjApplication.h"

using namespace std;
//using namespace qglviewer;

int main(int argc, char **argv)
{
	//Q_INIT_RESOURCE(sjSkeletonizer);

  QApplication app(argc, argv);
	sjApplication * sjapp = new sjApplication();
	sjapp->resize(800,600);
	//sjapp->setCaption("San Jeronimo Skeletonizer");
	sjapp->show();
  //QWidget * window = new QWidget;
  /*QMainWindow * window  = new QMainWindow;

  window->setWindowTitle("San Jeronimo Viewer");
  QPushButton *button = new QPushButton("Laplacian Smoothing.");
  sjSkeletonizer * myviewer = new sjSkeletonizer(window, false);

  sjDataIO dataio;
  dataio.setFileName("obj.off");
  dataio.load();

  sjLaplacianSmoothing lpsmoo;

  lpsmoo.setMeshG(dataio.getPolyhedronModel());
  cout<<"\n\initLaplacianSystem: ";
  //for(int yiu = 0;yiu<10; yiu++)
  lpsmoo.initLaplacianSmoothing();
  cout<<"\n\FinishLaplacianSystem: ";
  for(int yiu = 0;yiu<10; yiu++)
  lpsmoo.iterateLaplacianSmoothingOGF();

  sjPolyhedron P;
  ifstream myfile ("tetra.off");
  myfile>>P;


  cout<<"\nParece que lo abrio "<<P.size_of_vertices();
  myviewer->setVerticesFaces(lpsmoo.getMeshG());

  
  QObject::connect(button, SIGNAL(clicked()), myviewer, SLOT(LaplacianSmoothing()));
  QObject::connect(button, SIGNAL(clicked()), myviewer, SLOT(updateGL()));
  

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(button);
  //mainLayout->addWidget(myviewer);

  QDockWidget * docbot = new QDockWidget("Boton", window);

  QWidget * windowbut = new QWidget;

  docbot->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  windowbut->setLayout(mainLayout);
  docbot->setWidget(windowbut);
  
   //window->setLayout(mainLayout);
   window->setCentralWidget(myviewer);
  	window->addDockWidget( Qt::LeftDockWidgetArea, docbot);

   myviewer->draw();
   window->show();*/


  /*app.setOrganizationDomain("San Jeronimo");
  app.setOrganizationName("San Jeronimo");
  app.setApplicationName("San Jeronimo Viewer");*/

  /*Q_INIT_RESOURCE(File);
  Q_INIT_RESOURCE(Triangulation_2); 
  Q_INIT_RESOURCE(Input);
  Q_INIT_RESOURCE(CGAL);*/



  /*Polyhedron P;


  ifstream myfile ("elephant.off");
  

  myfile>>P;

  cout<<"Parece que lo abrio "<<P.size_of_vertices();*/
  

  /*

  for ( f = P.facets_begin(); f != P.facets_end(); ++f){
		Halfedge_facet_circulator j = f->facet_begin();
		cout<<"Cara: ";
		do{
			cout<<j->vertex()->point()<<", ";
		}while(++j != f->facet_begin());
		cout<<"\n";
  }*/
  


  /*QMainWindow *window = new QMainWindow;

  Ui::DvonnWindow ui;
        ui.setupUi(window);

		

		ui.viewer->setVerticesFaces(P);*/
		
		//window->
		//connect(ui.actionLaplacian_Smoothing, SIGNAL(triggered()), ui, SLOT(ui.viewer->LaplacianSmoothing()));
		
		//QObject::connect(& (ui.actionLaplacian_Smoothing), SIGNAL(triggered()), &app, SLOT(ui.viewer->LaplacianSmoothing()));
		/*ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();
		ui.viewer->LaplacianSmoothing();*/
		//ui.viewer->draw();

        //window->show();

  /*MainWindow mainWindow;
  mainWindow.show();
  QStringList args = app.arguments();
  args.removeAt(0);

  if(!args.empty() && args[0] == "--use-meta")
  {
    mainWindow.setAddKeyFrameKeyboardModifiers(::Qt::MetaModifier);
    args.removeAt(0);
  }

  Q_FOREACH(QString filename, args) {
    mainWindow.open(filename);
  }*/
  return app.exec();
}