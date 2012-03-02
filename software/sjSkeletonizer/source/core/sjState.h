#ifndef __SJSTATE__H__
#define __SJSTATE__H__

#include "sjSystem.h"

namespace sj{
	class sjStateContext;
	class sjState: public sjSystem{
	public:
		virtual void proccesEvent(sjEvent * evt) = 0;
		virtual ~sjState(){}
		virtual bool initialize(sjStateContext * ssc) = 0;
		virtual bool evolve(sjStateContext * ssc) = 0;
	};
}


#endif //__SJSTATE__H__