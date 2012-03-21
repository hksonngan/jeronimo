#ifndef __MEANCURVE_SMOOTH__H__
#define __MEANCURVE_SMOOTH__H__

#include "sjPipeFilter.h"
#include "sjKernelPlugin.h"
#include "sjStateContext.h"
#include "sjCoreSmoothing.h"
#include "sjState.h"
#include "sjDataIO.h"

#include <string>

using namespace std;

namespace sj{
	class ComputeMeanCurvature: public ComputeWeight{
	public:
		map<int, double> computeWeight(sjStateContext * m_ctx, sjVIterator vi, vector< sjVertex_handle> neighbors);
	};
	class PluginComputeMeanCurvature: public sjPlugin{
		public:
		PluginComputeMeanCurvature();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class MeanCurvatureSmoothing: public IterateSmoothingAlgorithm{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginMeanCurvatureSmoothing: public sjPlugin{
	public:
		PluginMeanCurvatureSmoothing();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};
}

#endif //__SJLAPLACE_SMOOTH__H__