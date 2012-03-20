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

	double WLi = std::pow(m_context->SL,((double) m_context->iteration)) * m_context->WL_0;
	WLi = 0.1;

	double A = 0.0;
	double Wii = 0.0;
	for(j_pos = 0; j_pos < (int)(vecinos.size()); j_pos++){
				
		j_Alpha_pos = ((j_pos + 1)+vecinos.size()) % vecinos.size();
		j_Beta_pos = ((j_pos - 1)+vecinos.size()) % vecinos.size();
		sjPoint_3 point_Alpha = ((vecinos[j_Alpha_pos]))->point();
		sjPoint_3 point_Beta = ((vecinos[j_Beta_pos]))->point();
		double Alpha = angle3(point_i, point_Alpha, ((vecinos[j_pos]))->point());
		if(Alpha < m_context->MIN_COT_ANGLE)				Alpha = m_context->MIN_COT_ANGLE;
		double Beta = angle3(point_i, point_Beta, ((vecinos[j_pos]))->point());
		if(Beta < m_context->MIN_COT_ANGLE)				Beta = m_context->MIN_COT_ANGLE;

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
	(m_context->iteration)++;
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

bool MeanCurvatureSmoothing::evolve(sjStateContext * ssc){
	size_t n = this->m_context->getNumberOfVertex();// getNsize();
	vector<vector<double>> matrixV = vector<vector<double>>(n,vector<double>(3,0.0));
	sjVIterator v;
			
	int i = 0;

	ComputeMeanCurvature * cmc = (ComputeMeanCurvature * )
								( sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_MEAN_CURVATURE_SYSTEM));
	//sjPolyhedron mesh_G = this->m_context->getMesh();
	//vector< vector<sjVertex_handle> > & rings = *(this->m_context->getRings());
	//for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
	for ( v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
		//sjPoint_3 newvi = computeMeanCurvature(v, rings[i]);
		//sjPoint_3 newvi = cmc->computeMeanCurvature(v, rings[i]);
		sjPoint_3 newvi = cmc->computeMeanCurvature(v, STATE_RINGS[i]);
		matrixV[i][0] = newvi.x();
		matrixV[i][1] = newvi.y();
		matrixV[i][2] = newvi.z();
		i++;
	}
	i = 0;
	//for ( v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
	for ( v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
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
