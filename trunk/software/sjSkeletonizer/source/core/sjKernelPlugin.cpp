#include "sjKernelPlugin.h"

using namespace sj;

sjKernelPlugin sjKernelPlugin::_instance;

sjKernelPlugin::sjKernelPlugin(void){
}

sjKernelPlugin::~sjKernelPlugin(void){
}

sjKernelPlugin::sjKernelPlugin(const sjKernelPlugin & Other){
}

sjPlugin * sjKernelPlugin::getPlugin(std::string key){
	if(m_plugins.size() >0)
		if(m_plugins.find(key) != m_plugins.end())
			return ((m_plugins.find(key))->second);
	return NULL;
}

bool sjKernelPlugin::addPlugin(sjPlugin * a_plugin){
	if(a_plugin == NULL ) return false;
	if(a_plugin->getName().length() == 0) return false;
	if(m_plugins.size() >0)
		if(m_plugins.find(a_plugin->getPluginName()) != m_plugins.end())
			return false;
	m_plugins.insert(TypeMapPlugin::value_type(a_plugin->getPluginName(), a_plugin));
	return true;
}

bool sjKernelPlugin::setDefaultSystem(std::string plugin_name){
	if(plugin_name.length() == 0) return false;
	if(m_plugins.size() >0){
        if(m_plugins.find(plugin_name) != m_plugins.end()){
            sjPlugin * plu = (sjPlugin *)((m_plugins.find(plugin_name))->second);
			m_systems.insert(TypeMapsjSystem::value_type(plu->getNameType(), plu->createSystem()));
            return true;
        }
	}
	return false;
}

bool sjKernelPlugin::setDefaultSystem(sjPlugin * m_plugin){
	return setDefaultSystem(m_plugin->getPluginName());
}

sjSystem * sjKernelPlugin::getSystem(std::string system_name){
	if(m_plugins.size() >0)
		if(m_systems.find(system_name) != m_systems.end())
			return ((m_systems.find(system_name))->second);
	return NULL;
}