#ifndef __SJSTATE__H__
#define __SJSTATE__H__

#define STATE_MESH (m_context->*mesh_G)
#define STATE_RINGS (m_context->*rings)

#include "sjSystem.h"
#include "sjStateContext.h"

namespace sj{
	class sjStateContext;
	
	class sjState: public sjSystem{
	public:
		sjState();
		//sjState(){
		//	m_context = NULL;
		//	mesh_G = &sjStateContext::mesh_G;
		//	rings = &sjStateContext::m_rings;
		//}
		virtual ~sjState(){}
		virtual void proccesEvent(sjEvent * evt){}
		virtual bool initialize(sjStateContext * ssc){
			this->m_context = ssc;
			return true;
		}
		virtual bool evolve(sjStateContext * ssc) = 0;
	protected:
		sjStateContext * m_context;
		sjPolyhedron sjStateContext::*mesh_G;
		vector< vector< sjVertex_handle > > sjStateContext::*rings;
	};
}


#endif //__SJSTATE__H__