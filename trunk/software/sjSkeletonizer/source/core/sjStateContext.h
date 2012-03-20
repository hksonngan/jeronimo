#ifndef __SJSTATECONTEXT__H__
#define __SJSTATECONTEXT__H__

#include "sjPipeFilter.h"
#include "sjObserver.h"
#include "sjState.h"

namespace sj{
	//class sjState;
	
	class sjStateContext: public sjPolyhedronPipe::sjFilter, public sjObserver{
	public:		
		sjStateContext();
		
		void proccesEvent(sjEvent * evt);
		void setMesh(sjPolyhedronPipe::PolyhedronType );
		void setRings (vector< vector<sjVertex_handle> > a_rings);
		void setParameters(sjPolyhedronPipe::ParametersType *);
		void setState(sjState * a_state);    
		
		sjPolyhedronPipe::PolyhedronType iterate();
		sjPolyhedronPipe::PolyhedronType getMesh();
		vector< vector<sjVertex_handle> > getRings();
		size_t getNumberOfVertex();
		sjPolyhedronPipe::ParametersType * getParameters();
		sjState * getState();

		bool evolve(int delta_frame);    

		sjPolyhedron mesh_G;
		vector< vector< sjVertex_handle > > m_rings;
		sjPolyhedronPipe::ParametersType * m_params;
		sjState * state;
		bool m_init_system;

		double WH_0 ;
		double WL_0 ;
		double SL ;
		double AVERAGE_FACE;
		double MAX_CONTRACTION_SQUARED;	// Maximum contraction weight^2 before using hard constraint (default 100000)
		double MAX_ATTRACTION;		// Maximum attraction weight before using hard constraint    (default 1000)
		double MIN_COT_ANGLE;	// Minimum angle for cotan weights before clamping           (default 0.0000001)
		int iteration;
	};
}

#endif //__SJSTATECONTEXT__H__