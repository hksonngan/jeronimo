#include "sjMeanCurvatureSmoothing.h"
#include "sjCoreSmoothing.h"
#include "sjMeshContractionAu2008.h"

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
#include "sjDataIO.h"
#include "sjLog.h"

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
using namespace sj;

map<int, double> ComputeMeanCurvature::computeWeight(sjStateContext * m_ctx, sjVIterator vi, vector< sjVertex_handle> neighbors){
		
	sjHalfedge_vertex_circulator vcir = vi->vertex_begin();
	sjPoint_3 point_i = vi->point();
	vector< sjVertex_handle> vecinos = neighbors;
	map<int, double> Lij;

	double Wij = 0.0;
	double L = 0.0;

	int j_pos, j_Alpha_pos, j_Beta_pos;
	int numbers_neighbor = (int)(vecinos.size());
	for(j_pos = 0; j_pos < numbers_neighbor; j_pos++){
		squared_distance(point_i, vecinos[j_pos]->point());
		(vi->point(), vecinos[j_pos]);
		//Wij = 1.0/ (double)(numbers_neighbor);
		Wij = 1.0/sqrt( squared_distance(point_i, vecinos[j_pos]->point()));
		Lij[vecinos[j_pos]->index] = Wij;
		L = L - Wij;
	}
	Lij[vi->index] = L;
	return Lij;	
}

PluginComputeMeanCurvature::PluginComputeMeanCurvature(){
	name = "PluginComputeMeanCurvature";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_COMPUTE_WEIGHT_SYSTEM ;
	registerPlugin(kernel);
}

void PluginComputeMeanCurvature::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginComputeMeanCurvature::createSystem(){
	return new ComputeMeanCurvature();
}

bool MeanCurvatureSmoothing::evolve(sjStateContext * ssc){
	int n = this->m_context->getNumberOfVertex();
	LinearSolver *solver;
	SystemSolverParameters params ;
	std::string  * mipo = new std::string("SUPERLU");
	params.set_arg_value("method", *mipo) ; 
		
	solver = new LinearSolver(n) ;
	solver->set_system_solver(params) ;
	solver->set_least_squares(true) ;
	solver->set_invert_matrix(true) ;

	cout<<"\nIterate system: No = "<<this->m_context->iteration+1<<"|\t";
	cout<<"Volume Ini= "<<calcVolume(STATE_MESH)<<"|\t";
	CGAL::Timer timer;
	timer.start();
	int i;
	sjVIterator v;
	int coord;
	vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
	double WLi = std::pow(this->m_context->SL,((double)(this->m_context->iteration))) * (this->m_context->WL_0);
	//double WLi = 1.0;
	bool found_solution = true;

		
	IsDegenerateVertex * idv= (IsDegenerateVertex *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_IS_DEGENERATE_VERTEX_SYSTEM));
	ComputeWeight * comlap= (ComputeWeight *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_WEIGHT_SYSTEM));
	for(coord = 0; coord<3; coord++){

		i = 0;
		for ( v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
			if(idv->isDegenerateVertex(this->m_context, v,  STATE_RINGS[i])){
				solver->variable(i).lock();
			}
			sjPoint_3 point_k = v->point();
			double r = point_k[coord];
			solver->variable(i).set_value(r);		
				
				
			i++;
		}
			
		solver->begin_system();
		i = 0;
		for ( v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
			if(!idv->isDegenerateVertex(this->m_context, v, STATE_RINGS[i])){
				sjHalfedge_vertex_circulator vcir = v->vertex_begin();
				map<int, double> mymap = comlap->computeWeight(this->m_context, v, STATE_RINGS[i]);
				map<int, double>::iterator it;
				solver->begin_row();
				for ( it=mymap.begin() ; it != mymap.end(); it++ ){
					int index = (*it).first ;
					double value = (*it).second;
					solver->add_coefficient(index, WLi * value);
				}
				solver->set_right_hand_side(0.0);
				solver->end_row();
			}
			i++;
		}
		i = 0;
		for ( v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
			if(!idv->isDegenerateVertex(this->m_context, v, STATE_RINGS[i])){
			sjHalfedge_vertex_circulator vcir = v->vertex_begin();
			sjPoint_3 point_i = v->point();
			double Whi;
			if(this->m_context->iteration ==0){
				Whi = this->m_context->WH_0;
			}else{
				Whi = this->m_context->WH_0*std::sqrt(v->initial_ring_area/areaRing(v, STATE_RINGS[i]));
			}
			solver->begin_row();
			solver->add_coefficient(i, Whi);
			solver->set_right_hand_side(Whi*point_i[coord]);
			solver->end_row();
			}
			i++;
		}
		solver->end_system();
			
		if ( solver->solve() ){
				
			for(i=0; i<n; i++){
				matrixV[i][coord] = solver->variable(i).value();
			}
		}else{
			cout<<"No solution found"<<endl;
			found_solution = false;
		}

		solver->restart();

	}
	if(found_solution){
		i = 0;
		for ( v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
			sjPoint_3 pointn = sjPoint_3(matrixV[i][0], matrixV[i][1], matrixV[i][2]);
			v->point() = pointn;
			i++;
		}
	}
	cout<<"Volume = "<<calcVolume(STATE_MESH)<<"|\t";
	cout<<"Time = "<< timer.time() << " seconds." << std::endl;
	delete solver;
	this->m_context->iteration++;
	return true;
}

PluginMeanCurvatureSmoothing::PluginMeanCurvatureSmoothing(){
	name = "PluginMeanCurvatureSmoothing";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_ITERATE_SMOOTHING_ALGORITHM_SYSTEM;
	registerPlugin(kernel);
}

void PluginMeanCurvatureSmoothing::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginMeanCurvatureSmoothing::createSystem(){
	return new MeanCurvatureSmoothing();
}
