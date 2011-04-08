#if defined (WIN32) || defined (_WIN32_)
	#pragma once
#endif

#ifndef __SJKERNELPLUGIN__H__
#define __SJKERNELPLUGIN__H__

#include <map>
#include "sjPlugin.h"
#include "sjExternalServer.h"

namespace sj{

	class sjKernelPlugin{
	private:
		static sjKernelPlugin _instance;
		sjKernelPlugin(void);
		~sjKernelPlugin(void);
		sjKernelPlugin(const sjKernelPlugin &);
		sjKernelPlugin & operator=(const sjKernelPlugin &);
	private:
		typedef std::map<std::string, sjExternalServer * > TypeMapExternalServer;
		TypeMapExternalServer external_servers;
		typedef std::map<std::string, sjPlugin * > TypeMapPlugin;
		TypeMapPlugin m_plugins;
	public:
		static sjKernelPlugin &getInstance(void){
			return _instance;
		}
		sjExternalServer * getExternalServer(std::string key);
		sjPlugin * getPlugin(std::string key);
		bool registerExternalServer(std::string key, sjExternalServer *);
		bool addPlugin(sjPlugin *);
	
	};


}


#endif //__SJKERNELPLUGIN__H__