#ifndef __SJPLUGIN__H__
#define __SJPLUGIN__H__

#include <string>
#include "sjSystem.h"

namespace sj{

	class sjKernelPlugin;
	class sjPlugin{
	public:
		sjPlugin();
		sjPlugin(const sjPlugin &Other);
		virtual ~sjPlugin();
		std::string getName();
		std::string getNameType();
		std::string getPluginName();
		virtual void registerPlugin(sjKernelPlugin & K);
		virtual sjSystem * createSystem() = 0;
	protected:
		std::string name;
		std::string name_type;
		sjPlugin &operator =(const sjPlugin &Other);
		typedef void fnRegisterPlugin(sjKernelPlugin &, std::string );
		fnRegisterPlugin * m_pfnRegisterPlugin;
	};
}


#endif //__SJPLUGIN__H__