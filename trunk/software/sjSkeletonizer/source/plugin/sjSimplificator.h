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

	class StopConstraint{
	public:
		StopConstraint(){
			n_window = 100.0;
			X.push_back(0.0);
			Y.push_back(0.0);
			XY.push_back(0.0);
			SUMX.push_back(0.0);
			SUMY.push_back(0.0);
			SUMXX.push_back(0.0);
			SUMXY.push_back(0.0);
			SUMX_WINDOW.push_back(0.0);
			SUMY_WINDOW.push_back(0.0);
			SUMXX_WINDOW.push_back(0.0);
			SUMXY_WINDOW.push_back(0.0);
			M.push_back(0.0);
			MWINDOW.push_back(0.0);
		}
		vector<double> X, Y, XY;
		vector<double> SUMX, SUMY, SUMXX, SUMXY;
		vector<double> SUMX_WINDOW, SUMY_WINDOW, SUMXX_WINDOW, SUMXY_WINDOW;
		vector<double> M, MWINDOW;
		double n_window;
		double threshold;
		void insert(double ay){
			double x,y,xx,xy,m;
			x = (double)(X.size());
			y = ay;
			xx = x*x;
			xy = x*y;

			X.push_back(x);
			Y.push_back(y);
			XY.push_back(xy);
			
			SUMX.push_back(SUMX.back() + X.back());
			SUMY.push_back(SUMY.back() + Y.back());
			SUMXY.push_back(SUMXY.back() + XY.back());
			SUMXX.push_back(SUMXX.back() + X.back() * X.back());

			double nsize, sx, sy, sxx, sxy, adiv;
			nsize = (double)(X.size());
			sx  = SUMX.back();
			sy  = SUMY.back();
			sxx = SUMXX.back();
			sxy = SUMXY.back();

			adiv = nsize*sxx - sx*sx;
			if(std::abs(adiv) < 0.000000000000001){
				m = 0.0;
			}else{
				m = (nsize * sxy - sx*sy)/adiv;
			}
			M.push_back(m);

			vector<double>::size_type size_window = (vector<double>::size_type)n_window;

			if(nsize > size_window){
				SUMX_WINDOW.push_back(SUMX.back() - SUMX[SUMX.size()-1-size_window]);
				SUMY_WINDOW.push_back(SUMY.back() - SUMY[SUMY.size()-1-size_window]);
				SUMXX_WINDOW.push_back(SUMXX.back() - SUMXX[SUMX.size()-1-size_window]);
				SUMXY_WINDOW.push_back(SUMXY.back() - SUMXY[SUMXY.size()-1-size_window]);

				nsize = n_window;
				sx  = SUMX_WINDOW.back();
				sy  = SUMY_WINDOW.back();
				sxx = SUMXX_WINDOW.back();
				sxy = SUMXY_WINDOW.back();

				adiv = nsize*sxx - sx*sx;
				if(std::abs(adiv) < 0.000000000000001){
					m = 0.0;
				}else{
					m = (nsize * sxy - sx*sy)/adiv;
				}
				MWINDOW.push_back(m);

			}else{
				SUMX_WINDOW.push_back(SUMX.back());
				SUMY_WINDOW.push_back(SUMY.back());
				SUMXX_WINDOW.push_back(SUMXX.back());
				SUMXY_WINDOW.push_back(SUMXY.back());
				MWINDOW.push_back(M.back());
			}


		}

		double getGlobalSlope(){
			return M[M.size()-1];
		}

		double getWindowSlope(){
			return MWINDOW[MWINDOW.size()-1];
		}

	};


	class sjGraphPoint{
	public:
		sjGraphPoint(double ax = 0, double ay = 0, double az = 0, int aid=-1): x(ax), y(ay), z(az), id(aid){}
		sjPoint_3 getPoint_3(){
			return sjPoint_3(x,y,z);
		}
		double x, y, z;
		int id;
		double weight;
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
		sjSimplificator(double wa = 1.0, double wb = 1.0, double wn = 1000.0);
		void convertPolyhedronToSkeleton();
		Matrix4d getFundamentalErrorQuadric(int);
		double calculateSamplingCost(int);
		double calculateNeighboringCost(int);
		double calculateAreaCost(int);
		double calculateShapeCost(Matrix4d Qi, Matrix4d Qj,  int he);
		double calculateTotalCost(int he);
		Matrix4d computeInitialQ(int);
		void computeAllInitialQ();
		//bool isCollapseTunnel(int);
		void computeHeapError();
		void init();
		list<sjNodeHeap>::iterator findNodeInHeapError(int he);
		void collapseEdge(int he);
		void updateError(int he);
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
		double Wa, Wb, Wn;
		map<int, Matrix4d> Qmap;
		sjPolyhedron mesh_G;
		vector< vector< sjVertex_handle > > rings;
		list<sjNodeHeap> heap_error;
		bool m_init;
		int number_nodes;
		sjGraphSkeletonType sjskeleton;
		int iteration;
		StopConstraint m_stop_constraint;
	};

}

#endif //__SJ_SIMPLI_H__