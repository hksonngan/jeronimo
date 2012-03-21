#include "sjLaplacianSmoothing.h"
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

sjVIterator ComputeLineEquations::computeLineEquations( sjVIterator vi, vector< sjVertex_handle> neighbors){
	//sjLogDebug("ComputeLineEquations::computeLineEquations 1\n");
	int i, n;
	n = neighbors.size();
	double x1, x2, x3;
	double y1, y2, y3;
	double z1, z2, z3;
	sjPoint_3 p1, p2, p3;
	sjVector_3 nt1, nt2;
	p1 = vi->point();
	p2 = neighbors[0]->point();
	nt1 = normalVectorFromNeighbors(vi, neighbors);
	//cout<<"Vertex["<<vi->index<<"] = "<<vi->point()<<"|\tN = "<<nt1<<endl;
	//cout<<"\tP2 = "<<p2<<endl;
	p3 = p1 - nt1;
	//cout<<"\tP3 = "<<p3<<endl;

	x1 = p1[0];		y1 = p1[1];		z1 = p1[2];
	x2 = p2[0];		y2 = p2[1];		z2 = p2[2];
	x3 = p3[0];		y3 = p3[1];		z3 = p3[2];
	vi->a1 = y1 *(z2 - z3) + y2 *(z3 - z1) + y3 *(z1 - z2) ;
	vi->b1 = z1 *(x2 - x3) + z2 *(x3 - x1) + z3 *(x1 - x2) ;
	vi->c1 = x1 *(y2 - y3) + x2 *(y3 - y1) + x3 *(y1 - y2) ;
	vi->d1 = -1.0 * (x1* (y2* z3 - y3* z2) + x2* (y3* z1 - y1* z3) + x3* (y1* z2 - y2* z1));
	double sabc = std::sqrt(vi->a1*vi->a1 + vi->b1*vi->b1 + vi->c1*vi->c1);
	vi->a1 = vi->a1 / sabc;
	vi->b1 = vi->b1 / sabc;
	vi->c1 = vi->c1 / sabc;
	vi->d1 = vi->d1 / sabc;
	//cout<<"\t"<<vi->a1<<"x + "<<vi->b1<<"y + "<<vi->c1<<"z + "<<vi->d1<<endl;
	//cout<<"angle abc con n = "<<angle3(		sjPoint_3(0,0,0) + nt1, 		sjPoint_3(0,0,0), 		sjPoint_3(vi->a1, vi->b1, vi->c1)		)<<endl;
	p1 = vi->point();
	p2 = neighbors[1]->point();
	nt2 = normalVectorFromNeighbors(vi, neighbors);
	//cout<<"\tP2 = "<<p2<<endl;
	p3 = p1 - nt2;
	//cout<<"\tP3 = "<<p3<<endl;
	x1 = p1[0];		y1 = p1[1];		z1 = p1[2];
	x2 = p2[0];		y2 = p2[1];		z2 = p2[2];
	x3 = p3[0];		y3 = p3[1];		z3 = p3[2];
	vi->a2 = y1 *(z2 - z3) + y2 *(z3 - z1) + y3 *(z1 - z2) ;
	vi->b2 = z1 *(x2 - x3) + z2 *(x3 - x1) + z3 *(x1 - x2) ;
	vi->c2 = x1 *(y2 - y3) + x2 *(y3 - y1) + x3 *(y1 - y2) ;
	vi->d2 = -1.0 * (x1* (y2* z3 - y3* z2) + x2* (y3* z1 - y1* z3) + x3* (y1* z2 - y2* z1));
	sabc = std::sqrt(vi->a2*vi->a2 + vi->b2*vi->b2 + vi->c2*vi->c2);
	vi->a2 = vi->a1 / sabc;
	vi->b2 = vi->b1 / sabc;
	vi->c2= vi->c1 / sabc;
	vi->d2 = vi->d1 / sabc;
	//cout<<"\t"<<vi->a2<<"x + "<<vi->b2<<"y + "<<vi->c2<<"z + "<<vi->d2<<endl;
	return vi;
		
}

PluginComputeLineEquations::PluginComputeLineEquations(){
	name = "PluginComputeLineEquations";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_COMPUTE_LINE_EQUATIONS_SYSTEM;
	registerPlugin(kernel);
}

void PluginComputeLineEquations::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginComputeLineEquations::createSystem(){
	return new ComputeLineEquations();
}

bool IterateLaplacianSmoothingIntegrate::evolve(sjStateContext * ssc){
	sjLogDebug("IterateLaplacianSmoothingIntegrate::evolve");
	m_context = ssc;
	int n = this->m_context->getNumberOfVertex();
	LinearSolver *solver;
	SystemSolverParameters params ;
	std::string  * mipo = new std::string("SUPERLU");
	params.set_arg_value("method", *mipo) ; 
	solver = new LinearSolver(n*3) ;
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
	double WLi = std::pow(this->m_context->SL,((double)(this->m_context->iteration))) * this->m_context->WL_0;
	//double WLi = 1.0;
	bool found_solution = true;

	IsDegenerateVertex * idv = (IsDegenerateVertex *) (sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_IS_DEGENERATE_VERTEX_SYSTEM));
	ComputeWeight * comlap =  (ComputeWeight *) (sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_WEIGHT_SYSTEM));
		i = 0;
		
		for ( v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
			if(idv->isDegenerateVertex(this->m_context, v, STATE_RINGS[i])){
				solver->variable(i).lock();
				solver->variable(i+n).lock();
				solver->variable(i+2*n).lock();
			}
			sjPoint_3 point_k = v->point();
			double r = point_k[coord];
			solver->variable(i).set_value(point_k[0]);
			solver->variable(i+n).set_value(point_k[1]);
			solver->variable(i+2*n).set_value(point_k[2]);
			i++;
		}
		
		/*for(sjHEIterator hi = mesh_G.halfedges_begin (); hi!= mesh_G.halfedges_end (); ++hi){
			if(angleOrientedPlanes(hi)<sjpi/4){
				sjVertex_handle vertex_A = hi->vertex();
				sjVertex_handle vertex_B = hi->next()->vertex();
				sjVertex_handle vertex_C = hi->prev()->vertex();
				sjVertex_handle vertex_D = hi->opposite()->next()->vertex();
				solver->variable(vertex_A->index).lock();
				solver->variable(vertex_B->index).lock();
				solver->variable(vertex_C->index).lock();
				solver->variable(vertex_D->index).lock();
			}
		}*/

		solver->begin_system();
		for(coord = 0; coord<3; coord++){
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
						solver->add_coefficient(index +coord*n , WLi * value);
					}
					solver->set_right_hand_side(0.0);
					solver->end_row();
				}
				i++;
			}
		}
		int jcoord;
		for(jcoord = 0; jcoord<3; jcoord++){
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
				solver->add_coefficient(i+jcoord*n, Whi);
				solver->set_right_hand_side(Whi*point_i[jcoord]);
				solver->end_row();
				}
				i++;
			}
		}
		/*
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			if(!isDegenerateVertex(v, rings[i])){
				solver->begin_row();
				solver->add_coefficient(i    ,v->a1);
				solver->add_coefficient(i+n  ,v->b1);
				solver->add_coefficient(i+2*n,v->c1);
				solver->set_right_hand_side(-v->d1);

				solver->end_row();

				solver->begin_row();
				solver->add_coefficient(i    ,v->a2);
				solver->add_coefficient(i+n  ,v->b2);
				solver->add_coefficient(i+2*n,v->c2);
				solver->set_right_hand_side(-v->d2);

				solver->end_row();
			}
			i++;
		}*/
			
		solver->end_system();
			
		if ( solver->solve() ){
				
			for(i=0; i<n; i++){
				matrixV[i][0] = solver->variable(i).value();
				matrixV[i][1] = solver->variable(i+n).value();
				matrixV[i][2] = solver->variable(i+2*n).value();
			}
		}else{
			cout<<"No solution found"<<endl;
			found_solution = false;
		}
		solver->restart();
	//}
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

PluginIterateLaplacianSmoothingIntegrate::PluginIterateLaplacianSmoothingIntegrate(){
	name = "PluginIterateLaplacianSmoothingIntegrate";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_ITERATE_SMOOTHING_ALGORITHM_SYSTEM;
	registerPlugin(kernel);
}

void PluginIterateLaplacianSmoothingIntegrate::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginIterateLaplacianSmoothingIntegrate::createSystem(){
	return new IterateLaplacianSmoothingIntegrate();
}


