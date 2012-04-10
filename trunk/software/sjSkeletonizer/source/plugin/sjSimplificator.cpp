#include "sjSimplificator.h"

#include <algorithm>

using namespace sj;
using namespace std;
using namespace OGF;

sjSimplificator::sjSimplificator(double wa, double wb){
	sjLogDebug("sjSimplificator");
	Wa = wa;
	Wb = wb;
	m_init = false;
}

Matrix4d sjSimplificator::getFundamentalErrorQuadric(sjHalfedge_handle heh){
	sjVector_3 a = normalizeVector( heh->vertex()->point() - heh->opposite()->vertex()->point());
	sjVector_3 vi = heh->vertex()->point() - sjPoint_3(0.0, 0.0, 0.0);
	sjVector_3 b = CGAL::cross_product(a, vi);
	
	Matrix4d K;

	K ( 0, 0 ) = 0.0;		K ( 0, 1 ) = -a.z();	K ( 0, 2 ) = a.y();		K ( 0, 3 ) = -b.x();
	K ( 1, 0 ) = a.z();		K ( 1, 1 ) = 0.0;		K ( 1, 2 ) = -a.x();	K ( 1, 3 ) = -b.y();
	K ( 2, 0 ) = -a.y();	K ( 2, 1 ) = a.x();		K ( 2, 2 ) = 0.0;		K ( 2, 3 ) = -b.z();
	K ( 3, 0 ) = 0.0;		K ( 3, 1 ) = 0.0;		K ( 3, 2 ) = 0.0;		K ( 3, 3 ) = 0.0;

	return K;
}

double sjSimplificator::calculateSamplingCost(sjHalfedge_handle he){
	sjVector_3			v = he->vertex()->point() - he->opposite()->vertex()->point();
	double			  dij = sqrt(v.squared_length () );
	double			 dtot = 0;
	sjVertex_handle  vert = he->vertex();

	do{
		v = he->vertex()->point() - he->opposite()->vertex()->point();
		dtot = dtot + sqrt(v.squared_length());
		//OGF::: he = he->prev_around_vertex();
		he = he->prev_on_vertex();
	} while ( he != vert->halfedge() );

	return dij*dtot;
}

Matrix4d sjSimplificator::computeInitialQ(sjVertex_handle v){
	Matrix4d Q;
	Matrix4d K;
	Matrix4d Kt;

	// loop around the edges around this vertex and add up the K matrices
	Q.load_zero();
	sjHalfedge_handle he = v->halfedge();
	
	do {
		K = getFundamentalErrorQuadric( he );
		Kt = K.transpose();
		Q = Q + (Kt*K);
		he = he->prev_on_vertex();
	} while ( he != v->halfedge() );
	return Q;
}

bool sjSimplificator::isCollapseTunnel(sjHalfedge_handle he){
	char strbuf[256];
	if(he->hedgeid==-1 || he == mesh_G.halfedges_end() ){
		sjLogDebug("isCollapseTunnel 0");
		return false;
	}
	sjLogDebug("isCollapseTunnel 1");
	sjHalfedge_vertex_circulator vcir_A = he->vertex()->vertex_begin();
	sjLogDebug("isCollapseTunnel 2");
	sjHalfedge_vertex_circulator vcir_B = he->opposite()->vertex()->vertex_begin();
	sjLogDebug("isCollapseTunnel 3");
	
	vector<int> neighbors_A;
	vector<int> neighbors_B;
	vector<int> neighbors_intersection_AB;
	do{
		sjLogDebug("isCollapseTunnel 3.1");
		if(vcir_A == NULL){
			sjLogDebug("isCollapseTunnel 3.1.1 null");
		}else{
			sjLogDebug("isCollapseTunnel 3.1.1 not null");
		}
		if(vcir_A->next() == NULL){
			sjLogDebug("isCollapseTunnel 3.2 null");
		}else{
			sjLogDebug("isCollapseTunnel 3.2 not null");
		}
		if((&vcir_A->next()) == NULL){
			sjLogDebug("isCollapseTunnel 3.2.1.0 null");
		}else{
			sjLogDebug("isCollapseTunnel 3.2.1.0 not null");
		}

		
		
		
		if(vcir_A->next()->hedgeid == -1){
			sjLogDebug("isCollapseTunnel 3.2.1 es -1");
		}else{
			cout<<"isCollapseTunnel 3.2.1 es -1"<<vcir_A->next()->hedgeid<<endl;
		}
		if(vcir_A->next()->vertex() == NULL){
			sjLogDebug("isCollapseTunnel 3.3 null");
		}else{
			sjLogDebug("isCollapseTunnel 3.3 not null");
		}
		int punto = (vcir_A->next()->vertex()->index);
		sjLogDebug("isCollapseTunnel 3.4");
		neighbors_A.push_back(punto);
		sjLogDebug("isCollapseTunnel 3.5");
	}while(++vcir_A != he->vertex()->vertex_begin ());

	sjLogDebug("isCollapseTunnel 4");

	do{
		sjLogDebug("isCollapseTunnel 4.1");
		int punto = (vcir_B->next()->vertex()->index);
		neighbors_B.push_back(punto);
	}while(++vcir_B != he->opposite()->vertex()->vertex_begin());
	sjLogDebug("isCollapseTunnel 5");

	set_intersection(neighbors_A.begin(),neighbors_A.end(), neighbors_B.begin(), neighbors_B.end(),
    back_inserter(neighbors_intersection_AB));
	sjLogDebug("isCollapseTunnel 6");
	if(neighbors_intersection_AB.size()>2){
		sjLogDebug("isCollapseTunnel 7");
		return false;
	}else{
		sjLogDebug("isCollapseTunnel 8");
		if(neighbors_intersection_AB.size() == 2){
			sjLogDebug("isCollapseTunnel 9");
			if(he->face()!= NULL && he->opposite()->face() != NULL ){
				sjLogDebug("isCollapseTunnel 10");
				return true;
			}else{
				sjLogDebug("isCollapseTunnel 11");
				return false;
			}
		}else{
			sjLogDebug("isCollapseTunnel 12");
			return true;
		}
	}
	sjLogDebug("isCollapseTunnel 13");
	return false;
}

double sjSimplificator::calculateShapeCost(Matrix4d Qi, Matrix4d Qj,  sjHalfedge_handle he){
	Matrix4d Q = Qi + Qj;
	sjPoint_3 p = he->opposite()->vertex()->point();
	// SHAPE COST
	// Multiply v'*Q*v
	// remember that v is in homogeneous coordinates
	double v[4];
	double Qv[4];
	v[0] = p.x(); v[1] = p.y(); v[2] = p.z(); v[3] = 1.;
	mult ( Q, v, Qv );
	double shapeError = Qv[0]*v[0] + Qv[1]*v[1] + Qv[2]*v[2] + Qv[3]*v[3];
	return shapeError;
}

double sjSimplificator::calculateTotalCost(sjHalfedge_handle he){
	double Fa = calculateShapeCost(Qmap[he->vertex()->index], Qmap[he->opposite()->vertex()->index], he);
	double Fb = calculateSamplingCost(he);
	double total = Wa*Fa + Wb*Fb;
	return total;
}

void sjSimplificator::computeAllInitialQ(){
	sjLogDebug("computeAllInitialQ 1");
	for(sjVertex_handle v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
		Qmap[v->index]	 = computeInitialQ(v);
	}
	sjLogDebug("computeAllInitialQ 2");
}

void sjSimplificator::computeHeapError(){
	sjLogDebug("computeHeapError 1");
	for(sjHalfedge_handle he = mesh_G.halfedges_begin(); he != mesh_G.halfedges_end(); ++he){
		
		sjNodeHeap node(he, calculateTotalCost(he));
		heap_error.push_back(node);
	}
	heap_error.sort();
	sjLogDebug("computeHeapError 2");
}

void sjSimplificator::init(){
	sjLogDebug("init 1");
	computeAllInitialQ();
	sjLogDebug("init 2");
	computeHeapError();
	sjLogDebug("init 3");
	for(sjHalfedge_handle he = mesh_G.halfedges_begin(); he != mesh_G.halfedges_end(); ++he){
		sjPoint_3 p3 = he->vertex()->point();
		char strbuf [80];
		sprintf(strbuf, "init 4: hedgeid=%d, vertex->id %d, point: %f, %f, %f", he->hedgeid, he->vertex()->index, 
			p3.x(), p3.y(), p3.z());
		sjLogDebug(std::string(strbuf));
	}
}

void sjSimplificator::collapseEdge(sjHalfedge_handle he){
	sjVertex_handle vi, vj;
	vi = he->vertex();
	vj = he->opposite()->vertex();
	Qmap[vi->index] = Qmap[vi->index] + Qmap[vj->index];
	mesh_G.join_vertex(he->opposite());
}

/*sjHalfedge_handle sjSimplificator::getHalfedgeFromID(int id){
	char strbuf [80];
	sprintf(strbuf, "getHalfedgeFromID 1: id: %d", id);
	sjLogDebug(string(strbuf));
	sjHalfedge_handle he = mesh_G.halfedges_begin();
	sjLogDebug("getHalfedgeFromID 2");
	while(he != mesh_G.halfedges_end() && he->hedgeid != id){
		sprintf(strbuf, "getHalfedgeFromID 3: id: %d", he->hedgeid);
		sjLogDebug(string(strbuf));
		++he;
	}
	sprintf(strbuf, "getHalfedgeFromID 4: id: %d", he->hedgeid);
	sjLogDebug(string(strbuf));
	return he;
}*/

list<sjNodeHeap>::iterator sjSimplificator::getValidEdgeToCollapse(){
	char strbuf [80];
	sjLogDebug("getValidEdgeToCollapse 1");
	list<sjNodeHeap>::iterator heap_iterator = heap_error.begin();
	sjLogDebug("getValidEdgeToCollapse 2");
	bool stop = false;
	sjLogDebug("getValidEdgeToCollapse 3");
	//sjHalfedge_handle he = getHalfedgeFromID(heap_iterator->index);
	sjHalfedge_handle he = heap_iterator->hedge;
	sprintf(strbuf, "getValidEdgeToCollapse 3-> %d", he->hedgeid);
	sjLogDebug(string(strbuf));
	sjLogDebug("getValidEdgeToCollapse 4");
	//while(heap_iterator !=heap_error.end() && (he != mesh_G.halfedges_end() || he->hedgeid !=-1) && !isCollapseTunnel(he)){
	while(!isCollapseTunnel(he)){
		sjLogDebug("getValidEdgeToCollapse 5");
		heap_iterator++;
		sjLogDebug("getValidEdgeToCollapse 6");
		if(heap_iterator !=heap_error.end()){
			sjLogDebug("getValidEdgeToCollapse 7");
			//he = getHalfedgeFromID(heap_iterator->index);
			he = heap_iterator->hedge;
		}else{
			break;
		}
		sprintf(strbuf, "getValidEdgeToCollapse 7.1 hedgeid-> %d", he->hedgeid);
		sjLogDebug(string(strbuf));
		sjLogDebug("getValidEdgeToCollapse 8");
	}
	sjLogDebug("getValidEdgeToCollapse 9");
	return heap_iterator;
}


sjPolyhedronPipe::PolyhedronType sjSimplificator::iterate(){
	sjLogDebug("iterate 1");
	char strbuf[512];
	if(m_init == false){
		sjLogDebug("iterate 2");
		this->mesh_G = input_pipe->read();
		if(mesh_G.is_valid()){
			sjLogDebug("iterate 2.1 Es Valido");
		}else{
			sjLogDebug("iterate 2.1 No Es Valido");
		}
		
		init();
		sjLogDebug("iterate 3");
		m_init = true;
	}else{
		sjLogDebug("iterate 4");
		list<sjNodeHeap>::iterator heap_iterator = getValidEdgeToCollapse();
		sjLogDebug("iterate 5");
		if(heap_iterator != heap_error.end()){
			sjLogDebug("iterate 6");
			//sjHalfedge_handle he = getHalfedgeFromID(heap_iterator->index);
			sjHalfedge_handle he = heap_iterator->hedge;
			sjNodeHeap nh = (*heap_iterator);
			collapseEdge(he);
			heap_iterator = heap_error.erase(heap_iterator);
			heap_error.push_back(*heap_iterator);
			sprintf(strbuf, "iterate 6.1: V=%d, E=%d", mesh_G.size_of_vertices(), mesh_G.size_of_halfedges());
			sjLogDebug(string(strbuf));
			cout<<strbuf<<endl;
		}
		sjLogDebug("iterate 7");
	}
	return getMesh();
}

void sjSimplificator::proccesEvent(sjEvent * evt){
	if(evt->getType() == sjEvent::EVT_ITERATE){
		this->update();
	}
}