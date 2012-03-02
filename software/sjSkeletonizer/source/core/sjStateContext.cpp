#include "sjStateContext.h"

using namespace sj;

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
	state->proccesEvent(evt);
}

sjPolyhedronPipe::PolyhedronType * sjStateContext::iterate(){
	return NULL;
}

void sjStateContext::setParameters(sjPolyhedronPipe::ParametersType *){
}

sjPolyhedronPipe::ParametersType * sjStateContext::getParameters(){
	return NULL;
}