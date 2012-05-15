#include "sjRefinement.h"

using namespace sj;

bool sjRefinement::isTerminalNode(int vid){
	if(sjskeleton.getNeighborsToPoint(vid).size() == 1) return true;
	return false;
}

bool sjRefinement::isLineNode(int vid){
	set<int> neighbors_vid = sjskeleton.getNeighborsToPoint(vid);
	if(neighbors_vid.size() == 2){
		vector<int> vect_neighbors;
		for(set<int>::iterator sit = neighbors_vid.begin(); sit!= neighbors_vid.end(); sit++){
			vect_neighbors.push_back(*sit);
		}
		if(sjskeleton.getNeighborsIntersection(vect_neighbors[0], vect_neighbors[1]).size() == 1) 
			return true;
	}
	return false;
}

bool sjRefinement::isJunctionNode(int vid){
	set<int> neighbors_vid = sjskeleton.getNeighborsToPoint(vid);
	if(neighbors_vid.size() > 2) return true;
	else if(!isTerminalNode(vid) && !isLineNode(vid)) return true;
	return false;
}

vector<vector<int>> sjRefinement::getBoundaries(int vid){
	vector<vector<int>> boundaries;
	vector<int> boundarie_i;
	set<int> mesh_region = sjskeleton.points_set_delete_points[vid];
	set<int> neighbors_vid = sjskeleton.getNeighborsToPoint(vid);
	vector<int> vect_neighbors_vid;
	std::copy(neighbors_vid.begin(), neighbors_vid.end(), std::back_inserter(vect_neighbors_vid));

	if(isTerminalNode(vid)){
		boundarie_i = vector<int>();
		std::copy(mesh_region.begin(), mesh_region.end(), std::back_inserter(boundarie_i));
		boundaries.push_back(boundarie_i);
	}else if(isLineNode(vid)){
		
		int neighbors_vid_a = vect_neighbors_vid[0];
		int neighbors_vid_b = vect_neighbors_vid[1];
		set<int> neighbors_a = sjskeleton.points_set_delete_points[neighbors_vid_a];
		set<int> neighbors_b = sjskeleton.points_set_delete_points[neighbors_vid_b];

		set<int> neighbors_mesh_region_a = getNeighborsToMeshRegion(neighbors_a);
		set<int> neighbors_mesh_region_b = getNeighborsToMeshRegion(neighbors_b);

		vector<int> boundarie_a = vector<int>();
		set_intersection(
			neighbors_mesh_region_a.begin(),neighbors_mesh_region_a.end(), 
			mesh_region.begin(), mesh_region.end(),
			back_inserter(boundarie_a));
		vector<int> boundarie_b = vector<int>();
		set_intersection(
			neighbors_mesh_region_b.begin(),neighbors_mesh_region_b.end(), 
			mesh_region.begin(), mesh_region.end(),
			back_inserter(boundarie_b));

		boundaries.push_back(boundarie_a);
		boundaries.push_back(boundarie_b);

	}else{
		set<int>::iterator it_neighbor_id;
		for(it_neighbor_id = neighbors_vid.begin(); it_neighbor_id!= neighbors_vid.end(); it_neighbor_id++){
			set<int> neighbors_to_neighbor = sjskeleton.points_set_delete_points[*it_neighbor_id];
			set<int> neighbors_mesh_region_neighbor = getNeighborsToMeshRegion(neighbors_to_neighbor);
			vector<int> boundarie_neighbor = vector<int>();
			set_intersection(
				neighbors_mesh_region_neighbor .begin(),neighbors_mesh_region_neighbor.end(), 
				mesh_region.begin(), mesh_region.end(),
				back_inserter(boundarie_neighbor));
			boundaries.push_back(boundarie_neighbor);
		}
	}
	return boundaries;
}

set<int> sjRefinement::getNeighborsToMeshRegion(set<int> mesh_region){
	set<int> neighbors_mesh_region;
	set<int>::iterator sit;	
	for(sit=mesh_region.begin(); sit!= mesh_region.end(); sit++){
		set<int> original_neighbors = original_skeleton.getNeighborsToPoint(*sit);
		neighbors_mesh_region.insert(original_neighbors.begin(), original_neighbors.end());
	}
	return neighbors_mesh_region;
}

vector<int> sjRefinement::getAdjacentEdgesinBoundaryToVertex(vector<int> boundary, int vid){
	set<int> neighbors_to_vid = original_skeleton.getNeighborsToPoint(vid);
	set<int> set_boundary = set<int>(boundary.begin(), boundary.end());
	vector<int> adjacent_points;
	set_intersection(
		neighbors_to_vid.begin(), neighbors_to_vid.end(),
		set_boundary.begin(), set_boundary.end(),
		back_inserter(adjacent_points));
	return adjacent_points;
}

double sjRefinement::getLengthAdjacentEdges(vector<int> adjacent_points, int vid){
	double length_i = 0.0;
	sjPoint_3 point_i = original_skeleton.points_data[vid].getPoint_3();
	for(vector<int>::iterator it = adjacent_points.begin(); it != adjacent_points.end(); it++){
		length_i = length_i + distance2Points(point_i, original_skeleton.points_data[*it].getPoint_3());
	}
	return length_i;
}

sjVector_3 sjRefinement::calculateWeightedAverageDisplacement(vector<vector<int>> boundaries, int node_id){
	sjVector_3 d_vect3(0.0,0.0,0.0);
	double suml = 0.0;
	double lij = 0.0;
	vector<int>::iterator it;
	if(isTerminalNode(node_id)){
		vector<int> boundary = boundaries[0];
		for(it = boundary.begin(); it!= boundary.end(); it++){
			double length_i = getLengthAdjacentEdges(getAdjacentEdgesinBoundaryToVertex(boundary, *it), *it);
			d_vect3 = d_vect3 + length_i * (contracted_skeleton.points_data[*it].getPoint_3() - original_skeleton.points_data[*it].getPoint_3());
			lij = lij + length_i;
		}		
		if(lij != 0.0)
			d_vect3 = d_vect3  / lij;
	}else if(isLineNode(node_id)){
		sjVector_3 d_vect3_a;
		sjVector_3 d_vect3_b;
		vector<int> boundary_a = boundaries[0];
		vector<int> boundary_b = boundaries[0];

	}

	return d_vect3;
}

a::a(){}