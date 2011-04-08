#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJEXTERNALSERVER__H__
#define __SJEXTERNALSERVER__H__

#include <vector>

namespace sj{

	class sjSystem{
	public:
		sjSystem(){}
		virtual ~sjSystem(){}
	};
	
	class sjSystemDriver {
	public:
			virtual ~sjSystemDriver() {}
			virtual const std::string &getName() const = 0;
			virtual sjSystem * createSystem();			
	};

	class sjExternalServer{
	public:
		sjExternalServer();
		sjExternalServer(const sjExternalServer &Other);
		virtual ~sjExternalServer();
		
		bool addSystemDriver(sjSystemDriver * ptr_system_driver);
		size_t getSystemDriverCount() const;
		sjSystemDriver * getSystemDriver(size_t index);
		sjSystemDriver * getSystemDriver(std::string filter_name);
		bool exiSystemDriver(std::string filter_name);
	protected:
		typedef std::vector<sjSystemDriver *> sjSystemDriverVector;
		sjSystemDriverVector m_system_driver_vector; ///< All available systems drivers
	};

}	//namespace sj
#endif //__SJEXTERNALSERVER__H__