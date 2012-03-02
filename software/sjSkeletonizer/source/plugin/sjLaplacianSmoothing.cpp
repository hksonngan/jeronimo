#include "sjLaplacianSmoothing.h"

#include <cmath>  
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "sjDataType.h"
#include "sjUtils.h"
#include "sjPipeFilter.h"
#include "sjStateContext.h"
#include "sjPlugin.h"
#include "sjKernelPlugin.h"

#include <CGAL/Timer.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/IO/Polyhedron_scan_OFF.h> 

#include <OGF/basic/types/counted.h>
#include <OGF/math/attributes/attribute_adapter.h>
#include <OGF/math/linear_algebra/vector.h>
#include <OGF/math/linear_algebra/matrix.h>
#include <OGF/math/geometry/types.h>
#include <OGF/math/numeric/linear_solver.h>

using namespace std;
using namespace OGF;

namespace sj{
	class BasicFilter{
	public:
		BasicFilter():
			WH_0(1.0), 
			WL_0(1.0), 
			SL(0), 
			iteration(0), 
			AVERAGE_FACE(0.0),
			MAX_CONTRACTION_SQUARED( 100000.),
			MAX_ATTRACTION(100000000.),
			MIN_COT_ANGLE(0.00000000000000001)		{
		}
		vector< vector<sjVertex_handle> > rings;
		double WH_0 ;
		double WL_0 ;
		double SL ;
		double AVERAGE_FACE;
		double MAX_CONTRACTION_SQUARED;	// Maximum contraction weight^2 before using hard constraint (default 100000)
		double MAX_ATTRACTION;		// Maximum attraction weight before using hard constraint    (default 1000)
		double MIN_COT_ANGLE;	// Minimum angle for cotan weights before clamping           (default 0.0000001)
		int iteration;
	};

	class InitIndex: public sjState{
	public:
		InitIndex(BasicFilter * a_data):
		  m_data(a_data){
		}

		void proccesEvent(sjEvent * evt){}

		bool initialize(sjStateContext * ssc){
			m_context = ssc;
			return true;
		}

		bool evolve(sjStateContext * ssc){
			int i = 0;
			sjPolyhedronPipe::PolyhedronType mesh_G;
			mesh_G = m_context->getMesh();
			for ( sjVIterator v1 = mesh_G.vertices_begin(); v1 != mesh_G.vertices_end(); ++v1){
				sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
				vcir->vertex()->index = i;
				vcir->vertex()->initial_ring_area = 0.0;
				i++;
			}
			return true;
		}

	private:
		BasicFilter * m_data;
		sjStateContext * m_context;

	};

	class PluginInitIndex: public sjPlugin{
	public:
		PluginInitIndex() {
			name = "PluginInitIndex";
			name_type = sjKernelPlugin.getInstance().INIT_INDEX_SYSTEM;
			registerPlugin(sjKernelPlugin.getInstance());
		}

		void registerPlugin(sjKernelPlugin & K){

		}
	};

}

