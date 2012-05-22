#ifndef __SJ__REFINEMENT_H__
#define __SJ__REFINEMENT_H__

#include <iostream>
#include <list>
#include <set>
#include <vector>
#include "sjDataType.h"
#include "sjUtils.h"
#include "sjGraphSkeleton.h"
#include "sjSimplificator.h"
#include "sjPipeFilter.h"
#include "sjObserver.h"

using namespace std;

namespace sj{

	class sjRefinement: public sjPolyhedronPipe::sjFilter, public sjObserver{
	public:
		sjRefinement();
		void proccesEvent(sjEvent * evt);
		void setParameters(sjPolyhedronPipe::ParametersType *){}
		void setMesh(sjPolyhedronPipe::PolyhedronType meshg){
			mesh_G = meshg;
		}
		size_t getNumberOfVertex(){
			return mesh_G.size_of_vertices();
		}
		sjPolyhedronPipe::PolyhedronType iterate();
		sjPolyhedronPipe::PolyhedronType getMesh(){
			return mesh_G;
		}
		sjPolyhedronPipe::ParametersType * getParameters(){
			return NULL;
		}

		bool isTerminalNode(int vid);
		bool isLineNode(int vid);
		bool isJunctionNode(int vid);
		vector<vector<int>> getBoundaries(int vid);
		set<int> getNeighborsToMeshRegion(set<int> mesh_region);
		sjVector_3 calculateWeightedAverageDisplacement(vector<vector<int>> boundaries, int node_id);
		vector<int> getAdjacentEdgesinBoundaryToVertex(vector<int> boundary, int vid);
		double getLengthAdjacentEdges(vector<int> adjacent_points, int vid);
		set<int> getValidNodesFromHedgeTrue();

		void embeddingRefinement();
		void convertPolyhedronToSkeleton();

		sjGraphSkeletonType sjskeleton;
		sjGraphSkeletonType original_skeleton;
		sjGraphSkeletonType contracted_skeleton;
		sjPolyhedron mesh_G;
		sjPolyhedron original_mesh_G;
		bool refinement_complete;

	};
}
#endif // __SJ__REFINEMENT_H__