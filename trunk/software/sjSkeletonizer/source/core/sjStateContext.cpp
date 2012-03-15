#include "sjStateContext.h"
#include "sjKernelPlugin.h"

using namespace sj;

sjStateContext::sjStateContext():
state(0), 
m_params(0),
WH_0(1.0), 
WL_0(1.0), 
SL(0), 
iteration(0), 
AVERAGE_FACE(0.0),
MAX_CONTRACTION_SQUARED( 100000.),
MAX_ATTRACTION(100000000.),
MIN_COT_ANGLE(0.00000000000000001){
	m_init_system = false;
	state = (sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_INIT_INDEX_SYSTEM));
	state->initialize(this);
	m_rings = new vector< vector< sjVertex_handle > >();
}

bool sjStateContext::evolve(int delta_frame){
	printf("sjStateContext::evolve 1\n");
	return state->evolve(this);
}

void sjStateContext::setState(sjState * a_state){
	state  = a_state;
	state->initialize(this);
}

sjState * sjStateContext::getState(){
	return state;
}

void sjStateContext::proccesEvent(sjEvent * evt){
	printf("sjStateContext::proccesEvent 1\n");
	if(evt->getType() == sjEvent::EVT_ITERATE){
		printf("sjStateContext::proccesEvent 2\n");
		this->update();
		//this->iterate();
		printf("sjStateContext::proccesEvent 3\n");
	}else{	
		printf("sjStateContext::proccesEvent 4\n");
		state->proccesEvent(evt);}
	printf("sjStateContext::proccesEvent 5\n");
}

sjPolyhedronPipe::PolyhedronType sjStateContext::iterate(){
	printf("PolyhedronType iterate 1\n");
	if(m_init_system == false){
		printf("PolyhedronType iterate 2\n");
		this->mesh_G = input_pipe->read();
		printf("PolyhedronType iterate 3\n");
		m_init_system = true;
		return this->getMesh();
	}
	printf("PolyhedronType iterate 4\n");
	if(this->evolve(1)){
		printf("PolyhedronType iterate 5\n");
		return this->getMesh();
	}else{
		//No more iterations
	}
	return sjPolyhedronPipe::PolyhedronType();
}

void sjStateContext::setParameters(sjPolyhedronPipe::ParametersType * a_params){
	m_params = a_params;
}

sjPolyhedronPipe::ParametersType * sjStateContext::getParameters(){
	return this->m_params;
}

void sjStateContext::setMesh(sjPolyhedronPipe::PolyhedronType meshG){	
	this->mesh_G = meshG;
}

size_t sjStateContext::getNumberOfVertex(){
	return mesh_G.size_of_vertices();
}

sjPolyhedronPipe::PolyhedronType sjStateContext::getMesh(){
	return mesh_G;
}

void sjStateContext::setRings (vector< vector<sjVertex_handle> > * a_rings){
	m_rings = a_rings;
}

vector< vector<sjVertex_handle> > * sjStateContext::getRings(){
	return m_rings ;
}