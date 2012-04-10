#ifndef __SJ_SIMPLI_H__
#define __SJ_SIMPLI_H__

#include "sjDataType.h"
#include "sjKernelPlugin.h"
#include "sjPipeFilter.h"
#include "sjSystem.h"
#include "sjUtils.h"
#include "sjState.h"
#include "sjLog.h"
#include <list>

using namespace std;



namespace sj{

	class sjNodeHeap {
	public:
		sjNodeHeap(sjHalfedge_handle he, double a_value){
			hedge = he;
			value = a_value;
		}
		/*~sjNodeHeap(){
			cout<<"Destruir nodo; "<<hedge->hedgeid<<endl;
			sjHalfedge_handle nulo;
			hedge = nulo;
		}*/
		sjHalfedge_handle hedge;
		double value;
		bool operator < (const sjNodeHeap& rhs){
			return value < rhs.value;
		}
	};

	class sjSimplificator: public sjPolyhedronPipe::sjFilter, public sjObserver{
	public:
		sjSimplificator(double wa = 1.0, double wb = 0.1);
		Matrix4d getFundamentalErrorQuadric(sjHalfedge_handle);
		double calculateSamplingCost(sjHalfedge_handle);
		double calculateShapeCost(Matrix4d Qi, Matrix4d Qj,  sjHalfedge_handle he);
		double calculateTotalCost(sjHalfedge_handle he);
		Matrix4d computeInitialQ(sjVertex_handle);
		void computeAllInitialQ();
		bool isCollapseTunnel(sjHalfedge_handle);
		void computeHeapError();
		void init();
		void collapseEdge(sjHalfedge_handle he);
		//sjHalfedge_handle getHalfedgeFromID(int id);
		list<sjNodeHeap>::iterator getValidEdgeToCollapse();

		void proccesEvent(sjEvent * evt);
		void setParameters(sjPolyhedronPipe::ParametersType *){}
		void setMesh(sjPolyhedronPipe::PolyhedronType amesh ){
			sjLogInformation("setMesh");
			mesh_G = amesh;
		}
		size_t getNumberOfVertex(){
			return mesh_G.size_of_vertices();
		}
		sjPolyhedronPipe::PolyhedronType iterate();
		sjPolyhedronPipe::PolyhedronType getMesh(){
			sjLogInformation("getMesh");
			return mesh_G;
		}
		sjPolyhedronPipe::ParametersType * getParameters(){
			return NULL;
		}

		int getNumberNodes(){
			return number_nodes;
		}

		void setNumberNodes(int num_nodes){
			number_nodes = num_nodes;
		}

		
	private:
		double Wa, Wb;
		map<int, Matrix4d> Qmap;
		sjPolyhedron mesh_G;
		vector< vector< sjVertex_handle > > rings;
		list<sjNodeHeap> heap_error;
		bool m_init;
		int number_nodes;
	};

}

#endif //__SJ_SIMPLI_H__