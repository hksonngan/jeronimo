#ifndef __SJSYSTEM__H__
#define __SJSYSTEM__H__

#include "sjObserver.h"

namespace sj{
	class sjSystem: public sjObserver{
	public:
		virtual void proccesEvent(sjEvent * evt) = 0;
		virtual ~sjSystem(){}
	};
}


#endif //__SJSYSTEM__H__