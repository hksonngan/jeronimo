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

void sjSimplificator::convertPolyhedronToSkeleton(){
	sjskeleton.init(mesh_G.size_of_vertices(), mesh_G.size_of_halfedges());
	for ( sjVIterator v1 = mesh_G.vertices_begin(); v1 != mesh_G.vertices_end(); ++v1){
		sjGraphPoint p(v1->point().x(), v1->point().y(), v1->point().z(), v1->index);
		sjskeleton.insertPointData(p);
	}
	for(sjHalfedge_handle he = mesh_G.halfedges_begin(); he != mesh_G.halfedges_end(); ++he){
		sjGraphHalfedge ghe(he->opposite()->vertex()->index, he->vertex()->index, he->hedgeid, he->opposite()->hedgeid);
		sjskeleton.insertHalfedgeData(ghe);
	}
}

Matrix4d sjSimplificator::getFundamentalErrorQuadric(int heh){
	sjPoint_3 point_inc = sjskeleton.getPointFromHalfedgeId(heh).getPoint_3();
	sjPoint_3 point_opp = sjskeleton.getPointFromHalfedgeId(sjskeleton.getHalfedgeOppositeIdFromHalfedgeId(heh)).getPoint_3();

	sjVector_3 a = normalizeVector(point_inc - point_opp );
	sjVector_3 vi = point_opp - sjPoint_3(0.0, 0.0, 0.0);
	sjVector_3 b = CGAL::cross_product(a, vi);
	
	Matrix4d K;

	K ( 0, 0 ) = 0.0;		K ( 0, 1 ) = -a.z();	K ( 0, 2 ) = a.y();		K ( 0, 3 ) = -b.x();
	K ( 1, 0 ) = a.z();		K ( 1, 1 ) = 0.0;		K ( 1, 2 ) = -a.x();	K ( 1, 3 ) = -b.y();
	K ( 2, 0 ) = -a.y();	K ( 2, 1 ) = a.x();		K ( 2, 2 ) = 0.0;		K ( 2, 3 ) = -b.z();
	K ( 3, 0 ) = 0.0;		K ( 3, 1 ) = 0.0;		K ( 3, 2 ) = 0.0;		K ( 3, 3 ) = 0.0;

	return K;
}

double sjSimplificator::calculateSamplingCost(int he){
	sjPoint_3 point_inc = sjskeleton.getPointFromHalfedgeId(he).getPoint_3();
	sjPoint_3 point_opp = sjskeleton.getPointFromHalfedgeId(sjskeleton.getHalfedgeOppositeIdFromHalfedgeId(he)).getPoint_3();
	sjVector_3			v = point_opp - point_inc;
	double			  dij = sqrt(v.squared_length () );
	double			 dtot = 0;

	set<int> neighbors_opposite = sjskeleton.getNeighborsToPoint(sjskeleton.halfedges_data[he].point_opposite_id);

	for(set<int>::iterator it=neighbors_opposite.begin(); it!=neighbors_opposite.end(); it++){
		sjPoint_3 pk = sjskeleton.getPointFromHalfedgeId(*it).getPoint_3();
		v = point_opp - pk;
		dtot = dtot + sqrt(v.squared_length());
	}

	return dij*dtot;
}

Matrix4d sjSimplificator::computeInitialQ(int vid){
	Matrix4d Q;
	Matrix4d K;
	Matrix4d Kt;

	// loop around the edges around this vertex and add up the K matrices
	Q.load_zero();
	//sjHalfedge_handle he = v->halfedge();
	
	set<int> incident_hedges = sjskeleton.points_set_halfedges[vid];
	for(set<int>::iterator it=incident_hedges.begin(); it!=incident_hedges.end(); it++){
		K = getFundamentalErrorQuadric( sjskeleton.getHalfedgeOppositeIdFromHalfedgeId(*it) );
		Kt = K.transpose();
		Q = Q + (Kt*K);
	}

	/*do {
		K = getFundamentalErrorQuadric( he );
		Kt = K.transpose();
		Q = Q + (Kt*K);
		he = he->prev_on_vertex();
	} while ( he != v->halfedge() );*/

	return Q;
}

/*
                    s*
                    / \
				   /   \
				  /     \
				p*<--pq--*q
				  \     /
				   \   /
				    \ /
					r*
	link condition
*/

/*bool sjSimplificator::isCollapseTunnel(sjHalfedge_handle he){
	char strbuf[256];
	if(he->hedgeid==-1 || he == mesh_G.halfedges_end() ){
		return false;
	}

	bool is_face_pqr;
	bool is_face_qps;
	
	is_face_pqr = he->is_triangle();
	is_face_qps = he->opposite()->is_triangle();
	
	
	sjHalfedge_vertex_circulator vcir_A = he->vertex()->vertex_begin();
	sjHalfedge_vertex_circulator vcir_B = he->opposite()->vertex()->vertex_begin();

	vector<int> neighbors_A;
	vector<int> neighbors_B;
	vector<int> neighbors_intersection_AB;
	do{
		int punto = (vcir_A->next()->vertex()->index);
		neighbors_A.push_back(punto);
	}while(++vcir_A != he->vertex()->vertex_begin ());

	do{
		int punto = (vcir_B->next()->vertex()->index);
		neighbors_B.push_back(punto);
	}while(++vcir_B != he->opposite()->vertex()->vertex_begin());

	set_intersection(neighbors_A.begin(),neighbors_A.end(), neighbors_B.begin(), neighbors_B.end(),
    back_inserter(neighbors_intersection_AB));
	if(neighbors_intersection_AB.size()>2){
		return false;
	}else{
		if(neighbors_intersection_AB.size() == 2){
			if(he->is_triangle() && he->opposite()->is_triangle() ){
				return true;
			}else{
				return false;
			}
		}else{
			return true;
		}
	}
	return false;
}*/

double sjSimplificator::calculateShapeCost(Matrix4d Qi, Matrix4d Qj,  int he){
	Matrix4d Q = Qi + Qj;
	//sjPoint_3 p = he->opposite()->vertex()->point();
	sjPoint_3 p = sjskeleton.getPointFromHalfedgeId(sjskeleton.getHalfedgeOppositeIdFromHalfedgeId(he)).getPoint_3();
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

double sjSimplificator::calculateTotalCost(int he){
	//double Fa = calculateShapeCost(Qmap[he->vertex()->index], Qmap[he->opposite()->vertex()->index], he);
	double Fa = calculateShapeCost(Qmap[sjskeleton.halfedges_data[he].point_opposite_id ], Qmap[sjskeleton.halfedges_data[he].point_incident_id], he);
	double Fb = calculateSamplingCost(he);
	double total = Wa*Fa + Wb*Fb;
	return total;
}

void sjSimplificator::computeAllInitialQ(){
	sjLogDebug("computeAllInitialQ 1");
	for(vector<sjGraphPoint>::iterator it=sjskeleton.points_data.begin(); it!=sjskeleton.points_data.end();it++){
		Qmap[(*it).id] = computeInitialQ((*it).id);
	}
	/*for(sjVertex_handle v = mesh_G.vertices_begin(); v != mesh_G.vertices_end(); ++v){
		Qmap[v->index]	 = computeInitialQ(v);
	}*/
	sjLogDebug("computeAllInitialQ 2");
}

void sjSimplificator::computeHeapError(){
	sjLogDebug("computeHeapError 1");
	for(vector<sjGraphHalfedge>::iterator it=sjskeleton.halfedges_data.begin(); it!=sjskeleton.halfedges_data.end(); it++){
		sjNodeHeap node((*it).id, calculateTotalCost((*it).id));
		heap_error.push_back(node);
	}
	/*for(sjHalfedge_handle he = mesh_G.halfedges_begin(); he != mesh_G.halfedges_end(); ++he){
		
		sjNodeHeap node(he, calculateTotalCost(he));
		heap_error.push_back(node);
	}*/
	heap_error.sort();
	cout<<"HeapError Size: "<<heap_error.size()<<endl;
	sjLogDebug("computeHeapError 2");
}

void sjSimplificator::init(){
	convertPolyhedronToSkeleton();
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

void sjSimplificator::collapseEdge(int he){
	int vi, vj;
	vi = sjskeleton.halfedges_data[he].point_opposite_id;
	vj = sjskeleton.halfedges_data[he].point_incident_id;
	Qmap[vj] = Qmap[vi] + Qmap[vj];
	//mesh_G.join_vertex(he->opposite());
	sjskeleton.collapseHalfedge(he);
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
	if(heap_iterator == heap_error.end()){
		sjLogDebug("getValidEdgeToCollapse 2 END");
	}
	sjLogDebug("getValidEdgeToCollapse 2");
	bool stop = false;
	sjLogDebug("getValidEdgeToCollapse 3");
	//sjHalfedge_handle he = getHalfedgeFromID(heap_iterator->index);
	int he = heap_iterator->hedge;
	sprintf(strbuf, "getValidEdgeToCollapse 3-> %d", he);
	sjLogDebug(string(strbuf));
	sjLogDebug("getValidEdgeToCollapse 4");
	//while(heap_iterator !=heap_error.end() && (he != mesh_G.halfedges_end() || he->hedgeid !=-1) && !isCollapseTunnel(he)){
	while(!sjskeleton.isCollapseTunnel(he)){
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
		sprintf(strbuf, "getValidEdgeToCollapse 7.1 hedgeid-> %d", he);
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
		sjskeleton.printData();
		sjLogDebug("iterate 4");
		list<sjNodeHeap>::iterator heap_iterator = getValidEdgeToCollapse();
		sjLogDebug("iterate 5");
		if(heap_iterator != heap_error.end()){
			sjLogDebug("iterate 6");
			//sjHalfedge_handle he = getHalfedgeFromID(heap_iterator->index);
			int he = heap_iterator->hedge;
			sjNodeHeap nh = (*heap_iterator);
			collapseEdge(he);
			list<sjNodeHeap>::iterator heap_iterator2;
			for(heap_iterator2 = heap_error.begin();heap_iterator2!=heap_error.end(); ++heap_iterator2){
				if(heap_iterator2->hedge == sjskeleton.getHalfedgeOppositeIdFromHalfedgeId(he))
					break;
			}
			
			heap_error.erase(heap_iterator);
			heap_error.erase(heap_iterator2);
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