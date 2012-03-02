#ifndef __SJSTATECONTEXT__H__
#define __SJSTATECONTEXT__H__

#include "sjPipeFilter.h"
#include "sjObserver.h"
#include "sjState.h"

namespace sj{

	
	class sjStateContext: public sjPolyhedronPipe::sjFilter, public sjObserver{
	public:		
		bool evolve(int delta_frame);    
		void setState(sjState * a_state);    
		sjState * getState();
		void proccesEvent(sjEvent * evt);
		sjPolyhedronPipe::PolyhedronType * iterate();
		void setParameters(sjPolyhedronPipe::ParametersType *);
		sjPolyhedronPipe::ParametersType * getParameters();
	protected:
		sjState * state;
	};
}


#endif //__SJSTATECONTEXT__H__