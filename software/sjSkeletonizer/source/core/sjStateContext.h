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
		void setRings (vector< vector<sjVertex_handle> > a_rings);
		void setParameters(sjPolyhedronPipe::ParametersType *);
		void setState(sjState * a_state);    
		
		sjPolyhedronPipe::PolyhedronType * iterate();
		sjPolyhedronPipe::PolyhedronType getMesh();
		vector< vector<sjVertex_handle> > getRings();
		size_t getNumberOfVertex();
		sjPolyhedronPipe::ParametersType * getParameters();
		sjState * getState();

		bool evolve(int delta_frame);    

	protected:
		sjPolyhedron mesh_G;
		vector< vector<sjVertex_handle> > m_rings;
		sjPolyhedronPipe::ParametersType * m_params;
		sjState * state;
	};
}

#endif //__SJSTATECONTEXT__H__