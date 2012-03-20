#ifndef __SJLAPLACE_SMOOTH__H__
#define __SJLAPLACE_SMOOTH__H__

#include "sjPipeFilter.h"
#include "sjKernelPlugin.h"
#include "sjStateContext.h"
#include "sjCoreSmoothing.h"
#include "sjState.h"
#include "sjDataIO.h"

#include <string>

using namespace std;

namespace sj{

	class ComputeLineEquations: public sjSystem{
	public:
		void proccesEvent(sjEvent * evt){}
		sjVIterator computeLineEquations(sjVIterator vi, vector< sjVertex_handle> neighbors);
	};
	class PluginComputeLineEquations:public sjPlugin{
	public:
		PluginComputeLineEquations();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IterateLaplacianSmoothingIntegrate: public IterateSmoothingAlgorithm{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginIterateLaplacianSmoothingIntegrate: public sjPlugin{
	public:
		PluginIterateLaplacianSmoothingIntegrate();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

}
#endif
