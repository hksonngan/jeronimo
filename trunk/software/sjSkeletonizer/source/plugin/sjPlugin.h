#ifndef __SJPLUGIN__H__
#define __SJPLUGIN__H__

#include <string>

namespace sj{

	class sjKernelPlugin;
	class sjPlugin{
	public:
		sjPlugin();
		sjPlugin(const sjPlugin &Other);
		virtual ~sjPlugin();
		void setName(std::string a_name);
		std::string getName();
		void registerPlugin(sjKernelPlugin & K);
	protected:
		std::string name;
		sjPlugin &operator =(const sjPlugin &Other);
		typedef void fnRegisterPlugin(sjKernelPlugin &, std::string );
		fnRegisterPlugin * m_pfnRegisterPlugin;
	};
}


#endif //__SJPLUGIN__H__