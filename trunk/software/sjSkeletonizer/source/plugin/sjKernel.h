#ifndef __SJKERNELENGINE__H__
#define __SJKERNELENGINE__H__

#include "sjEngine.h"
#include "sjMeshFilterServer.h"
#include "sjPluginXmlLoader.h"

#include <string>
#include <map>

namespace sj {

/// The engine's system core
class sjKernelEngine {
	public:

	SJENGINE_API sjMeshFilterServer &getsjMeshFilterServer()
    { 
		return m_sjMeshFilterServer; 
	}
	
    /// Loads a plugin
    SJENGINE_API bool loadPlugin(const std::string &sFilename) {
		if(m_LoadedPlugins.find(sFilename) == m_LoadedPlugins.end())
		{
			sjPluginXmlLoader aPlugin(sFilename);
			m_LoadedPlugins.insert(
				PluginMap::value_type( sFilename, aPlugin )
								   ).first->second.registerPlugin(*this);
			return true;
		}
		return false;
    }
    
  private:
    /// Map of plugins by their associated file names
    typedef std::map<std::string, sjPluginXmlLoader> PluginMap;    
    PluginMap       m_LoadedPlugins;  ///< All plugins currently loaded
    sjMeshFilterServer m_sjMeshFilterServer; 

};

} // namespace sj

#endif