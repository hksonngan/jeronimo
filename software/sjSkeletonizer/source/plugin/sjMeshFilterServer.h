#ifndef __SJMESHFILTERSERVER__H__
#define __SJMESHFILTERSERVER__H__

#include "sjEngine.h"
#include "sjMeshFilter.h"
#include <vector>
#include <string>

namespace sj {
    
class sjMeshFilterServer {
  public:
    
    class sjMeshFilterPlugin {
      public:
        virtual ~sjMeshFilterPlugin() {}
		virtual char * getName() = 0;
        virtual sjMeshFilter * createMeshFilter() = 0;
    };

    /// Destructor
    SJENGINE_API ~sjMeshFilterServer() {
      for(sjMeshFilterVector::reverse_iterator It = m_sjMeshFilterVector.rbegin();
          It != m_sjMeshFilterVector.rend();
          ++It)
        delete *It;
    }

    //SJENGINE_API 
	void addsjMeshFilter(std::auto_ptr<sjMeshFilterPlugin> MF) {
      m_sjMeshFilterVector.push_back(MF.release());
    }

    SJENGINE_API size_t getsjMeshFilterCount() const {
      return m_sjMeshFilterVector.size();
    }
	
    SJENGINE_API sjMeshFilterPlugin &getsjMeshFilter(size_t Index) {
      return *m_sjMeshFilterVector.at(Index);
    }

	SJENGINE_API sjMeshFilterPlugin &getkMeshFilter(std::string FilterName) {
		size_t tam = m_sjMeshFilterVector.size();
		size_t it;
		for(it = 0; it<tam; it++){
			std::string cad = (*m_sjMeshFilterVector.at(it)).getName();
			if(cad.compare(FilterName) == 0){
				return *m_sjMeshFilterVector.at(it);
			}
		}
		sj::sjMeshFilterServer::sjMeshFilterPlugin * mNULL = NULL;
      return *mNULL;
    }

	SJENGINE_API bool existMeshFilter(std::string FilterName)
	{
		size_t tam = m_sjMeshFilterVector.size();
		size_t it;
		for(it = 0; it<tam; it++){
			std::string cad = (*m_sjMeshFilterVector.at(it)).getName();
			if(cad.compare(FilterName) == 0){
				return true;
			}
		}
		return false;
	}

  private:
    /// A vector of graphics drivers
    typedef std::vector<sjMeshFilterPlugin *> sjMeshFilterVector;

    sjMeshFilterVector m_sjMeshFilterVector; ///< All available graphics drivers
};

} // namespace sj

#endif // __SJMESHFILTERSERVER__H__