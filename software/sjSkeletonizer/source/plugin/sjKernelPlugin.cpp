#include "sjKernelPlugin.h"

using namespace sj;

sjKernelPlugin sjKernelPlugin::_instance;

sjKernelPlugin::sjKernelPlugin(void)
{
}

sjKernelPlugin::~sjKernelPlugin(void)
{
}

sjKernelPlugin::sjKernelPlugin(const sjKernelPlugin & Other)
{
}

sjExternalServer * sjKernelPlugin::getExternalServer(std::string key) 
{
	if(external_servers.size() >0)
		if(external_servers.find(key) != external_servers.end())
			return ((external_servers.find(key))->second);
	return NULL;
}

sjPlugin * sjKernelPlugin::getPlugin(std::string key) 
{
	if(m_plugins.size() >0)
		if(m_plugins.find(key) != m_plugins.end())
			return ((m_plugins.find(key))->second);
	return NULL;
}

bool sjKernelPlugin::registerExternalServer(std::string key, sjExternalServer * ext_server)
{
	if(key.length() == 0) return false;
	if(ext_server == NULL ) return false;
	if(external_servers.size() >0)
		if(external_servers.find(key) != external_servers.end())
			return false;
	external_servers.insert(TypeMapExternalServer::value_type(key, ext_server));
	return true;
}

bool sjKernelPlugin::addPlugin(sjPlugin * a_plugin)
{
	if(a_plugin == NULL ) return false;
	if(a_plugin->getName().length() == 0) return false;
	if(m_plugins.size() >0)
		if(m_plugins.find(a_plugin->getName()) != m_plugins.end())
			return false;
	m_plugins.insert(TypeMapPlugin::value_type(a_plugin->getName(), a_plugin));
	return true;
}