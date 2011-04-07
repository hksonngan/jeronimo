#include "sjPluginWidget.h"

sjPluginWidget::sjPluginWidget(sjMeshFilter * a_mesh_filter, QWidget * parent, Qt::WindowFlags f):
QWidget(parent, f), mesh_filter (0){
	mesh_filter = a_mesh_filter;

	name = std::string( mesh_filter->getName());
	unsigned int i = 0;
	unsigned int n = mesh_filter->getParameters()->getNumberOfParameters();

	QVBoxLayout *toolLayout = new QVBoxLayout;

	for(i = 0; i<n; i++){
		sjParameterBase * a_param = mesh_filter->getParameters()->getParameter(i);
		if(a_param->getType() >= 10 && a_param->getType() < 100){
			QLineEdit * a_QLineEdit = new QLineEdit("", this);
			QSlider * sld_iterations = getSlider(a_param);
			QLabel * a_label = new QLabel(QString(a_param->getName().c_str()));
			sjSliderSlotSignal * m_slider_ss = new sjSliderSlotSignal();
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
	int min_value, max_value, default_value, step_value;
	switch(param->getType()){
		case SJ_DATA_TYPE_CHAR:					
			GET_VALUES_NUMERIC_PARAM(sjCharParam)
			break;
		case SJ_DATA_TYPE_DOUBLE:				
			GET_VALUES_NUMERIC_PARAM(sjDoubleParam)
			break;
		case SJ_DATA_TYPE_FLOAT:				
			GET_VALUES_NUMERIC_PARAM(sjFloatParam)
			break;
		case SJ_DATA_TYPE_INT:					
			GET_VALUES_NUMERIC_PARAM(sjIntParam)
			break;
		case SJ_DATA_TYPE_LONG:					
			GET_VALUES_NUMERIC_PARAM(sjLongParam)
			break;
		case SJ_DATA_TYPE_LONG_DOUBLE:			
			GET_VALUES_NUMERIC_PARAM(sjLongDoubleParam)
			break;
		case SJ_DATA_TYPE_SHORT:				
			GET_VALUES_NUMERIC_PARAM(sjShortParam)
			break;
		case SJ_DATA_TYPE_UNSIGNED_CHAR:		
			GET_VALUES_NUMERIC_PARAM(sjUnsignedCharParam)
			break;
		case SJ_DATA_TYPE_UNSIGNED_INT:			
			GET_VALUES_NUMERIC_PARAM(sjUnsignedIntParam)
			break;
		case SJ_DATA_TYPE_UNSIGNED_LONG:		
			GET_VALUES_NUMERIC_PARAM(sjUnsignedLongParam)
			break;
		case SJ_DATA_TYPE_UNSIGNED_SHORT:		
			GET_VALUES_NUMERIC_PARAM(sjUnsignedShortParam)
			break;
	}

	a_slider->setMinimum(min_value);
	a_slider->setMaximum(max_value);
	a_slider->setSingleStep(step_value);
	a_slider->setValue(default_value);

	return a_slider ;
}

void sjPluginWidget::configure(){
	if(mesh_filter == NULL) return;
	mesh_filter->configure();
}

void sjPluginWidget::iterate(){
	if(mesh_filter == NULL) return;
	mesh_filter->iterate();
	emit getMesh(mesh_filter->getMesh());
}