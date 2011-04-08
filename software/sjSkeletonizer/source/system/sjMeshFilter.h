#ifndef __SJMESHFILTER__H__
#define __SJMESHFILTER__H__

#include "sjDataType.h"
#include "sjParameter.h"

namespace sj{
	class sjMeshFilter {
	public:
		virtual ~sjMeshFilter() {}
		virtual char * getName() = 0;
		virtual void setMesh(sjPolyhedron) = 0;
		virtual sjPolyhedron getMesh() = 0;
		virtual void configure() = 0;
		virtual void iterate() = 0;
		virtual void setParameters(sjParameterStore *) = 0;
		virtual sjParameterStore * getParameters() = 0;
	};
}
#endif