#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJOBSERVER__H__
#define __SJOBSERVER__H__

#include <vector>

namespace sj{
	class sjEvent{
	public:
		enum TYPES{
			EVT_NONE = 1,
			EVT_ITERATE = 2
		};
		sjEvent(){
			m_type = EVT_NONE;
		}

		sjEvent(TYPES a_type){
			this->m_type = a_type;
		}
		void setType(TYPES a_type){
			this->m_type = a_type;
		}
		TYPES getType(){
			return m_type;
		}
		template<class C>
		C * getData() = 0;
	private:
		TYPES m_type;
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
		void dispatch(sjEvent * evt);
		void clear();
	protected:
		std::vector < sjObserver * > observers;
	};
}
#endif //__SJOBSERVER__H__