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
		InitIndex():
		  m_data(0){
		}

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
			sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
			name_type = kernel.SYS_INIT_INDEX_SYSTEM;
			registerPlugin(kernel);
		}

		void registerPlugin(sjKernelPlugin & K){

		}

		sjSystem * createSystem(){
			return new InitIndex();
		}
	};

	class ComputeRings: public sjState{
	public:
		ComputeRings(){
		}

		void proccesEvent(sjEvent * evt){}

		bool initialize(sjStateContext * ssc){
			m_context = ssc;
			return true;
		}

		bool evolve(sjStateContext * ssc){
			vector< vector<sjVertex_handle> > rings;
			rings = m_context->getRings();
			sjPolyhedronPipe::PolyhedronType mesh_G;
			mesh_G = m_context->getMesh();

			rings.clear();
			for ( sjVIterator v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
				sjHalfedge_vertex_circulator vcir = v->vertex_begin();
				vector< sjVertex_handle> neighbors;
				do{
					sjVertex_handle punto = (vcir->next()->vertex());
					neighbors.push_back(punto);
				}while(++vcir != v->vertex_begin ());
				vcir->vertex()->initial_ring_area = areaRing(v, neighbors);
				rings.push_back(neighbors);
			}
			return true;
		}

	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	};
	class PluginComputeRings:public sjPlugin{
	public:
		PluginComputeRings(){
			name = "PluginComputeRings";
			sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
			name_type = kernel.SYS_COMPUTE_RINGS_SYSTEM;
			registerPlugin(kernel);
		}

		void registerPlugin(sjKernelPlugin & K){

		}

		sjSystem * createSystem(){
			return new ComputeRings();
		}
	};

	class ComputeLaplacian: public sjState{
	public:
		void proccesEvent(sjEvent * evt){}

		bool initialize(sjStateContext * ssc){
			m_context = ssc;
			return true;
		}

		bool evolve(sjStateContext * ssc){
			
			return true;
		}
		
		map<int, double> computeLaplacian(sjVIterator vi, vector< sjVertex_handle> neighbors){
		
			sjHalfedge_vertex_circulator vcir = vi->vertex_begin();
			sjPoint_3 point_i = vi->point();
			vector< sjVertex_handle> vecinos = neighbors;

			map<int, double> Lij;

			double Wij = 0.0;
			double L = 0.0;

			int j_pos, j_Alpha_pos, j_Beta_pos;
			for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
				
				j_Alpha_pos = ((j_pos + 1)+vecinos.size()) % vecinos.size();
				j_Beta_pos = ((j_pos - 1)+vecinos.size()) % vecinos.size();
				sjPoint_3 point_Alpha = ((vecinos[j_Alpha_pos]))->point();
				sjPoint_3 point_Beta = ((vecinos[j_Beta_pos]))->point();
				double Alpha = angle3(point_i, point_Alpha, ((vecinos[j_pos]))->point());
				if(Alpha < m_data->MIN_COT_ANGLE)				Alpha = m_data->MIN_COT_ANGLE;
				double Beta = angle3(point_i, point_Beta, ((vecinos[j_pos]))->point());
				if(Beta < m_data->MIN_COT_ANGLE)				Beta = m_data->MIN_COT_ANGLE;
				Wij = 1.0/ std::tan(Alpha) + 1.0/std::tan(Beta);
				Lij[vecinos[j_pos]->index] = Wij;
				L = L - Wij;
			}
			Lij[vi->index] = L;
		return Lij;	
	}
	
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	};
	class PluginComputeLaplacian: public sjPlugin{
	public:
		PluginComputeLaplacian(){
			name = "PluginComputeLaplacian";
			sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
			name_type = kernel.SYS_COMPUTE_LAPLACIAN_SYSTEM;
			registerPlugin(kernel);
		}

		void registerPlugin(sjKernelPlugin & K){

		}

		sjSystem * createSystem(){
			return new ComputeLaplacian();
		}
	};

	class ComputeMeanCurvature: public sjState{
	public:
		ComputeMeanCurvature(){}
		
		void proccesEvent(sjEvent * evt){}

		bool initialize(sjStateContext * ssc){
			m_context = ssc;
			return true;
		}

		bool evolve(sjStateContext * ssc){
			
			return true;
		}

		sjPoint_3 computeMeanCurvature(sjVIterator vi, vector< sjVertex_handle> neighbors){
			sjPoint_3 point_i = vi->point();
			vector< sjVertex_handle> vecinos = neighbors;
			//if(isDegenerateVertex(vi, neighbors))			return point_i;
			/*int j_pos;
			std::vector<double> pointnew = std::vector<double>(3,0.0);
			for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
				pointnew[0] = pointnew[0] + neighbors[j_pos]->point()[0];
				pointnew[1] = pointnew[1] + neighbors[j_pos]->point()[1];
				pointnew[2] = pointnew[2] + neighbors[j_pos]->point()[2];
			}
			pointnew[0] = pointnew[0] / (double)(vecinos.size());
			pointnew[1] = pointnew[1] / (double)(vecinos.size());
			pointnew[2] = pointnew[2] / (double)(vecinos.size());
			return sjPoint_3(pointnew[0], pointnew[1], pointnew[2]);*/


			double Wij = 0.0;
			double L = 0.0;

			int j_pos, j_Alpha_pos, j_Beta_pos;
			std::vector<double> pointnew = std::vector<double>(3,0.0);

			double WLi = std::pow(m_data->SL,((double) m_data->iteration)) * m_data->WL_0;
			WLi = 0.1;

			double A = 0.0;
			double Wii = 0.0;
			for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
				
				j_Alpha_pos = ((j_pos + 1)+vecinos.size()) % vecinos.size();
				j_Beta_pos = ((j_pos - 1)+vecinos.size()) % vecinos.size();
				sjPoint_3 point_Alpha = ((vecinos[j_Alpha_pos]))->point();
				sjPoint_3 point_Beta = ((vecinos[j_Beta_pos]))->point();
				double Alpha = angle3(point_i, point_Alpha, ((vecinos[j_pos]))->point());
				if(Alpha < m_data->MIN_COT_ANGLE)				Alpha = m_data->MIN_COT_ANGLE;
				double Beta = angle3(point_i, point_Beta, ((vecinos[j_pos]))->point());
				if(Beta < m_data->MIN_COT_ANGLE)				Beta = m_data->MIN_COT_ANGLE;

				Wij = 1.0/ std::tan(Alpha) + 1.0/std::tan(Beta);
				Wii = Wii + Wij;
				pointnew[0] = pointnew[0] + Wij*(point_i[0] - vecinos[j_pos]->point()[0] );
				pointnew[1] = pointnew[1] + Wij*(point_i[1] - vecinos[j_pos]->point()[1]);
				pointnew[2] = pointnew[2] + Wij*(point_i[2] - vecinos[j_pos]->point()[2]);
				A = A + area3(point_i, vecinos[j_pos]->point(), point_Alpha);
				//Lij = Lij + (vecinos[j_pos]->point() - point_i) * Wij;
			}
			double Lij = 0.1*(1.0/(4.0*A));
			pointnew[0] = point_i[0] - Lij * pointnew[0];
			pointnew[1] = point_i[1] - Lij * pointnew[1];
			pointnew[2] = point_i[2] - Lij * pointnew[2];
			(m_data->iteration)++;
		return sjPoint_3(pointnew[0], pointnew[1], pointnew[2]);

	}

	
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;

	};
	class PluginComputeMeanCurvature: public sjPlugin{
		public:
		PluginComputeMeanCurvature(){
			name = "PluginComputeMeanCurvature";
			sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
			name_type = kernel.SYS_COMPUTE_MEAN_CURVATURE_SYSTEM ;
			registerPlugin(kernel);
		}

		void registerPlugin(sjKernelPlugin & K){

		}

		sjSystem * createSystem(){
			return new ComputeMeanCurvature();
		}
	};

	class MeanCurvatureSmoothing: public sjState{
		MeanCurvatureSmoothing(){}
		
		void proccesEvent(sjEvent * evt){}

		bool initialize(sjStateContext * ssc){
			m_context = ssc;
			return true;
		}

		bool evolve(sjStateContext * ssc){
			size_t n = this->m_context->getNumberOfVertex();// getNsize();
			vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
			sjVIterator v;
			
			int i = 0;

			ComputeMeanCurvature * cmc =(ComputeMeanCurvature * )
									    ( sjKernelPlugin.getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_MEAN_CURVATURE_SYSTEM));
			sjPolyhedron mesh_G = this->m_context->getMesh();
			vector< vector<sjVertex_handle> > rings = this->m_context->getRings();
			for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
				//sjPoint_3 newvi = computeMeanCurvature(v, rings[i]);
				sjPoint_3 newvi = cmc->computeMeanCurvature(v, rings[i]);
				matrixV[i][0] = newvi.x();
				matrixV[i][1] = newvi.y();
				matrixV[i][2] = newvi.z();
				i++;
			}
			i = 0;
			for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
				sjPoint_3 pointn = sjPoint_3(matrixV[i][0], matrixV[i][1], matrixV[i][2]);
				v->point() = pointn;
				i++;
			}
			return true;
		}
	
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	};
	class PluginMeanCurvatureSmoothing: public sjPlugin{
	public:
		PluginMeanCurvatureSmoothing(){
			name = "PluginMeanCurvatureSmoothing";
			sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
			name_type = kernel.SYS_MEAN_CURVATURE_SMOOTHING_SYSTEM;
			registerPlugin(kernel);
		}

		void registerPlugin(sjKernelPlugin & K){

		}

		sjSystem * createSystem(){
			return new MeanCurvatureSmoothing();
		}

	};

	class IsDegenerateVertex: public sjState{
	public:
		IsDegenerateVertex(){}
		
		void proccesEvent(sjEvent * evt){}

		bool initialize(sjStateContext * ssc){
			m_context = ssc;
			return true;
		}

		bool evolve(sjStateContext * ssc){
			return true;
		}

		bool isDegenerateVertex(sjVIterator vi, vector< sjVertex_handle> neighbors){
			int i, j, m, pos_1, pos_2;
			double area;
			m = neighbors.size();
			for(j=0; j<m; j++){
				pos_1 = j;
				pos_2 = (j+1) % m;
				sjPoint_3 point_i = vi->point();
				area = area3(point_i,((neighbors[pos_1]))->point(),((neighbors))[pos_2]->point());
				if(m_data->AVERAGE_FACE/m_data->MAX_CONTRACTION_SQUARED > area){
					return true;
				}
			}

			ComputeLaplacian * coml = (ComputeLaplacian * )(sjKernelPlugin.getInstance().getSystem(sjKernelPlugin.SYS_COMPUTE_LAPLACIAN_SYSTEM));
			map<int, double> mymap = coml->computeLaplacian(vi, neighbors);
			map<int, double>::iterator it;
			for ( it=mymap.begin() ; it != mymap.end(); it++ ){
				int index = (*it).first ;
				double value = (*it).second;
				if(value > m_data->MAX_ATTRACTION){
					return true;
				}
			}
			return false;
	}
		
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	};
	class PluginIsDegenerateVertex: public sjPlugin{
	public:
		PluginIsDegenerateVertex(){
			name = "PluginIsDegenerateVertex";
			sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
			name_type = kernel.SYS_IS_DEGENERATE_VERTEX_SYSTEM;
			registerPlugin(kernel);
		}

		void registerPlugin(sjKernelPlugin & K){

		}

		sjSystem * createSystem(){
			return new IsDegenerateVertex();
		}

	};

	class InitLaplacianSmoothing: public sjState{
	private:
		BasicFilter * m_data;
		sjStateContext * m_context;
	public:
		InitLaplacianSmoothing(){}
		
		void proccesEvent(sjEvent * evt){}

		bool initialize(sjStateContext * ssc){
			m_context = ssc;
			return true;
		}

		bool evolve(sjStateContext * ssc){
			return true;
		}

		void initLaplacianSmoothing(double a_WH0 = 1.0, double a_WL0 = 0.01, double a_SL = 2.0){
		
			this->m_context->getMesh
			mesh_G = * input_pipe->read();

			initIndex();
			computeRings();
			AVERAGE_FACE = averageFaces();
			//WH_0 = a_WH0;
			WH_0 = 10;
			//WL_0 = a_WL0 * std::sqrt(AVERAGE_FACE);
			WL_0 = 1.0;
			SL = a_SL;
			int i = 0;
			for ( sjVIterator v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
				computeLineEquations(v, rings[i]);
				i = i + 1;
			}
		}
	
	};
}

