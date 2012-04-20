#ifndef __SJ_GRAPH_LIST_H__
#define __SJ_GRAPH_LIST_H__

#include <iterator>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

#define SJGRAPH_INVALID_ID -1
#define SJGRAPH_NOT_FACE -2

namespace sj{


class sjGraphHalfedge{
public:
	int point_opposite_id;
	int point_incident_id;
	int id;
	int hedge_opposite_id;

	sjGraphHalfedge(int a_point_opposite_id		=	SJGRAPH_INVALID_ID,
					int a_point_incident_id		=	SJGRAPH_INVALID_ID,
					int				   a_id		=	SJGRAPH_INVALID_ID,
					int a_hedge_opposite_id		=	SJGRAPH_INVALID_ID):	
						  point_opposite_id			(a_point_opposite_id),	
						  point_incident_id			(a_point_incident_id),	
										 id			(a_id),	
						  hedge_opposite_id			(a_hedge_opposite_id)
	{}

	bool isValid(){
		if( point_opposite_id == SJGRAPH_INVALID_ID 
			|| point_incident_id == SJGRAPH_INVALID_ID 
			|| id == SJGRAPH_INVALID_ID)
			return false;
		return true;
	}
};

class sjGraphFace{
public:
	int id;
	int point_a_id;
	int point_b_id;
	int point_c_id;

	sjGraphFace(
		int apoint_a_id	= SJGRAPH_INVALID_ID, 
		int apoint_b_id	= SJGRAPH_INVALID_ID,	
		int apoint_c_id	= SJGRAPH_INVALID_ID, 
		int			aid = SJGRAPH_INVALID_ID):
	id(aid),
	point_a_id(apoint_a_id),
	point_b_id(apoint_b_id),
	point_c_id(apoint_c_id)
	{}

	bool isValid(){
		if( id == SJGRAPH_INVALID_ID ||
		point_a_id == SJGRAPH_INVALID_ID ||
		point_b_id == SJGRAPH_INVALID_ID ||
		point_c_id == SJGRAPH_INVALID_ID)
			return false;
		return true;
	}
};

template <class T_PointClass, class T_GetIdClass>
class sjGraphSkeleton{
public:
	typedef typename T_PointClass PointType;
	vector<PointType>			points_data;
	vector<bool>				points_bool;
	vector<sjGraphHalfedge>		halfedges_data;
	vector<bool>				halfedges_bool;
	vector<sjGraphFace>			faces_data;
	vector<bool>				faces_bool;
	vector<set<int> >			points_set_halfedges;	
	vector<set<int> >			halfedges_set_faces;	

	sjGraphSkeleton(){}

	void init(int a_size_points, int a_size_halfedges, int a_size_faces){
		points_data = vector<PointType>(a_size_points, PointType());
		points_bool = vector<bool>(a_size_points, false);
		points_set_halfedges = vector<set<int> >(a_size_points, set<int>());

		halfedges_data = vector<sjGraphHalfedge>(a_size_halfedges, sjGraphHalfedge());
		halfedges_bool = vector<bool>(a_size_halfedges, false);
		halfedges_set_faces = vector<set<int>>(a_size_halfedges, set<int>());
		
		faces_data = vector<sjGraphFace>(a_size_faces, sjGraphFace());
		faces_bool = vector<bool>(a_size_faces, false);
		

	}


	PointType getPointFromHalfedgeId(int hedge_id){
		return points_data[halfedges_data[hedge_id].point_incident_id];
	}

	int getHalfedgeOppositeIdFromHalfedgeId(int hedge_id){
		return halfedges_data[hedge_id].hedge_opposite_id;
	}
	
	int getIdFromPoint(PointType p){
		T_GetIdClass getid;

		return getid.getID(p);
	}

	bool insertPointData(PointType p){
		int index = getIdFromPoint(p);
		if(index == SJGRAPH_INVALID_ID) return false;
		if(index >= points_data.size()) return false;

		points_data[index] = p;
		points_bool[index] = true;

		return true;
	}

	bool insertIncidentHalfedgeToPoint(int point_id, int hedge_id){
		if(point_id >= points_bool.size() || hedge_id >= halfedges_bool.size() || point_id<0 || hedge_id<0) return false;
		if(points_bool[point_id] == false || halfedges_bool[hedge_id] == false) return false;
		if(halfedges_data[hedge_id].point_incident_id != point_id) return false;

		points_set_halfedges[point_id].insert(hedge_id);

		return true;
	}

	bool insertHalfedgeData(sjGraphHalfedge he){
		if(!he.isValid()) return false;
		if(he.id >= halfedges_data.size()) return false;

		halfedges_data[he.id] = he;
		halfedges_bool[he.id] = true;

		sjGraphHalfedge * heop = new sjGraphHalfedge();
		heop->id = he.hedge_opposite_id;
		heop->hedge_opposite_id = he.id;
		heop->point_incident_id = he.point_opposite_id;
		heop->point_opposite_id = he.point_incident_id;

		halfedges_data[heop->id] = *heop;
		halfedges_bool[heop->id] = true;
		

		return insertIncidentHalfedgeToPoint(he.point_incident_id, he.id) && insertIncidentHalfedgeToPoint(heop->point_incident_id, heop->id);
	}

	bool insertFaceData(sjGraphFace fa){
		if(!fa.isValid()) return false;
		if(fa.id<0 || fa.id>=faces_data.size()) return false;
		faces_data[fa.id] = fa;
		faces_bool[fa.id] = true;

		set<int>::iterator it;
		for(it = points_set_halfedges[fa.point_a_id].begin(); it!=points_set_halfedges[fa.point_a_id].end(); it++){
			if(halfedges_data[*it].point_opposite_id == fa.point_c_id){
				halfedges_set_faces[*it].insert(fa.id);
			}
		}
		for(it = points_set_halfedges[fa.point_b_id].begin(); it!=points_set_halfedges[fa.point_b_id].end(); it++){
			if(halfedges_data[*it].point_opposite_id == fa.point_a_id ){
				halfedges_set_faces[*it].insert(fa.id);
			}
		}
		for(it = points_set_halfedges[fa.point_c_id].begin(); it!=points_set_halfedges[fa.point_c_id].end(); it++){
			if(halfedges_data[*it].point_opposite_id == fa.point_b_id){
				halfedges_set_faces[*it].insert(fa.id);
			}
		}
		
		return true;
	}

	set<int> getNeighborsToPoint(int point_id){
		set<int> neighbors = set<int>();
		if(point_id <0 || point_id>=points_bool.size() ) return neighbors;
		set<int>::iterator it;

		for (it= points_set_halfedges[point_id].begin(); it!=points_set_halfedges[point_id].end(); it++){
			int hedge_id = (int)(*it);
			int point_opposite_id = halfedges_data[hedge_id].point_opposite_id;
			neighbors.insert(point_opposite_id);
		}
		return neighbors;
	}

	bool changeHalfedgeIncidentPoint(int hegde_id, int point_id){
		if(point_id >= points_bool.size() || hegde_id >= halfedges_bool.size() || point_id<0 || hegde_id<0) return false;
		int hedge_opposite_id = halfedges_data[hegde_id].hedge_opposite_id;
		int old_point_incident_id = halfedges_data[hegde_id].point_incident_id;
		
		halfedges_data[hegde_id].point_incident_id = point_id;
		halfedges_data[hedge_opposite_id].point_opposite_id = point_id;
		points_set_halfedges[old_point_incident_id].erase(hegde_id);
		points_set_halfedges[point_id].insert(hegde_id);



		return true;
	}

	set<int> getHalfedgesIncidentToFace(int face_id){
		set<int> inci;
		if(face_id >= faces_bool.size() || face_id<0) return inci;
		sjGraphFace gf = faces_data[face_id];
		set<int> he_incident_to_point;
		set<int>::iterator it;
		
		he_incident_to_point = points_set_halfedges[gf.point_a_id];
		for(it=he_incident_to_point.begin(); it!=he_incident_to_point.end(); it++){
			if(halfedges_set_faces[*it].find(face_id)!=halfedges_set_faces[*it].end()){
				inci.insert(*it);
			}
		}

		he_incident_to_point = points_set_halfedges[gf.point_b_id];
		for(it=he_incident_to_point.begin(); it!=he_incident_to_point.end(); it++){
			if(halfedges_set_faces[*it].find(face_id)!=halfedges_set_faces[*it].end()){
				inci.insert(*it);
			}
		}

		he_incident_to_point = points_set_halfedges[gf.point_c_id];
		for(it=he_incident_to_point.begin(); it!=he_incident_to_point.end(); it++){
			if(halfedges_set_faces[*it].find(face_id)!=halfedges_set_faces[*it].end()){
				inci.insert(*it);
			}
		}

		return inci;
		
	}

	bool deleteHalfedge(int hedge_id){
		if(hedge_id >= halfedges_bool.size() || hedge_id<0) return false;
		int hedge_opposite_id = halfedges_data[hedge_id].hedge_opposite_id;
		int incident_point_id = halfedges_data[hedge_id].point_incident_id;
		int opposite_point_id = halfedges_data[hedge_id].point_opposite_id;

		points_set_halfedges[incident_point_id].erase(hedge_id);
		points_set_halfedges[opposite_point_id].erase(hedge_opposite_id);
		halfedges_bool[hedge_id] = false;
		halfedges_bool[hedge_opposite_id] = false;
		
		/*
		incident_faces_to_halfedges = halfedges_set_faces[hedge_id];
		for(it=incident_faces_to_halfedges.begin(); it!=incident_faces_to_halfedges.end(); it++){
			faces_bool[*it] = preserve_face;
			other_halfedges_incident_to_face = getHalfedgesIncidentToFace(*it);
			for(ihe=other_halfedges_incident_to_face.begin(); ihe!=other_halfedges_incident_to_face.end();ihe++){
				halfedges_set_faces[*ihe].erase(*it);
			}
		}

		incident_faces_to_halfedges = halfedges_set_faces[hedge_opposite_id];
		for(it=incident_faces_to_halfedges.begin(); it!=incident_faces_to_halfedges.end(); it++){
			faces_bool[*it] = preserve_face;
			other_halfedges_incident_to_face = getHalfedgesIncidentToFace(*it);
			for(ihe=other_halfedges_incident_to_face.begin(); ihe!=other_halfedges_incident_to_face.end();ihe++){
				halfedges_set_faces[*ihe].erase(*it);
			}
		}*/
		return true;
	}

	bool isFace(int pa_id, int pb_id, int pc_id){
		if(pa_id<0 || pa_id>=points_bool.size()) return false;
		if(pb_id<0 || pb_id>=points_bool.size()) return false;
		if(pc_id<0 || pc_id>=points_bool.size()) return false;
		if(!(points_bool[pa_id] && points_bool[pb_id] && points_bool[pc_id])) return false;

		set<int> all_faces;
		set<int>::iterator it;
		for(it = points_set_halfedges[pa_id].begin(); it != points_set_halfedges[pa_id].end(); it++){
			all_faces.insert(halfedges_set_faces[*it].begin(), halfedges_set_faces[*it].end());
			all_faces.insert(halfedges_set_faces[halfedges_data[*it].hedge_opposite_id].begin(), halfedges_set_faces[halfedges_data[*it].hedge_opposite_id].end());
		}
		for(it = points_set_halfedges[pb_id].begin(); it != points_set_halfedges[pb_id].end(); it++){
			all_faces.insert(halfedges_set_faces[*it].begin(), halfedges_set_faces[*it].end());
			all_faces.insert(halfedges_set_faces[halfedges_data[*it].hedge_opposite_id].begin(), halfedges_set_faces[halfedges_data[*it].hedge_opposite_id].end());
		}
		for(it = points_set_halfedges[pc_id].begin(); it != points_set_halfedges[pc_id].end(); it++){
			all_faces.insert(halfedges_set_faces[*it].begin(), halfedges_set_faces[*it].end());
			all_faces.insert(halfedges_set_faces[halfedges_data[*it].hedge_opposite_id].begin(), halfedges_set_faces[halfedges_data[*it].hedge_opposite_id].end());
		}
		for(it=all_faces.begin(); it!=all_faces.end(); it++){
			if(faces_bool[*it])
				if(
					(pa_id == faces_data[*it].point_a_id || pa_id == faces_data[*it].point_b_id || pa_id == faces_data[*it].point_c_id)
				&&  (pb_id == faces_data[*it].point_a_id || pb_id == faces_data[*it].point_b_id || pb_id == faces_data[*it].point_c_id)
				&&  (pc_id == faces_data[*it].point_a_id || pc_id == faces_data[*it].point_b_id || pc_id == faces_data[*it].point_c_id)
				) return true;
		}

		return false;
	}

	bool isCollapseTunnel(int hedge_id){
		if(hedge_id >= halfedges_bool.size() || hedge_id<0) return false;

		int pointAid = halfedges_data[hedge_id].point_incident_id;
		int pointBid = halfedges_data[hedge_id].point_opposite_id;

		set<int> neighbors_A = getNeighborsToPoint(pointAid);
		set<int> neighbors_B = getNeighborsToPoint(pointBid);
		vector<int> neighbors_intersection_AB;
		vector<int>::iterator it;

		set_intersection(neighbors_A.begin(),neighbors_A.end(), neighbors_B.begin(), neighbors_B.end(), back_inserter(neighbors_intersection_AB));

		if(neighbors_intersection_AB.size()>0){
			for(it=neighbors_intersection_AB.begin(); it!=neighbors_intersection_AB.end(); it++){
				if(!isFace(pointAid, pointBid, *it)) return false;
			}		
		}
		return true;
	}

	/*
                    s*
                    /|\
				   / | \
				  /  |  \
				c*---b---*q
				  \  |  /
				   \ v /
				    \|/
					a*
					
	*/

	bool exchangeAndDeleteFaces(int hedge_id){
		if(hedge_id<0 ||hedge_id>=halfedges_bool.size()) return false;

		int hedge_opposite_id = halfedges_data[hedge_id].hedge_opposite_id;
		int point_incident_id = halfedges_data[hedge_id].point_incident_id;	//a
		int point_opposite_id = halfedges_data[hedge_id].point_opposite_id;	//b

		set<int> faces_incident_to_hedge;
		set<int>::iterator itf, ith, ith2, itf2, itf3;
		
		int ida = point_incident_id;
		int idb = point_opposite_id;
		int idc ;
		int idf;
		
		faces_incident_to_hedge.insert(halfedges_set_faces[hedge_id].begin(), halfedges_set_faces[hedge_id].end());
		faces_incident_to_hedge.insert(halfedges_set_faces[hedge_opposite_id].begin(), halfedges_set_faces[hedge_opposite_id].end());

		for(itf = faces_incident_to_hedge.begin(); itf!=faces_incident_to_hedge.end(); itf++){
			
			idf = (*itf);
			if(faces_data[idf].point_a_id != ida && faces_data[idf].point_a_id != idb){
				idc = faces_data[idf].point_a_id;
			}else if(faces_data[idf].point_b_id != ida && faces_data[idf].point_b_id != idb){
				idc = faces_data[idf].point_b_id;
			}else{
				idc = faces_data[idf].point_c_id;
			}
			set<int> hedge_incident_bc_face_idf;
			set<int> hedge_incident_ca_face_idf;
			for(ith2 = points_set_halfedges[idb].begin(); ith2 != points_set_halfedges[idb].end(); ith2++){
				if(halfedges_data[*ith2].point_opposite_id == idc && halfedges_set_faces[*ith2].find(idf)!=halfedges_set_faces[*ith2].end()){
					hedge_incident_bc_face_idf.insert(*ith2);
				}

			}
			for(ith2 = points_set_halfedges[idc].begin(); ith2 != points_set_halfedges[idc].end(); ith2++){
				if(halfedges_data[*ith2].point_opposite_id == idb && halfedges_set_faces[*ith2].find(idf)!=halfedges_set_faces[*ith2].end()){
					hedge_incident_bc_face_idf.insert(*ith2);
				}
				if(halfedges_data[*ith2].point_opposite_id == ida && halfedges_set_faces[*ith2].find(idf)!=halfedges_set_faces[*ith2].end()){
					hedge_incident_ca_face_idf.insert(*ith2);
				}
			}
			for(ith2 = points_set_halfedges[ida].begin(); ith2 != points_set_halfedges[ida].end(); ith2++){
				if(halfedges_data[*ith2].point_opposite_id == idc && halfedges_set_faces[*ith2].find(idf)!=halfedges_set_faces[*ith2].end()){
					hedge_incident_ca_face_idf.insert(*ith2);
				}

			}
			set<int> faces_to_change;
			if(hedge_incident_bc_face_idf.size()>0){
				for(ith2=hedge_incident_bc_face_idf.begin(); ith2!=hedge_incident_bc_face_idf.end(); ith2++){
					sjGraphHalfedge heidf = halfedges_data[(*ith2)];
					faces_to_change.insert(halfedges_set_faces[heidf.id].begin(), halfedges_set_faces[heidf.id].end());
					faces_to_change.insert(halfedges_set_faces[heidf.hedge_opposite_id].begin(), halfedges_set_faces[heidf.hedge_opposite_id].end());
				}
				faces_to_change.erase(idf);
				for(ith2=hedge_incident_bc_face_idf.begin(); ith2!=hedge_incident_bc_face_idf.end(); ith2++){
					sjGraphHalfedge heidf = halfedges_data[(*ith2)];
					for(itf3=faces_to_change.begin(); itf3!=faces_to_change.end(); itf3++){
						halfedges_set_faces[heidf.id].erase(*itf3);
					}
				}
			}
			if(hedge_incident_ca_face_idf.size()>0){
				for(ith2=hedge_incident_ca_face_idf.begin(); ith2!=hedge_incident_ca_face_idf.end(); ith2++){
					sjGraphHalfedge heidf = halfedges_data[(*ith2)];
					halfedges_set_faces[heidf.id].erase(idf);	
					halfedges_set_faces[heidf.id].insert(faces_to_change.begin(), faces_to_change.end());
				}
			}
			for(itf2=faces_to_change.begin(); itf2!=faces_to_change.end(); itf2++){
				if(faces_data[*itf2].point_a_id ==idb){
					faces_data[*itf2].point_a_id = ida;
				}else if(faces_data[*itf2].point_b_id ==idb){
					faces_data[*itf2].point_b_id = ida;
				}else if(faces_data[*itf2].point_c_id ==idb){
					faces_data[*itf2].point_c_id = ida;
				}
			}
			faces_bool[idf] = false;
			halfedges_set_faces[hedge_id].erase(idf);
		}


		return true;
	}

	bool collapseHalfedge(int hedge_id){
		//set_intersection(neighbors_A.begin(),neighbors_A.end(), neighbors_B.begin(), neighbors_B.end(),    back_inserter(neighbors_intersection_AB));
		if(hedge_id<0 ||hedge_id>=halfedges_bool.size()) return false;
		if(!isCollapseTunnel(hedge_id)) return false;
		cout<<"collapseHalfedge"<<endl;

		int point_incident_id = halfedges_data[hedge_id].point_incident_id;	//a
		int point_opposite_id = halfedges_data[hedge_id].point_opposite_id;	//b
		int hedge_opposite_id = halfedges_data[hedge_id].hedge_opposite_id;

		set<int> neighbors_point_incident = getNeighborsToPoint(point_incident_id);
		set<int> neighbors_point_opposite = getNeighborsToPoint(point_opposite_id);
		vector<int> neighbors_intersection;
		set_intersection(neighbors_point_incident.begin(),neighbors_point_incident.end(), 
			neighbors_point_opposite.begin(), neighbors_point_opposite.end(),    
			back_inserter(neighbors_intersection));
		
		

		set<int> halfedges_to_delete;
		set<int> halfedges_to_changue;
		exchangeAndDeleteFaces(hedge_id);
		deleteHalfedge(hedge_id);
		

		set<int> halfedges_incident_to_point_opposite = points_set_halfedges[point_opposite_id];
		set<int>::iterator it;
		for (it= halfedges_incident_to_point_opposite.begin(); it!=halfedges_incident_to_point_opposite.end(); it++){
			int hedge_id_incident_to_b = (int)(*it);
			int point_id_opposite_to_b = halfedges_data[hedge_id_incident_to_b].point_opposite_id;
			
			set<int> set_neighbors_intersection = set<int>(neighbors_intersection.begin(), neighbors_intersection.end());

			if(set_neighbors_intersection.find(point_id_opposite_to_b) != set_neighbors_intersection.end()){
				halfedges_to_delete.insert(hedge_id_incident_to_b);
			}else{
				if(hedge_id_incident_to_b != halfedges_data[hedge_id].hedge_opposite_id ){
					halfedges_to_changue.insert(hedge_id_incident_to_b);
				}
			}
		}

		for (it= halfedges_to_delete.begin(); it!=halfedges_to_delete.end(); it++){
			deleteHalfedge(*it);
		}
		for (it= halfedges_to_changue.begin(); it!=halfedges_to_changue.end(); it++){
			changeHalfedgeIncidentPoint(*it, point_incident_id);
		}

		
	}

	
	void printData(){
		T_GetIdClass printObj;
		vector<PointType>::iterator it;
		for(it = points_data.begin(); it!=points_data.end(); it++){
			printObj.print(*it);
			/*if((*it).value<10)			cout<<"\t\t";
			else			cout<<"\t";*/
			printNeighbors(getIdFromPoint(*it));
			if(points_bool[(*it).id])cout<<"True";
			else cout<<"False";
			cout<<endl;
		}
		vector<sjGraphHalfedge>::iterator ithe;
		for(ithe = halfedges_data.begin(); ithe!=halfedges_data.end(); ithe++){
			cout<<"Halfedge ["<<(*ithe).id<<"]\t["<<(*ithe).hedge_opposite_id <<"] \t= { "<<(*ithe).point_opposite_id<<" -> "<<(*ithe).point_incident_id<<" }";
			cout<<"\t";
			if(halfedges_bool[(*ithe).id]) cout<<"True ";
			else  cout<<"False ";
			printFaces((*ithe).id);
			cout<<endl;
		}

		vector<sjGraphFace>::iterator itf;
		for(itf=faces_data.begin(); itf!=faces_data.end(); itf++){
			cout<<"Face ["<<(*itf).id<<"] = { "<<(*itf).point_a_id<<", "<<(*itf).point_b_id<<", "<<(*itf).point_c_id<<"} ";
			if(faces_bool[(*itf).id]) cout<<"True";
			else cout<<"False";
			cout<<endl;
		}
	}

	void printNeighbors(int point_id){
		set<int> pn = getNeighborsToPoint(point_id);
		cout<<" Neighbors={ ";
		set<int>::iterator it;
		for(it = pn.begin(); it!=pn.end(); it++){
			cout<<*it<<", ";
		}
		cout<<"} \tEdges={";
		for( it = points_set_halfedges[point_id].begin(); it!=points_set_halfedges[point_id].end(); it++){
			cout<<*it<<", ";
		}
		cout<<"}";


	}

	void printFaces(int hedge_id){
		if(hedge_id<0) return;
		set<int> fn = halfedges_set_faces[hedge_id];
		cout<<" Faces={ ";
		for(set<int>::iterator it = fn.begin(); it!=fn.end(); it++){
			cout<<*it<<", ";
		}
		cout<<"} ";
		
	}


};

}

#endif // __SJ_GRAPH_LIST_H__