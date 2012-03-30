#include "sjSimplificator.h"

#include <algorithm>

using namespace sj;
using namespace std;
using namespace OGF;

sjSimplificator::sjSimplificator(){
	
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
	}while(++vcir_B != he->vertex()->vertex_begin ());

	set_intersection(neighbors_A.begin(),neighbors_A.end(), neighbors_B.begin(), neighbors_B.end(),
    back_inserter(neighbors_intersection_AB));
	if(neighbors_intersection_AB.size()>2){
		return false;
	}else{
		if(neighbors_intersection_AB.size() == 2){
			if(he->face()!= NULL && he->opposite()->face() != NULL ){
				return true;
			}else{
				return false;
			}
		}else{
			return true;
		}
	}
	return false;
}