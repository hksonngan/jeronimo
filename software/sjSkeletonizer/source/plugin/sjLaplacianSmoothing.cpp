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
#include "sjDataIO.h"

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

BasicFilter::BasicFilter():
WH_0(1.0), 
WL_0(1.0), 
SL(0), 
iteration(0), 
AVERAGE_FACE(0.0),
MAX_CONTRACTION_SQUARED( 100000.),
MAX_ATTRACTION(100000000.),
MIN_COT_ANGLE(0.00000000000000001)		{

}

void BasicFilter::proccesEvent(sjEvent * evt){
}

PluginBasicFilter::PluginBasicFilter() {
	name = "PluginBasicFilter";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_BASIC_FILTER_SYSTEM;
	registerPlugin(kernel);
}

void PluginBasicFilter::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginBasicFilter::createSystem(){
	return new BasicFilter();
}

InitIndex::InitIndex():
	m_data(0){
		m_data = (BasicFilter *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_BASIC_FILTER_SYSTEM));
}

void InitIndex::proccesEvent(sjEvent * evt){}

bool InitIndex::initialize(sjStateContext * ssc){
	m_context = ssc;
	return true;
}

bool InitIndex::evolve(sjStateContext * ssc){
	int i = 0;
	sjPolyhedronPipe::PolyhedronType mesh_G;
	mesh_G = m_context->getMesh();
	for ( sjVIterator v1 = mesh_G.vertices_begin(); v1 != mesh_G.vertices_end(); ++v1){
		sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
		vcir->vertex()->index = i;
		vcir->vertex()->initial_ring_area = 0.0;
		i++;
	}
	m_context = ssc;
	m_context->setState((sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_RINGS_SYSTEM)));
	return true;
}

PluginInitIndex::PluginInitIndex() {
	name = "PluginInitIndex";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_INIT_INDEX_SYSTEM;
	registerPlugin(kernel);
}

void PluginInitIndex::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginInitIndex::createSystem(){
	return new InitIndex();
}

ComputeRings::ComputeRings(){
}

void ComputeRings::proccesEvent(sjEvent * evt){}

bool ComputeRings::initialize(sjStateContext * ssc){
	m_context = ssc;
	return true;
}

bool ComputeRings::evolve(sjStateContext * ssc){
	m_context = ssc;
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

	m_context->setState((sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_INIT_LAPLACIAN_SMOOTHING_SYSTEM)));
	return true;
}

PluginComputeRings::PluginComputeRings(){
	name = "PluginComputeRings";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_COMPUTE_RINGS_SYSTEM;
	registerPlugin(kernel);
}

void PluginComputeRings::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginComputeRings::createSystem(){
	return new ComputeRings();
}

ComputeLaplacian::ComputeLaplacian(){
	m_data = (BasicFilter *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_BASIC_FILTER_SYSTEM));
}
void ComputeLaplacian::proccesEvent(sjEvent * evt){}
		
map<int, double> ComputeLaplacian::computeLaplacian(sjVIterator vi, vector< sjVertex_handle> neighbors){
		
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

PluginComputeLaplacian::PluginComputeLaplacian(){
	name = "PluginComputeLaplacian";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_COMPUTE_LAPLACIAN_SYSTEM;
	registerPlugin(kernel);
}

void PluginComputeLaplacian::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginComputeLaplacian::createSystem(){
	return new ComputeLaplacian();
}

ComputeMeanCurvature::ComputeMeanCurvature(){}
		
void ComputeMeanCurvature::proccesEvent(sjEvent * evt){}

bool ComputeMeanCurvature::initialize(sjStateContext * ssc){
	m_context = ssc;
	return true;
}

bool ComputeMeanCurvature::evolve(sjStateContext * ssc){
			
	return true;
}

sjPoint_3 ComputeMeanCurvature::computeMeanCurvature(sjVIterator vi, vector< sjVertex_handle> neighbors){
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

PluginComputeMeanCurvature::PluginComputeMeanCurvature(){
	name = "PluginComputeMeanCurvature";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_COMPUTE_MEAN_CURVATURE_SYSTEM ;
	registerPlugin(kernel);
}

void PluginComputeMeanCurvature::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginComputeMeanCurvature::createSystem(){
	return new ComputeMeanCurvature();
}

MeanCurvatureSmoothing::MeanCurvatureSmoothing(){}
		
void MeanCurvatureSmoothing::proccesEvent(sjEvent * evt){}

bool MeanCurvatureSmoothing::initialize(sjStateContext * ssc){
	m_context = ssc;
	return true;
}

bool MeanCurvatureSmoothing::evolve(sjStateContext * ssc){
	size_t n = this->m_context->getNumberOfVertex();// getNsize();
	vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
	sjVIterator v;
			
	int i = 0;

	ComputeMeanCurvature * cmc = (ComputeMeanCurvature * )
								( sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_MEAN_CURVATURE_SYSTEM));
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

PluginMeanCurvatureSmoothing::PluginMeanCurvatureSmoothing(){
	name = "PluginMeanCurvatureSmoothing";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_MEAN_CURVATURE_SMOOTHING_SYSTEM;
	registerPlugin(kernel);
}

void PluginMeanCurvatureSmoothing::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginMeanCurvatureSmoothing::createSystem(){
	return new MeanCurvatureSmoothing();
}

IsDegenerateVertex::IsDegenerateVertex(){
	m_data = (BasicFilter *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_BASIC_FILTER_SYSTEM));
	m_comlapl = (ComputeLaplacian * )(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_LAPLACIAN_SYSTEM));
}
		
void IsDegenerateVertex::proccesEvent(sjEvent * evt){}

bool IsDegenerateVertex::isDegenerateVertex(sjVIterator vi, vector< sjVertex_handle> neighbors){
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

	map<int, double> mymap = m_comlapl->computeLaplacian(vi, neighbors);
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
		
PluginIsDegenerateVertex::PluginIsDegenerateVertex(){
	name = "PluginIsDegenerateVertex";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_IS_DEGENERATE_VERTEX_SYSTEM;
	registerPlugin(kernel);
}

void PluginIsDegenerateVertex::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginIsDegenerateVertex::createSystem(){
	return new IsDegenerateVertex();
}

ComputeLineEquations::ComputeLineEquations(){}
		
void ComputeLineEquations::proccesEvent(sjEvent * evt){}

void ComputeLineEquations::computeLineEquations(sjVIterator vi, vector< sjVertex_handle> neighbors){
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

InitLaplacianSmoothing::InitLaplacianSmoothing(){
	m_data = (BasicFilter *) (sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_BASIC_FILTER_SYSTEM));
}
		
void InitLaplacianSmoothing::proccesEvent(sjEvent * evt){}

bool InitLaplacianSmoothing::initialize(sjStateContext * ssc){
	m_context = ssc;
	return true;
}

bool InitLaplacianSmoothing::evolve(sjStateContext * ssc){
	initLaplacianSmoothing();
	m_context = ssc;
	m_context->setState((sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_ITERATE_SMOOTHING_ALGORITHM_SYSTEM)));
	return true;
}

void InitLaplacianSmoothing::initLaplacianSmoothing(double a_WH0, double a_WL0, double a_SL){
		
	sjPolyhedronPipe::PolyhedronType mesh_G = this->m_context->getMesh();
	this->m_data->AVERAGE_FACE = averageFaces(mesh_G);
	//WH_0 = a_WH0;
	this->m_data->WH_0 = 10;
	//WL_0 = a_WL0 * std::sqrt(AVERAGE_FACE);
	this->m_data->WL_0 = 1.0;
	this->m_data->SL = a_SL;
	int i = 0;
	ComputeLineEquations * cle = (ComputeLineEquations *) (sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_LINE_EQUATIONS_SYSTEM));
	vector< vector<sjVertex_handle> > rings = this->m_context->getRings() ;
	for ( sjVIterator v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
		cle->computeLineEquations(v, rings[i]);
		i = i + 1;
	}
}
	
PluginInitLaplacianSmoothing::PluginInitLaplacianSmoothing(){
	name = "PluginInitLaplacianSmoothing";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_INIT_LAPLACIAN_SMOOTHING_SYSTEM;
	registerPlugin(kernel);
}

void PluginInitLaplacianSmoothing::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginInitLaplacianSmoothing::createSystem(){
	return new InitLaplacianSmoothing();
}

bool IterateLaplacianSmoothingSeparate::evolve(sjStateContext * ssc){
	int n = this->m_context->getNumberOfVertex();// getNsize();
	LinearSolver *solver;
	//delete solver;
	SystemSolverParameters params ;
	std::string  * mipo = new std::string("SUPERLU");
	params.set_arg_value("method", *mipo) ; 
		
	solver = new LinearSolver(n) ;
	solver->set_system_solver(params) ;
	solver->set_least_squares(true) ;
	solver->set_invert_matrix(true) ;

	cout<<"\nIterate system: No = "<<this->m_data->iteration+1<<"|\t";
		
	sjPolyhedron mesh_G = this->m_context->getMesh();

	cout<<"Volume Ini= "<<calcVolume(mesh_G)<<"|\t";
	CGAL::Timer timer;
	timer.start();
	int i;
	sjVIterator v;
	int coord;
	vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
	double WLi = std::pow(this->m_data->SL,((double)(this->m_data->iteration))) * (this->m_data->WL_0);
	//double WLi = 1.0;
	bool found_solution = true;

		
	IsDegenerateVertex * idv= (IsDegenerateVertex *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_IS_DEGENERATE_VERTEX_SYSTEM));
	vector< vector<sjVertex_handle> > rings = this->m_context->getRings();
	ComputeLaplacian * comlap= (ComputeLaplacian *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_LAPLACIAN_SYSTEM));
	for(coord = 0; coord<3; coord++){

		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			if(idv->isDegenerateVertex(v,  rings[i])){
				solver->variable(i).lock();
			}
			sjPoint_3 point_k = v->point();
			double r = point_k[coord];
			solver->variable(i).set_value(r);		
				
				
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
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){

			if(!idv->isDegenerateVertex(v, rings[i])){
				sjHalfedge_vertex_circulator vcir = v->vertex_begin();
				map<int, double> mymap = comlap->computeLaplacian(v, rings[i]);
				map<int, double>::iterator it;
				solver->begin_row();
				for ( it=mymap.begin() ; it != mymap.end(); it++ ){
					int index = (*it).first ;
					double value = (*it).second;
					solver->add_coefficient(index, WLi * value);
					//cout<<WLi * value<<"\t\t";

				}
				//cout<<"\n";
				solver->set_right_hand_side(0.0);
				solver->end_row();

			}


			i++;
		}
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			if(!idv->isDegenerateVertex(v, rings[i])){
			sjHalfedge_vertex_circulator vcir = v->vertex_begin();
			sjPoint_3 point_i = v->point();
			double Whi;
			if(this->m_data->iteration ==0){
				Whi = this->m_data->WH_0;
			}else{
				Whi = this->m_data->WH_0*std::sqrt(v->initial_ring_area/areaRing(v, rings[i]));
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
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
				
			sjPoint_3 pointn = sjPoint_3(matrixV[i][0], matrixV[i][1], matrixV[i][2]);
			v->point() = pointn;
			i++;
		}
	}
	cout<<"Volume = "<<calcVolume(mesh_G)<<"|\t";
	cout<<"Time = "<< timer.time() << " seconds." << std::endl;
	delete solver;
	this->m_data->iteration++;
	return true;

}

PluginIterateLaplacianSmoothingSeparate::PluginIterateLaplacianSmoothingSeparate(){
	name = "PluginIterateLaplacianSmoothingSeparate";
	sjKernelPlugin & kernel = sjKernelPlugin::getInstance();
	name_type = kernel.SYS_ITERATE_SMOOTHING_ALGORITHM_SYSTEM;
	registerPlugin(kernel);
}

void PluginIterateLaplacianSmoothingSeparate::registerPlugin(sjKernelPlugin & K){
	K.addPlugin(this);
}

sjSystem * PluginIterateLaplacianSmoothingSeparate::createSystem(){
	return new IterateLaplacianSmoothingSeparate();
}

bool IterateLaplacianSmoothingIntegrate::evolve(sjStateContext * ssc){
	m_context = ssc;
	//int n = getNsize();
	int n = this->m_context->getNumberOfVertex();
	LinearSolver *solver;
	//delete solver;
	SystemSolverParameters params ;
	std::string  * mipo = new std::string("SUPERLU");
	params.set_arg_value("method", *mipo) ; 
	solver = new LinearSolver(n*3) ;
	solver->set_system_solver(params) ;
	solver->set_least_squares(true) ;
	solver->set_invert_matrix(true) ;

	sjPolyhedronPipe::PolyhedronType mesh_G = this->m_context->getMesh();


	cout<<"\nIterate system: No = "<<this->m_data->iteration+1<<"|\t";
		
	cout<<"Volume Ini= "<<calcVolume(mesh_G)<<"|\t";
	CGAL::Timer timer;
	timer.start();
	int i;
	sjVIterator v;
	int coord;
	vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
	double WLi = std::pow(this->m_data->SL,((double)(this->m_data->iteration))) * this->m_data->WL_0;
	//double WLi = 1.0;
	bool found_solution = true;

		

	//for(coord = 0; coord<3; coord++){

	IsDegenerateVertex * idv = (IsDegenerateVertex *) (sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_IS_DEGENERATE_VERTEX_SYSTEM));
	ComputeLaplacian * comlap =  (ComputeLaplacian *) (sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_LAPLACIAN_SYSTEM));
		i = 0;
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
			if(idv->isDegenerateVertex(v, this->m_context->getRings()[i])){
				solver->variable(i).lock();
				solver->variable(i+n).lock();
				solver->variable(i+2*n).lock();
			}
			sjPoint_3 point_k = v->point();
			double r = point_k[coord];
			//solver->variable(i).set_value(r);		
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
			for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){

				if(!idv->isDegenerateVertex(v, this->m_context->getRings()[i])){
					sjHalfedge_vertex_circulator vcir = v->vertex_begin();
					map<int, double> mymap = comlap->computeLaplacian(v, this->m_context->getRings()[i]);
					map<int, double>::iterator it;
					solver->begin_row();
					for ( it=mymap.begin() ; it != mymap.end(); it++ ){
						int index = (*it).first ;
						double value = (*it).second;
						solver->add_coefficient(index +coord*n , WLi * value);
						//cout<<WLi * value<<"\t\t";

					}
					//cout<<"\n";
					solver->set_right_hand_side(0.0);
					solver->end_row();

				}


				i++;
			}
		}
		int jcoord;
		for(jcoord = 0; jcoord<3; jcoord++){
			i = 0;
			for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
				if(!idv->isDegenerateVertex(v, this->m_context->getRings()[i])){
				sjHalfedge_vertex_circulator vcir = v->vertex_begin();
				sjPoint_3 point_i = v->point();
				double Whi;
				if(this->m_data->iteration ==0){
					Whi = this->m_data->WH_0;
				}else{
					Whi = this->m_data->WH_0*std::sqrt(v->initial_ring_area/areaRing(v, this->m_context->getRings()[i]));
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
		for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
				
			sjPoint_3 pointn = sjPoint_3(matrixV[i][0], matrixV[i][1], matrixV[i][2]);
			v->point() = pointn;
			i++;
		}
	}
	cout<<"Volume = "<<calcVolume(mesh_G)<<"|\t";
	cout<<"Time = "<< timer.time() << " seconds." << std::endl;
	delete solver;
	this->m_data->iteration++;
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

OFFLoaderSource::OFFLoaderSource(std::string filename){
	m_filename = filename;
}

sjPolyhedronPipe::PolyhedronType * OFFLoaderSource::produce(){
	sjDataIO dataio;
	dataio.setFileName(m_filename );
	try{
		dataio.load();
		return &(dataio.getPolyhedronModel());
	}catch(std::exception e) {
	}
	return NULL;
}


