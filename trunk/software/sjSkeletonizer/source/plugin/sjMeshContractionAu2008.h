#ifndef __SJMESH_CONTRACTION__H__
#define __SJMESH_CONTRACTION__H__

#include "sjPipeFilter.h"
#include "sjKernelPlugin.h"
#include "sjStateContext.h"
#include "sjCoreSmoothing.h"
#include "sjState.h"
#include "sjDataIO.h"

#include <string>

using namespace std;

namespace sj{


	class ComputeLaplacian: public sjSystem{
	public:
		void proccesEvent(sjEvent * evt){}
		map<int, double> computeLaplacian(sjStateContext * m_ctx, sjVIterator vi, vector< sjVertex_handle> neighbors);
	};
	class PluginComputeLaplacian: public sjPlugin{
	public:
		PluginComputeLaplacian();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IsDegenerateVertex: public sjSystem{
	public:
		IsDegenerateVertex();
		void proccesEvent(sjEvent * evt){}
		bool isDegenerateVertex(sjStateContext * m_ctx, sjVIterator vi, vector< sjVertex_handle> neighbors);
	private:
		ComputeLaplacian * m_comlapl;
	};

	class PluginIsDegenerateVertex: public sjPlugin{
	public:
		PluginIsDegenerateVertex();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IterateLaplacianSmoothingSeparate: public IterateSmoothingAlgorithm{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginIterateLaplacianSmoothingSeparate: public sjPlugin{
	public:
		PluginIterateLaplacianSmoothingSeparate();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};
}

#endif //__SJMESH_CONTRACTION__H__