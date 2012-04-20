#ifndef __SJ_SIMPLI_H__
#define __SJ_SIMPLI_H__

#include "sjDataType.h"
#include "sjKernelPlugin.h"
#include "sjPipeFilter.h"
#include "sjSystem.h"
#include "sjUtils.h"
#include "sjState.h"
#include "sjLog.h"
#include "sjGraphSkeleton.h"
#include <list>

using namespace std;



namespace sj{

	class sjGraphPoint{
	public:
		sjGraphPoint(double ax = 0, double ay = 0, double az = 0, int aid=-1): x(ax), y(ay), z(az), id(aid){}
		sjPoint_3 getPoint_3(){
			return sjPoint_3(x,y,z);
		}
		double x, y, z;
		int id;
	};

	class sjGetIdFromPoint{
		public:
		int getID(sjGraphPoint p){
			return p.id;
		}
		void print(sjGraphPoint p){
			cout<<p.getPoint_3();
		}
	};

	typedef sjGraphSkeleton<sjGraphPoint, sjGetIdFromPoint> sjGraphSkeletonType;


	

	class sjNodeHeap {
	public:
		sjNodeHeap(int he, double a_value){
			hedge = he;
			value = a_value;
			//sjSkelVertexType::PointerHalfedgeType he344 = new sjSkelVertexType::HalfedgeType();
		}
		int hedge;
		double value;
		bool operator < (const sjNodeHeap& rhs){
			return value < rhs.value;
		}
	};

	class sjSimplificator: public sjPolyhedronPipe::sjFilter, public sjObserver{
	public:
		sjSimplificator(double wa = 1.0, double wb = 0.1);
		void convertPolyhedronToSkeleton();
		Matrix4d getFundamentalErrorQuadric(int);
		double calculateSamplingCost(int);
		double calculateShapeCost(Matrix4d Qi, Matrix4d Qj,  int he);
		double calculateTotalCost(int he);
		Matrix4d computeInitialQ(int);
		void computeAllInitialQ();
		//bool isCollapseTunnel(int);
		void computeHeapError();
		void init();
		void collapseEdge(int he);
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

		sjGraphSkeletonType getSkeleton(){
			return sjskeleton;
		}

		
	private:
		double Wa, Wb;
		map<int, Matrix4d> Qmap;
		sjPolyhedron mesh_G;
		vector< vector< sjVertex_handle > > rings;
		list<sjNodeHeap> heap_error;
		bool m_init;
		int number_nodes;
		sjGraphSkeletonType sjskeleton;
	};

}

#endif //__SJ_SIMPLI_H__