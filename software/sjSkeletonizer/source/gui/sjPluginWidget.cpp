#include "sjPluginWidget.h"

//sjPluginWidget::sjPluginWidget(sjMeshFilter * a_mesh_filter, sjApplication * parent, Qt::WindowFlags f):
sjPluginWidget::sjPluginWidget(sjPolyhedronPipe::sjFilter * a_mesh_filter, sjApplication * parent, Qt::WindowFlags f):
QWidget(parent, f), mesh_filter (0), m_parent(parent)
{
	mesh_filter = a_mesh_filter;

	//name = std::string( mesh_filter->getName());
	name = std::string( "Mesh Filter");
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();

	/*sjStoreServer * server_store = (sjStoreServer *)kernel.getExternalServer(std::string("sjStoreServer"));
	printf("sjPluginWidget::sjPluginWidget Line 0\n");
	sjMeshStoreDriver * mdriver =(sjMeshStoreDriver *) server_store->getInstanceStoreDriver(std::string("MESH_FILTER_NAME"));
	printf("sjPluginWidget::sjPluginWidget Line 1\n");
	sjMeshFilterInfo * info_mf = (sjMeshFilterInfo * ) mdriver->getObject(name);
	printf("sjPluginWidget::sjPluginWidget Line 2\n");*/
	sjParameterStore * info_mf = kernel.getMeshFilterInfo("Mesh Filter");
	if(info_mf == NULL) printf("sjPluginWidget::sjPluginWidget is NULLLLLLL\n");

	unsigned int i = 0;
	//if(info_mf->getParameters() == NULL) printf("sjPluginWidget::sjPluginWidget info_mf->getParameters() is NULLLLLLL\n");
	//unsigned int n = info_mf->getParameters()->getNumberOfParameters();
	unsigned int n = info_mf->getNumberOfParameters();

	QVBoxLayout *toolLayout = new QVBoxLayout;

	QLabel * a1_label = new QLabel(QString(name.c_str()));
	toolLayout->addWidget(a1_label);

	for(i = 0; i<n; i++){
		//sjParameterBase * a_param = info_mf->getParameters()->getParameter(i);
		sjParameterBase * a_param = info_mf->getParameter(i);
		if(a_param->getType() >= 10 && a_param->getType() < 100){
			QLineEdit * a_QLineEdit = new QLineEdit("", this);
			QSlider * sld_iterations = getSlider(a_param);
			QLabel * a_label = new QLabel(QString(a_param->getName().c_str()));
			
			double min_value, max_value, step_value;

			min_value = (double)((sjDoubleParam *)a_param)->getMinValue();
			max_value = (double)((sjDoubleParam *)a_param)->getMaxValue();
			step_value = (double)((sjDoubleParam *)a_param)->getStep();

			sjSliderSlotSignal * m_slider_ss = new sjSliderSlotSignal(min_value, max_value, step_value);

			connect(sld_iterations, SIGNAL(valueChanged(int)),sld_iterations, SLOT(setValue(int)));
			connect(sld_iterations, SIGNAL(valueChanged(int)),m_slider_ss, SLOT(setValue(int)));
			connect(m_slider_ss, SIGNAL(valueChanged(QString)),a_QLineEdit, SLOT(setText(QString)));

			toolLayout->addWidget(a_label);
			toolLayout->addWidget(a_QLineEdit);
			toolLayout->addWidget(sld_iterations);
			
			m_txt_sliders.push_back(a_QLineEdit);
			m_sliders.push_back(sld_iterations);
		}
	}

	QPushButton * cmd_configure = new QPushButton("Configure", this);
	connect(cmd_configure, SIGNAL(clicked()),this, SLOT(configure()));

	QPushButton * cmd_iterate = new QPushButton("Iterate", this);
	connect(cmd_iterate, SIGNAL(clicked()),this, SLOT(iterate()));

	toolLayout->addWidget(cmd_configure);
	toolLayout->addWidget(cmd_iterate);

	setLayout(toolLayout);
}

sjPluginWidget::~sjPluginWidget(){

}

std::string sjPluginWidget::getName(){
	return name;
}

QSlider * sjPluginWidget::getSlider(sj::sjParameterBase * param){
	QSlider * a_slider = new QSlider(Qt::Horizontal, this);
	double min_value, max_value, default_value, step_value;

	min_value = (double)((sjDoubleParam *)param)->getMinValue();
	max_value = (double)((sjDoubleParam *)param)->getMaxValue();
	default_value = (double)((sjDoubleParam *)param)->getDefaultValue();
	step_value = (double)((sjDoubleParam *)param)->getStep();

	int i_max;
	if(step_value !=0) i_max = (int)((max_value-min_value)/ step_value);
	else i_max = 1;
	a_slider->setMinimum(0);
	a_slider->setMaximum(i_max );
	a_slider->setSingleStep(1);
	a_slider->setValue((int)((default_value-min_value)/ step_value));

	return a_slider ;
}

void sjPluginWidget::configure(){
	if(mesh_filter == NULL) return;
	//mesh_filter->configure();
	mesh_filter->transform();
}

void sjPluginWidget::iterate(){
	if(mesh_filter == NULL) return;
	//mesh_filter->iterate();
	mesh_filter->transform();
	sjPolyhedronPipe::sjPipe * pipe;
	mesh_filter->setOutputPipe(pipe);
	//emit getMesh(mesh_filter->);
	emit getMesh(*(pipe->read()));
}