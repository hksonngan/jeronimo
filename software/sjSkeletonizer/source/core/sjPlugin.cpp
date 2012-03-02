#include "sjPlugin.h"

using namespace sj;

sjPlugin::sjPlugin()
:m_pfnRegisterPlugin(0)
{

}

sjPlugin::sjPlugin(const sjPlugin &Other)
:m_pfnRegisterPlugin(Other.m_pfnRegisterPlugin), name(Other.name)
{
}

sjPlugin::~sjPlugin()
{
}

void sjPlugin::setName(std::string a_name)
{
	name = a_name;
}

std::string sjPlugin::getName()
{
	return name;
}

void sjPlugin::registerPlugin(sjKernelPlugin & K)
{
	if(m_pfnRegisterPlugin == NULL)return;
	m_pfnRegisterPlugin(K, name);
}