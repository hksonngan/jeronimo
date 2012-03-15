#ifndef __SJSTATE__H__
#define __SJSTATE__H__

#include "sjSystem.h"

namespace sj{
	class sjStateContext;
	class sjState: public sjSystem{
	public:
		virtual ~sjState(){}
		virtual void proccesEvent(sjEvent * evt){}
		virtual bool initialize(sjStateContext * ssc){
			this->m_context = ssc;
			return true;
		}
		virtual bool evolve(sjStateContext * ssc) = 0;
	protected:
		sjStateContext * m_context;
	};
}


#endif //__SJSTATE__H__