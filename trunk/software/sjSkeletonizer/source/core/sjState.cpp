#include "sjState.h"
#include "sjStateContext.h"

using namespace sj;

sjState::sjState(){
	m_context = NULL;
	mesh_G = &sjStateContext::mesh_G;
	rings = &sjStateContext::m_rings;
}

bool sjState::initialize(sjStateContext * ssc){
	this->m_context = ssc;
	return true;
}

void sjState::proccesEvent(sjEvent * evt){
}