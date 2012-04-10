#include "sjStateContext.h"
#include "sjKernelPlugin.h"
#include "sjLog.h"
#include "sjState.h"

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
}

bool sjStateContext::evolve(int delta_frame){
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
	sjLogDebug("sjStateContext::proccesEvent 1\n");
	if(evt->getType() == sjEvent::EVT_ITERATE){
		sjLogDebug("sjStateContext::proccesEvent 2\n");
		this->update();
		//this->iterate();
		sjLogDebug("sjStateContext::proccesEvent 3\n");
	}else{	
		sjLogDebug("sjStateContext::proccesEvent 4\n");
		state->proccesEvent(evt);}
	sjLogDebug("sjStateContext::proccesEvent 5\n");
}

sjPolyhedronPipe::PolyhedronType sjStateContext::iterate(){
	if(m_init_system == false){
		this->mesh_G = input_pipe->read();
		m_init_system = true;
		return this->getMesh();
	}
	if(this->evolve(1)){
		int sizenn = getNumberOfVertex();
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