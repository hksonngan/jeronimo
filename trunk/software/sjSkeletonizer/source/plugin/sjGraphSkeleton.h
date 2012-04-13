#ifndef __SJ_GRAPH_LIST_H__
#define __SJ_GRAPH_LIST_H__

#include <iterator>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

#define SJGRAPH_INVALID_ID -1

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

template <class T_PointClass, class T_GetIdClass>
class sjGraphSkeleton{
public:
	typedef typename T_PointClass PointType;
	vector<PointType>			points_data;
	vector<bool>				points_bool;
	vector<sjGraphHalfedge>		halfedges_data;
	vector<bool>				halfedges_bool;
	vector<int>					halfedges_pair;
	vector<set<int> >			points_set_halfedges;	

	sjGraphSkeleton(){}

	void init(int a_size_points, int a_size_halfedges){
		points_data = vector<PointType>(a_size_points, PointType());
		points_bool = vector<bool>(a_size_points, false);

		halfedges_data = vector<sjGraphHalfedge>(a_size_halfedges, sjGraphHalfedge());
		halfedges_bool = vector<bool>(a_size_halfedges, false);
		points_set_halfedges = vector<set<int> >(a_size_halfedges, set<int>());
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

	bool deleteHalfedge(int hedge_id){
		if(hedge_id >= halfedges_bool.size() || hedge_id<0) return false;
		int hedge_opposite_id = halfedges_data[hedge_id].hedge_opposite_id;
		int incident_point_id = halfedges_data[hedge_id].point_incident_id;
		int opposite_point_id = halfedges_data[hedge_id].point_opposite_id;

		points_set_halfedges[incident_point_id].erase(hedge_id);
		points_set_halfedges[opposite_point_id].erase(hedge_opposite_id);
		halfedges_bool[hedge_id] = false;
		halfedges_bool[hedge_opposite_id] = false;
		return true;
	}

	bool isCollapseTunnel(int hedge_id){
		if(hedge_id >= halfedges_bool.size() || hedge_id<0) return false;

		int pointAid = halfedges_data[hedge_id].point_incident_id;
		int pointBid = halfedges_data[hedge_id].point_opposite_id;

		set<int> neighbors_A = getNeighborsToPoint(pointAid);
		set<int> neighbors_B = getNeighborsToPoint(pointBid);
		vector<int> neighbors_intersection_AB;

		set_intersection(neighbors_A.begin(),neighbors_A.end(), neighbors_B.begin(), neighbors_B.end(), back_inserter(neighbors_intersection_AB));

		if(neighbors_intersection_AB.size()>2){
			return false;
		}else{
			if(neighbors_intersection_AB.size() == 2){
				set<int> neighbors_C = getNeighborsToPoint(neighbors_intersection_AB[0]);
				int point_id_D =  neighbors_intersection_AB[1];

				if(neighbors_C.find(point_id_D) == neighbors_C.end()){
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

	bool collapseHalfedge(int hedge_id){
		//set_intersection(neighbors_A.begin(),neighbors_A.end(), neighbors_B.begin(), neighbors_B.end(),    back_inserter(neighbors_intersection_AB));
		if(hedge_id<0 ||hedge_id>=halfedges_bool.size()) return false;
		if(!isCollapseTunnel(hedge_id)) return false;

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
		halfedges_to_delete.insert(hedge_id);

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

	
	/*void printData(){
		T_GetIdClass printObj;
		vector<PointType>::iterator it;
		for(it = points_data.begin(); it!=points_data.end(); it++){
			printObj.print(*it);
			cout<<"\t";
			printNeighbors(getIdFromPoint(*it));
			cout<<endl;
		}
		vector<sjGraphHalfedge>::iterator ithe;
		for(ithe = halfedges_data.begin(); ithe!=halfedges_data.end(); ithe++){
			cout<<"Halfedge ["<<(*ithe).id<<"]\t["<<(*ithe).hedge_opposite_id <<"] \t= { "<<(*ithe).point_opposite_id<<" -> "<<(*ithe).point_incident_id<<" }";
			
				cout<<endl;
		}
	}

	void printNeighbors(int point_id){
		set<int> pn = getNeighborsToPoint(point_id);
		cout<<" Neighbors={ ";
		for(set<int>::iterator it = pn.begin(); it!=pn.end(); it++){
			cout<<*it<<", ";
		}
		cout<<"}";

	}*/


};

}
#endif // __SJ_GRAPH_LIST_H__