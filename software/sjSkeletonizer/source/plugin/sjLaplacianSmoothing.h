#ifndef __SJLAPLACE_SMOOTH__H__
#define __SJLAPLACE_SMOOTH__H__

#include "sjPipeFilter.h"
#include "sjKernelPlugin.h"
#include "sjStateContext.h"
#include "sjState.h"
#include "sjDataIO.h"

#include <string>

using namespace std;

namespace sj{
	class BasicFilter: public sjSystem{
	public:
		BasicFilter();
		void proccesEvent(sjEvent * evt);
		double WH_0 ;
		double WL_0 ;
		double SL ;
		double AVERAGE_FACE;
		double MAX_CONTRACTION_SQUARED;	// Maximum contraction weight^2 before using hard constraint (default 100000)
		double MAX_ATTRACTION;		// Maximum attraction weight before using hard constraint    (default 1000)
		double MIN_COT_ANGLE;	// Minimum angle for cotan weights before clamping           (default 0.0000001)
		int iteration;
	};
	class PluginBasicFilter:public sjPlugin{
	public:
		PluginBasicFilter();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class InitIndex: public sjState{
	public:
		InitIndex();
		void proccesEvent(sjEvent * evt);
		bool initialize(sjStateContext * ssc);
		bool evolve(sjStateContext * ssc);
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	};
	class PluginInitIndex: public sjPlugin{
	public:
		PluginInitIndex();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeRings: public sjState{
	public:
		ComputeRings();
		void proccesEvent(sjEvent * evt);
		bool initialize(sjStateContext * ssc);
		bool evolve(sjStateContext * ssc);
	private:
		sjStateContext * m_context;
	};
	class PluginComputeRings:public sjPlugin{
	public:
		PluginComputeRings();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeLaplacian: public sjSystem{
	public:
		ComputeLaplacian();
		void proccesEvent(sjEvent * evt);
		map<int, double> computeLaplacian(sjVIterator vi, vector< sjVertex_handle> neighbors);
	private:
		BasicFilter * m_data;
	};
	class PluginComputeLaplacian: public sjPlugin{
	public:
		PluginComputeLaplacian();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeMeanCurvature: public sjState{
	public:
		ComputeMeanCurvature();
		void proccesEvent(sjEvent * evt);
		bool initialize(sjStateContext * ssc);
		bool evolve(sjStateContext * ssc);
		sjPoint_3 computeMeanCurvature(sjVIterator vi, vector< sjVertex_handle> neighbors);	
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	};
	class PluginComputeMeanCurvature: public sjPlugin{
		public:
		PluginComputeMeanCurvature();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class MeanCurvatureSmoothing: public sjState{
	public:
		MeanCurvatureSmoothing();
		void proccesEvent(sjEvent * evt);
		bool initialize(sjStateContext * ssc);
		bool evolve(sjStateContext * ssc);
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	};
	class PluginMeanCurvatureSmoothing: public sjPlugin{
	public:
		PluginMeanCurvatureSmoothing();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IsDegenerateVertex: public sjSystem{
	public:
		IsDegenerateVertex();
		void proccesEvent(sjEvent * evt);
		bool isDegenerateVertex(sjVIterator vi, vector< sjVertex_handle> neighbors);
	private:
		BasicFilter * m_data;
		ComputeLaplacian * m_comlapl;
	};
	class PluginIsDegenerateVertex: public sjPlugin{
	public:
		PluginIsDegenerateVertex();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class ComputeLineEquations: public sjSystem{
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	public:
		ComputeLineEquations();
		void proccesEvent(sjEvent * evt);
		void computeLineEquations(sjVIterator vi, vector< sjVertex_handle> neighbors);
	};
	class PluginComputeLineEquations:public sjPlugin{
	public:
		PluginComputeLineEquations();
		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class InitLaplacianSmoothing: public sjState{
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	public:
		InitLaplacianSmoothing();		
		void proccesEvent(sjEvent * evt);
		bool initialize(sjStateContext * ssc);
		bool evolve(sjStateContext * ssc);
		void initLaplacianSmoothing(double a_WH0 = 1.0, double a_WL0 = 0.01, double a_SL = 2.0);
	};
	class PluginInitLaplacianSmoothing: public sjPlugin{
		public:
		PluginInitLaplacianSmoothing();

		void registerPlugin(sjKernelPlugin & K);
		sjSystem * createSystem();
	};

	class IterateSmoothingAlgorithm: public sjState{
	protected:
		BasicFilter * m_data;
		sjStateContext * m_context;
	public:
			virtual ~IterateSmoothingAlgorithm(){}
			virtual void proccesEvent(sjEvent * evt){}
			virtual bool initialize(sjStateContext * ssc){
				m_context = ssc;
				return true;
			}
			virtual bool evolve(sjStateContext * ssc) = 0;
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

	class OFFLoaderSource: public sjPolyhedronPipe::sjSource{
	public:
		OFFLoaderSource(string filename){
			printf("OFFLoaderSource + %s 1\n", filename.c_str());
			m_filename = filename;
			printf("OFFLoaderSource 2\n");
		}
		sjPolyhedronPipe::PolyhedronType * produce(){
			sjDataIO dataio;
			dataio.setFileName(m_filename );
			try{
				dataio.load();
				return &(dataio.getPolyhedronModel());
			}catch(std::exception e) {
			}
			return NULL;
		}
	private:
		string m_filename;
	};
}
#endif
