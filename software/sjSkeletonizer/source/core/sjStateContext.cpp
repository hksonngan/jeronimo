#include "sjStateContext.h"
#include "sjKernelPlugin.h"

using namespace sj;

sjStateContext::sjStateContext():
state(0), 
m_params(0){
	state = (sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_INIT_INDEX_SYSTEM));
	m_init_system = false;
}

bool sjStateContext::evolve(int delta_frame){
	return state->evolve(this);
}

void sjStateContext::setState(sjState * a_state){
	state  = a_state;
}

sjState * sjStateContext::getState(){
	return state;
}

void sjStateContext::proccesEvent(sjEvent * evt){
	if(evt->getType() == sjEvent::EVT_ITERATE){
		this->iterate();
	}else{	
		state->proccesEvent(evt);}
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

void sjStateContext::setRings (vector< vector<sjVertex_handle> > a_rings){
	m_rings = a_rings;
}

vector< vector<sjVertex_handle> > sjStateContext::getRings(){
	return m_rings ;
}