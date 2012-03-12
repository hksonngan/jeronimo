#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJKERNELPLUGIN__H__
#define __SJKERNELPLUGIN__H__

#include <map>
#include "sjPlugin.h"
#include "sjSystem.h"
#include "sjObserver.h"

namespace sj{

	class sjKernelPlugin: public sjSubject{
	public:
		static const std::string SYS_BASIC_FILTER_SYSTEM;
		static const std::string SYS_COMPUTE_LAPLACIAN_SYSTEM;
		static const std::string SYS_COMPUTE_LINE_EQUATIONS_SYSTEM;
		static const std::string SYS_COMPUTE_MEAN_CURVATURE_SYSTEM;
		static const std::string SYS_COMPUTE_RINGS_SYSTEM;
		static const std::string SYS_INIT_LAPLACIAN_SMOOTHING_SYSTEM;
		static const std::string SYS_INIT_INDEX_SYSTEM ;
		static const std::string SYS_IS_DEGENERATE_VERTEX_SYSTEM;
		static const std::string SYS_ITERATE_SMOOTHING_ALGORITHM_SYSTEM;
		static const std::string SYS_MEAN_CURVATURE_SMOOTHING_SYSTEM;
	private:
		static sjKernelPlugin _instance;
		sjKernelPlugin (void);
		sjKernelPlugin (const sjKernelPlugin &);
		sjKernelPlugin & operator=(const sjKernelPlugin &);
		~sjKernelPlugin (void);
		typedef std::map<std::string, sjPlugin * > TypeMapPlugin;
		typedef std::map<std::string, sjSystem * > TypeMapsjSystem;
		TypeMapPlugin m_plugins;
		TypeMapsjSystem m_systems;
	public:
		static sjKernelPlugin &getInstance(void){
			return _instance;
		}
		
		bool addPlugin(sjPlugin *);
		bool setDefaultSystem(std::string plugin_name);
		bool setDefaultSystem(sjPlugin * m_plugin);
		bool existSystem(std::string system_name);

		sjPlugin * getPlugin(std::string key);
		sjSystem * getSystem(std::string system_name);
	};

}


#endif //__SJKERNELPLUGIN__H__