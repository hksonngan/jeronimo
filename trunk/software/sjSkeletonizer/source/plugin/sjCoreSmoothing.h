#ifndef __SJCORE_SMOOTH__H__
#define __SJCORE_SMOOTH__H__

#include "sjPipeFilter.h"
#include "sjKernelPlugin.h"
#include "sjStateContext.h"
#include "sjState.h"
#include "sjDataIO.h"
#include <string>

using namespace std;

namespace sj{
	class InitIndex: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginInitIndex: public sjPlugin{
	public:
		PluginInitIndex();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeRings: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
	};
	class PluginComputeRings:public sjPlugin{
	public:
		PluginComputeRings();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class InitLaplacianSmoothing: public sjState{
	public:
		bool evolve(sjStateContext * ssc);
		void initLaplacianSmoothing(double a_WH0 = 1.0, double a_WL0 = 0.01, double a_SL = 2.0);
	};
	class PluginInitLaplacianSmoothing: public sjPlugin{
		public:
		PluginInitLaplacianSmoothing();

		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class OFFLoaderSource: public sjPolyhedronPipe::sjSource{
	public:
		OFFLoaderSource(string filename);
		sjPolyhedronPipe::PolyhedronType produce();
	private:
		string m_filename;
	};
}

#endif //__SJCORE_SMOOTH__H__