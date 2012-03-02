#ifndef __SJSTATECONTEXT__H__
#define __SJSTATECONTEXT__H__

#include "sjPipeFilter.h"
#include "sjObserver.h"
#include "sjState.h"

namespace sj{
	
	class sjStateContext: public sjPolyhedronPipe::sjFilter, public sjObserver{
	public:		
		sjStateContext();
		
		void proccesEvent(sjEvent * evt);
		void setMesh(sjPolyhedronPipe::PolyhedronType);
		void setParameters(sjPolyhedronPipe::ParametersType *);
		void setState(sjState * a_state);    
		
		sjPolyhedronPipe::PolyhedronType * iterate();
		sjPolyhedronPipe::PolyhedronType getMesh();
		size_t getNumberOfVertex();
		sjPolyhedronPipe::ParametersType * getParameters();
		sjState * getState();

		bool evolve(int delta_frame);    

	protected:
		sjPolyhedron mesh_G;
		sjPolyhedronPipe::ParametersType * m_params;
		sjState * state;
	};
}

#endif //__SJSTATECONTEXT__H__