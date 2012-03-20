#include "sjCoreSmoothing.h"
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

bool InitIndex::evolve(sjStateContext * ssc){
	sjLogDebug("InitIndex::evolve");
	int i = 0;
	for ( sjVIterator v1 = STATE_MESH.vertices_begin(); v1 != STATE_MESH.vertices_end(); ++v1){
		sjHalfedge_vertex_circulator vcir = v1->vertex_begin();
		vcir->vertex()->index = i;
		vcir->vertex()->initial_ring_area = 0.0;
		i++;
	}
	m_context->setState((sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_RINGS_SYSTEM)));
	return m_context->evolve(1);
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

bool ComputeRings::evolve(sjStateContext * ssc){
	sjLogDebug("ComputeRings::evolve");
	m_context = ssc;
	STATE_RINGS.clear();
	for ( sjVIterator v = STATE_MESH.vertices_begin(); v != STATE_MESH.vertices_end(); ++v){
		sjHalfedge_vertex_circulator vcir = v->vertex_begin();
		vector< sjVertex_handle> neighbors;
		do{
			sjVertex_handle punto = (vcir->next()->vertex());
			neighbors.push_back(punto);
		}while(++vcir != v->vertex_begin ());
		vcir->vertex()->initial_ring_area = areaRing(v, neighbors);
		STATE_RINGS.push_back(neighbors);
	}
	m_context->setState((sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_INIT_LAPLACIAN_SMOOTHING_SYSTEM)));
	return m_context->evolve(1);
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

bool InitLaplacianSmoothing::evolve(sjStateContext * ssc){
	sjLogDebug("InitLaplacianSmoothing::evolve");
	initLaplacianSmoothing();
	m_context = ssc;
	m_context->setState((sjState *)(sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_ITERATE_SMOOTHING_ALGORITHM_SYSTEM)));
	return m_context->evolve(1);
}

void InitLaplacianSmoothing::initLaplacianSmoothing(double a_WH0, double a_WL0, double a_SL){
	sjLogDebug("InitLaplacianSmoothing::initLaplacianSmoothing");
	this->m_context->AVERAGE_FACE = averageFaces(STATE_MESH);
	//WH_0 = a_WH0;
	this->m_context->WH_0 = 1.0;
	//WL_0 = a_WL0 * std::sqrt(AVERAGE_FACE);
	this->m_context->WL_0 = 1.0;
	this->m_context->SL = a_SL;
	int i = 0;
	ComputeLineEquations * cle = (ComputeLineEquations *) (sjKernelPlugin::getInstance().getSystem(sjKernelPlugin::SYS_COMPUTE_LINE_EQUATIONS_SYSTEM));
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

OFFLoaderSource::OFFLoaderSource(string filename){
	printf("OFFLoaderSource + %s 1\n", filename.c_str());
	m_filename = filename;
	printf("OFFLoaderSource 2\n");
}
sjPolyhedronPipe::PolyhedronType OFFLoaderSource::produce(){
	printf("OFFLoaderSource produce 1\n");
	sjDataIO dataio;
	dataio.setFileName(m_filename );
	try{
		dataio.load();
		printf("OFFLoaderSource produce 2\n");
		return dataio.getPolyhedronModel();
	}catch(std::exception e) {
	}
	return sjPolyhedronPipe::PolyhedronType();
}