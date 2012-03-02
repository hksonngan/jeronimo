#include "sjPlugin.h"

using namespace sj;

sjPlugin::sjPlugin()
:m_pfnRegisterPlugin(0){

}

sjPlugin::sjPlugin(const sjPlugin &Other)
:m_pfnRegisterPlugin(Other.m_pfnRegisterPlugin), name(Other.name){
}

sjPlugin::~sjPlugin(){
}

std::string sjPlugin::getName(){
	return name;
}

std::string sjPlugin::getNameType(){
	return name_type;
}

std::string sjPlugin::getPluginName(){
        return name_type + "->" + name;
}

void sjPlugin::registerPlugin(sjKernelPlugin & K)
{
	if(m_pfnRegisterPlugin == NULL)return;
	m_pfnRegisterPlugin(K, name);
}