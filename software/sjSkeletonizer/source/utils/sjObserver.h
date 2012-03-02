#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJOBSERVER__H__
#define __SJOBSERVER__H__

#include <vector>

namespace sj{
	class sjEvent{
		template<class C>
		C * getData() = 0;
	};

	class sjObserver
	{
	public:
		virtual void proccesEvent(sjEvent * evt) = 0;
	};

	class sjSubject
	{
	public:
		void attach( sjObserver* observer );
		void detach( sjObserver* observer );
		void notify(sjEvent * evt);
	protected:
		std::vector < sjObserver * > observers;
	};
}
#endif //__SJOBSERVER__H__